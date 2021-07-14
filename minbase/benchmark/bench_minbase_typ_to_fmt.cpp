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

#include <benchmark/benchmark.h>
#include <minbase/mintyp.h>
#include <minbase/min_pp_mintyp.h>

#include <cassert>
#include <random>


MUSTINLINE MinFmt MinFormatOfMinTypeRawInl(MinTyp typ) {
  assert(typ >= TYP_INVALID && typ < MINTYP_COUNT);
  static const MinFmt formats[MINTYP_COUNT + 2] = {
    FMT_INVALID,
    FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT,
    FMT_INT,  FMT_INT,  FMT_INT,  FMT_INT,
    FMT_REAL, FMT_REAL, FMT_REAL,
    FMT_INVALID
  };
  return formats[typ + 1];
}


MUSTINLINE MinFmt MinFormatOfMinTypeHalfRawInl(MinTyp typ) {
  assert(typ >= TYP_INVALID && typ < MINTYP_COUNT);
  static const MinFmt formats[MINTYP_COUNT + 2] = {
    FMT_INVALID,
    FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT,
    FMT_INT,  FMT_INT,  FMT_INT,  FMT_INT,
    FMT_REAL, FMT_REAL, FMT_REAL,
    FMT_INVALID
  };
  MinFmt ret = formats[typ + 1];
  return typ < 0 || MINTYP_COUNT <= typ ? FMT_INVALID : ret;
}


MUSTINLINE MinFmt MinFormatOfMinTypeInl(MinTyp typ) {
  static const MinFmt formats[MINTYP_COUNT] = {
    FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT,
    FMT_INT,  FMT_INT,  FMT_INT,  FMT_INT,
    FMT_REAL, FMT_REAL, FMT_REAL
  };
  return typ < 0 || MINTYP_COUNT <= typ ? FMT_INVALID : formats[typ];
}


MUSTINLINE MinFmt MinFormatOfMinTypeSwInl(MinTyp typ) {
  switch (typ) {
  case TYP_UINT1:   return FMT_UINT;
  case TYP_UINT8:   return FMT_UINT;
  case TYP_UINT16:  return FMT_UINT;
  case TYP_UINT32:  return FMT_UINT;
  case TYP_UINT64:  return FMT_UINT;

  case TYP_INT8:    return FMT_INT;
  case TYP_INT16:   return FMT_INT;
  case TYP_INT32:   return FMT_INT;
  case TYP_INT64:   return FMT_INT;

  case TYP_REAL16:  return FMT_REAL;
  case TYP_REAL32:  return FMT_REAL;
  case TYP_REAL64:  return FMT_REAL;

  default:          return FMT_INVALID;
  }
}


MINBASE_API MinFmt MinFormatOfMinTypeRaw(MinTyp typ) {
  return MinFormatOfMinTypeRawInl(typ);
}


MINBASE_API MinFmt MinFormatOfMinTypeHalfRaw(MinTyp typ) {
  return MinFormatOfMinTypeHalfRawInl(typ);
}


MINBASE_API MinFmt MinFormatOfMinTypeSw(MinTyp typ) {
  return MinFormatOfMinTypeSwInl(typ);
}


constexpr int sz = 100000;
constexpr int seed = 1337;


#define DECLARE_BM_FOR_FUNCTION(Function) \
  static void MIN_PP_CONCAT(BM, Function)(benchmark::State& state) { \
    MinTyp typs[MINTYP_COUNT + 1];  \
    typs[MINTYP_COUNT] = TYP_INVALID; \
    for (int t = 0; t < MINTYP_COUNT; ++t)  \
      typs[t] = static_cast<MinTyp>(t); \
    MinFmt fmt[sz]; \
    MinTyp typ[sz]; \
    std::mt19937_64 mt(seed); \
    std::uniform_int_distribution<int> distr(0, MINTYP_COUNT);  \
    for (int i = 0; i < sz; ++i)  \
      typ[i] = typs[distr(mt)]; \
    for (auto _ : state) {  \
      benchmark::DoNotOptimize(fmt);  \
      benchmark::DoNotOptimize(typ);  \
      for (int i = 0; i < sz; ++i)  \
        fmt[i] = Function(typ[i]);  \
      benchmark::ClobberMemory(); \
    } \
  } \
  BENCHMARK(MIN_PP_CONCAT(BM, Function))

DECLARE_BM_FOR_FUNCTION(MinFormatOfMinType);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeHalfRaw);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeRaw);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeSw);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeInl);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeHalfRawInl);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeRawInl);
DECLARE_BM_FOR_FUNCTION(MinFormatOfMinTypeSwInl);

#undef DECLARE_BM_FOR_FUNCTION


BENCHMARK_MAIN();
