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
// This module is meant to be used with minimgprc tests only
//

#pragma once
#include <minutils/smartptr.h>
#include <minbase/mintyp.h>


namespace mincompare {
  template<typename TData>
  MUSTINLINE
  typename std::enable_if<std::numeric_limits<TData>::is_integer, bool>::type
  equal(const TData& a, const TData& b, const TData& eps) {
    const TData diff = a > b ? a - b : b - a;
    return diff <= eps;
  }
  template<typename TData>
  MUSTINLINE
  typename std::enable_if<!std::numeric_limits<TData>::is_integer, bool>::type
  equal(const TData& a, const TData& b, const TData& eps) {
    const TData diff = a > b ? a - b : b - a;
    if (diff > eps) {
      return false;
    }
    if ((a != a) != (b != b)) {
      return false;
    }
    return true;
  }
}

// compare all types
template<typename TData>
bool CompareEps(const MinImg *a_img, const MinImg *b_img, const TData eps)
{
  if (CompareMinImagePrototypes(a_img, b_img))
    return false;
  const int len = a_img->width * a_img->channels;
  for (int y = 0; y < a_img->height; y++) {
    const TData *a_line = minimg_raw::GetLineRaw<TData>(*a_img, y);
    const TData *b_line = minimg_raw::GetLineRaw<TData>(*b_img, y);
    for (int x = 0; x < len; x++) {
      if (!mincompare::equal(a_line[x], b_line[x], eps))
        return false;
    }
  }
  return true;
}

// compare uint1 type
bool CompareEps(const MinImg *p_img_a, const MinImg *p_img_b, const bool eps=false)
{
  // for TYP_UINT1 images
  if (eps)
    return true;
  // might call
  if (_AssureMinImageIsValid(p_img_a) != NO_ERRORS)
    return false;
  if (_AssureMinImageIsValid(p_img_b) != NO_ERRORS)
    return false;
  if (_CompareMinImagePrototypes(p_img_a, p_img_b) != NO_ERRORS)
    return false;
  int height = p_img_a->height;

  const int bit_line_width = _GetMinImageBitsPerPixel(p_img_a) * p_img_a->width;
  const int tail_width = bit_line_width & 0xFU;
  for (int y = 0; y < height; ++y) {
    const uint8_t* p_line_a = minimg_raw::GetLineRaw<uint8_t>(*p_img_a, y);
    const uint8_t* p_line_b = minimg_raw::GetLineRaw<uint8_t>(*p_img_b, y);
    if (::memcmp(p_line_a, p_line_b, bit_line_width >> 3))
      return false;
    if (tail_width) {
      uint8_t diff = p_line_a[bit_line_width >> 3] ^ p_line_a[bit_line_width >> 3];
      for (int bit_no = 0; bit_no < tail_width; ++bit_no) {
        if (diff & 1)
          return false;
        diff >>= 1;
      }
    }
  }
  return true;
}
