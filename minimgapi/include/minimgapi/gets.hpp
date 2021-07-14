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

#pragma once
#ifndef MINIMGAPI_GETS_HPP_INCLUDED
#define MINIMGAPI_GETS_HPP_INCLUDED

#include "minimgapi_base.hpp"

#include "assures_and_compares.hpp"

#include "raw/gets.hpp"

namespace minimg {


#ifdef MINIMGAPI_STOPWATCH_NORMAL_INTERFACE
DECLARE_MINSTOPWATCH(swGetBitsPerPixel,    "GetBitsPerPixel");
DECLARE_MINSTOPWATCH(swGetBytesPerLine,    "GetBytesPerLine");
DECLARE_MINSTOPWATCH(swGetLine,            "GetLine");
DECLARE_MINSTOPWATCH(swGetLineEx,          "GetLineEx");
DECLARE_MINSTOPWATCH(swGetRegion,          "GetRegion");
DECLARE_MINSTOPWATCH(swChangeRegion,       "ChangeRegion");
#endif


static MINIMGAPI_CPP_API int32_t GetBitsPerPixel(const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swGetBitsPerPixel);

  MR_PROPAGATE_ERROR(AssurePrototypeIsValid<CT_PRECONDITION>(image));

  return minimg_raw::GetBitsPerPixelRaw(image);
}


static MINIMGAPI_CPP_API int32_t GetBytesPerLine(const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swGetBytesPerLine);

  MR_PROPAGATE_ERROR(AssurePrototypeIsValid<CT_PRECONDITION>(image));

  return minimg_raw::GetBytesPerLineRaw(image);
}


template<typename T>
static MINIMGAPI_CPP_API T *GetLine(const MinImg &image, int32_t y) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swGetLine);

  MR_BACKED_CALL(AssureImageIsAllocated<CT_PRECONDITION>(image),
      return nullptr);
  if (0 > y || image.height <= y)
    return nullptr;

  return minimg_raw::GetLineRaw<T>(image, y);
}


template<typename T>
static MINIMGAPI_CPP_API const T *GetLineEx(
    const MinImg &image,
    int32_t       y,
    BorderOption  border   = BO_VOID,
    const T      *p_canvas = nullptr) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swGetLineEx);

  MR_BACKED_CALL(AssureImageIsAllocated<CT_PRECONDITION>(image),
      return nullptr);
  if (BO_IGNORE == border) {
    MR_BACKED_CALL(minimg_raw::AssureOverrunPossibleRaw(
        image, 0, y, 0, y - image.height),
      return nullptr);
  } else if (BO_CONSTANT == border) {
    /// MAYBE: Is this check needed?
    if (!p_canvas)
      return nullptr;
  }

  return minimg_raw::GetLineExRaw<T>(image, y, border, p_canvas);
}


static MINIMGAPI_CPP_API MinResult GetRegion(
    MinImg       &dst_image,
    const MinImg &src_image,
    int32_t       x0,
    int32_t       y0,
    int32_t       width,
    int32_t       height,
    bool          get_flipped = false) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swGetRegion);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(dst_image));
  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(src_image));
  if (0 > width || 0 > height)
    return MR_CONTRACT_VIOLATION;
  MR_PROPAGATE_ERROR(minimg_raw::AssureOverrunPossibleRaw(src_image,
      x0, y0, x0 + width - src_image.width, y0 + height - src_image.height));
  int32_t bit_shift = x0 * minimg_raw::GetBitsPerPixelRaw(src_image);
  if (bit_shift & 0x07)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::GetRegionRaw(
      dst_image, src_image, x0, y0, width, height, get_flipped);
}


static MINIMGAPI_CPP_API MinResult ChangeRegion(
    MinImg &image,
    int32_t delta_lft,
    int32_t delta_top,
    int32_t delta_rgt,
    int32_t delta_btm,
    bool    flip = false) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swChangeRegion);

  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(image));
  if (image.is_owner)
    return MR_CONTRACT_VIOLATION;
  if (0 > image.width + delta_rgt - delta_lft ||
      0 > image.height + delta_btm - delta_top)
    return MR_CONTRACT_VIOLATION;
  MR_PROPAGATE_ERROR(minimg_raw::AssureOverrunPossibleRaw(
      image, delta_lft, delta_top, delta_rgt, delta_btm));
  const int32_t bits_per_pixel = minimg_raw::GetBitsPerPixelRaw(image);
  if (bits_per_pixel & 0x07)
    /// TODO: Implement check of bit shift here
    return MR_NOT_IMPLEMENTED;

  return minimg_raw::ChangeRegionRaw(
      image, delta_lft, delta_top, delta_rgt, delta_btm, flip);
}


} // namespace minimg

#endif // #ifndef MINIMGAPI_GETS_HPP_INCLUDED
