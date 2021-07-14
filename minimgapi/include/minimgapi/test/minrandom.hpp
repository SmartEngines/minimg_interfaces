/*

Copyright 2021 Smart Engines Service LLC

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

//
// This module is meant to be used with minimgprc tests and benchmarks only
//

#pragma once
#include <minbase/minstd.hpp>
#include <minimgapi/minimgapi.h>
#include <minimgapi/minimgapi-helpers.hpp>  // for `GetMinTypByCType`
#include <cstdlib>  // for `rand` function

class RNG
{
  // https://en.wikipedia.org/wiki/Xorshift
  uint64_t x;
public:
  inline RNG(uint64_t const* seed=0) : x((seed ? *seed : rand()) + 5631205976402044421ULL /* <- large enough */) {}
  inline uint64_t operator () ()
  {
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * 2685821657736338717ULL;
  }
};

static inline int global_random_bytes(uint8_t *dst_line, int len, RNG &rng)
{
  int x = 0;
  for (; x < (len & ~0x07); x += sizeof(uint64_t))
    *reinterpret_cast<uint64_t*>(dst_line + x) = rng();
  if (x < len)
  {
    uint64_t rnd = rng();
    ::memcpy(dst_line + x, &rnd, len - x);
  }
  return 0;
}

// Fill image with random bytes regardless of image type (including bit images).
struct RandomBytes
{
  RNG rng;
  int operator () (int) { return NO_ERRORS; }
  int operator() (uint8_t *dst_line, int len) {
    return global_random_bytes(dst_line, len, rng);
  }
};

// Random values for integers and [0-1] for floats
struct RandomSaneValues
{
  RNG rng;
  int(RandomSaneValues::*_func)(uint8_t *dst_line, int len);
  int operator()(int typ) {
    switch (typ)
    {
    case TYP_UINT8:
    case TYP_INT8:
    case TYP_UINT16:
    case TYP_INT16:
    case TYP_UINT32:
    case TYP_INT32:
    case TYP_UINT64:
    case TYP_INT64:
      _func = &RandomSaneValues::random_bytes;
      break;
    case TYP_REAL32:
      _func = &RandomSaneValues::random_0_1<real32_t>;
      break;
    case TYP_REAL64:
      _func = &RandomSaneValues::random_0_1<real64_t>;
      break;
    default:
      return BAD_ARGS;
    }
    return NO_ERRORS;
  }

  int random_bytes(uint8_t *dst_line, int len) {
    return global_random_bytes(dst_line, len, rng);
  }

  template<typename TData>
  int random_0_1(uint8_t *dst_line, int len)
  {
    TData *dst_line_ = reinterpret_cast<TData*>(dst_line);
    static const TData max_val = static_cast<TData>(0xFFFFFFFFFFFFFFFFULL);
    len /= sizeof(TData);
    for (int x = 0; x < len; ++x)
      dst_line_[x] = static_cast<TData>(rng()) / max_val;
    return 0;
  }

  template<typename TData>
  int not_nan(uint8_t *dst_line, int len)
  {
    const int ret = global_random_bytes(dst_line, len, rng);
    len /= sizeof(TData);
    TData *line = reinterpret_cast<TData*>(dst_line);
    for (int x = 0; x < len; ++x)
      if (line[x] != line[x])
        line[x] = 0;
    return ret;
  }

  int operator() (uint8_t *dst_line, int len) {
    return (this->*_func)(dst_line, len);
  }
};

// Random values, only not allowing NaN for reals
struct RandomNotNaNValues : public RandomSaneValues
{
  using RandomSaneValues::operator();
  int operator() (int typ)
  {
    const int ret = RandomSaneValues::operator()(typ);
    if (ret == NO_ERRORS)
      switch (typ) {
      case TYP_REAL32:
        _func = &RandomSaneValues::not_nan<real32_t>;
        break;
      case TYP_REAL64:
        _func = &RandomSaneValues::not_nan<real64_t>;
        break;
      }
    return ret;
  }
};

// Random values in specified range
template<typename TData>
struct RandomMinMax
{
  TData min_val;
  TData range;
  RNG rng;
  RandomMinMax(TData min_val, TData max_val) :
    min_val(min_val), range(max_val - min_val) { }
  int operator() (int typ) {
    if (typ != GetMinTypByCType<TData>())
      return BAD_ARGS;
    return NO_ERRORS;
  }
  int operator() (uint8_t *dst_line, int len) {
    len /= sizeof(TData);
    TData *line = reinterpret_cast<TData*>(dst_line);
    for (int x = 0; x < len; ++x)
      line[x] = rand_mm(line[x]);
    return NO_ERRORS;
  };
  // non-inclusive: [)
  template<typename T>
  inline typename
  std::enable_if<std::numeric_limits<T>::is_integer, T>::type
  rand_mm(T /*val*/) {
    return min_val + rng() % range;
  }
  // inclusive: []
  template<typename T>
  inline typename
  std::enable_if<!std::numeric_limits<T>::is_integer, T>::type
  rand_mm(T /*val*/)
  {
    return min_val + (rng() / static_cast<TData>(0xFFFFFFFFFFFFFFFFULL)
                      ) * range;
  }
};

// random values in range [0-255]
struct Random255
{
  RNG rng;
  Random255(uint64_t const* seed=0): rng(seed) {}
  int(Random255::*_func)(uint8_t *dst_line, int len);
  int operator() (int typ) {
    switch (typ) {
    case TYP_UINT8 : _func = &Random255::random<uint8_t >; break;
    case TYP_INT8  : _func = &Random255::random<int8_t  >; break;
    case TYP_UINT16: _func = &Random255::random<uint16_t>; break;
    case TYP_INT16 : _func = &Random255::random<int16_t >; break;
    case TYP_UINT32: _func = &Random255::random<uint32_t>; break;
    case TYP_INT32 : _func = &Random255::random<int32_t >; break;
    case TYP_UINT64: _func = &Random255::random<uint64_t>; break;
    case TYP_INT64 : _func = &Random255::random<int64_t >; break;
    case TYP_REAL32: _func = &Random255::random<real32_t>; break;
    case TYP_REAL64: _func = &Random255::random<real64_t>; break;
    default:
      return BAD_ARGS;
    }
    return NO_ERRORS;
  }
  template<typename TData>
  int random(uint8_t *dst_line, int len)
  {
    static const int sizeof_uint64_t = static_cast<int>(sizeof(uint64_t));
    len /= sizeof(TData);
    TData *line = reinterpret_cast<TData*>(dst_line);
    int x = 0;
    for (; x < (len & ~0x07); x += sizeof_uint64_t)
    {
      const uint64_t val = rng();
      for (int shift = 0; shift < sizeof_uint64_t; ++shift)
        line[x + shift] = static_cast<TData>(
          reinterpret_cast<const uint8_t*>(&val)[shift]);
    }
    for (; x < len; ++x)
      line[x] = static_cast<TData>(rng() & 0xFF);
    return NO_ERRORS;
  }
  int operator() (uint8_t *dst_line, int len) {
    return (this->*_func)(dst_line, len);
  }
};

template<typename Functor>
int ApplyLineFunctor(MinImg const *dst, Functor functor)
{
  PROPAGATE_ERROR(AssureMinImageIsValid(dst));
  const int typ = GetMinImageType(dst);
  if (typ < 0)
    return BAD_ARGS;
  const int init_ret = functor(typ);
  if (init_ret != 0)
    return init_ret;
  const int height = dst->height;
  const int len_bytes = GetMinImageBytesPerLine(dst);
  for (int y = 0; y < height; ++y)
  {
    uint8_t *dst_line = dst->p_zero_line + y * dst->stride;
    const int ret = functor(dst_line, len_bytes);
    if (ret != 0)
      return ret;
  }
  return 0;
}
