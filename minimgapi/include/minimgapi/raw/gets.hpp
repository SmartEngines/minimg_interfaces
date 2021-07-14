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
#ifndef MINIMGAPI_RAW_GETS_HPP_INCLUDED
#define MINIMGAPI_RAW_GETS_HPP_INCLUDED

#include <cstddef>

#include "minimgapi_raw_base.hpp"
#include "get_coord.hpp"

namespace minimg_raw {


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swGetLastYRaw,           "GetLastYRaw");
DECLARE_MINSTOPWATCH(swGetBitsPerPixelRaw,    "GetBitsPerPixelRaw");
DECLARE_MINSTOPWATCH(swGetBytesPerLineRaw,    "GetBytesPerLineRaw");
DECLARE_MINSTOPWATCH(swGetLineRaw,            "GetLineRaw");
DECLARE_MINSTOPWATCH(swGetLineExRaw,          "GetLineExRaw");
DECLARE_MINSTOPWATCH(swGetRegionRaw,          "GetRegionRaw");
DECLARE_MINSTOPWATCH(swChangeRegionRaw,       "ChangeRegionRaw");
DECLARE_MINSTOPWATCH(swGetImageMemoryPtrRaw,  "GetImageMemoryPtrRaw");
DECLARE_MINSTOPWATCH(swGetDeltaRectLimitRaw,  "GetDeltaRectLimitRaw");
#endif


static MINIMGAPI_RAW_API int32_t GetLastYRaw(const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetLastYRaw);

  return image.height ? image.height - 1 : 0;
}


static MINIMGAPI_RAW_API int32_t GetBitsPerPixelRaw(const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetBitsPerPixelRaw);

  return image.channels * (1 << LogBitSizeOfMinType(image.scalar_type));
}


static MINIMGAPI_RAW_API int32_t GetBytesPerLineRaw(const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetBytesPerLineRaw);

  return (image.width * GetBitsPerPixelRaw(image) + 7) >> 3;
}


template<typename T> static MINIMGAPI_RAW_API T* GetLineRaw(
    const MinImg& image,
    int32_t       y) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetLineRaw);

  return reinterpret_cast<T*>(
      image.p_zero_line + static_cast<ptrdiff_t>(y) * image.stride);
}


template<typename T>
static MINIMGAPI_RAW_API const T* GetLineExRaw(
    const MinImg& image,
    int32_t       y,
    BorderOption  border   = BO_VOID,
    const T*      p_canvas = nullptr) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetLineExRaw);

  const int32_t ht = image.height;
  switch (border) {
  case BO_IGNORE:
    return GetLineRaw<T>(image, GetCoordRaw<BO_IGNORE>(y, ht));
  case BO_REPEAT:
    return GetLineRaw<T>(image, GetCoordRaw<BO_REPEAT>(y, ht));
  case BO_CYCLIC:
    return GetLineRaw<T>(image, GetCoordRaw<BO_CYCLIC>(y, ht));
  case BO_SYMMETRIC:
    return GetLineRaw<T>(image, GetCoordRaw<BO_SYMMETRIC>(y, ht));
  case BO_CONSTANT:
    return 0 > y || ht <= y ? p_canvas : GetLineRaw<T>(image, y);
  case BO_VOID:
    return 0 > y || ht <= y ? nullptr  : GetLineRaw<T>(image, y);
  default:
    return nullptr;
  }
}


static MINIMGAPI_RAW_API MinResult GetRegionRaw(
    MinImg&       dst_image,
    const MinImg& src_image,
    int32_t       x0,
    int32_t       y0,
    int32_t       width,
    /// TODO: Maybe add some sugar to get "to the end"?
    int32_t       height,
    bool          get_flipped = false) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetRegionRaw);

  int32_t bit_shift = x0 * GetBitsPerPixelRaw(src_image);
  dst_image = src_image;
  dst_image.width = width;
  dst_image.height = height;
  /// GetLine is used here "unsafely", but it's safe if the region is
  /// checked to belong to landlord
  dst_image.p_zero_line = GetLineRaw<uint8_t>(src_image, y0) + (bit_shift >> 3);
  /// TODO: Maybe refactor flipping
  if (get_flipped) {
    dst_image.p_zero_line = GetLineRaw<uint8_t>(dst_image, GetLastYRaw(dst_image));
    dst_image.stride *= -1;
  }
  dst_image.p_alloc_info = src_image.p_alloc_info;
  dst_image.is_owner = false;
  return MR_MRR;
}


static MINIMGAPI_RAW_API MinResult ChangeRegionRaw(
    MinImg& image,
    int32_t delta_lft,
    int32_t delta_top,
    int32_t delta_rgt,
    int32_t delta_btm,
    bool    flip) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swChangeRegionRaw);

  const int32_t bits_per_pixel = GetBitsPerPixelRaw(image);
  if (bits_per_pixel & 0x07)
    /// FIXME: Check bit_shifts for all 4 sides // not here, in minimgapi.hpp
    /// or not
    return MR_NOT_IMPLEMENTED;
  const int32_t bytes_per_pixel = bits_per_pixel >> 3;
  image.p_zero_line += delta_top * static_cast<ptrdiff_t>(image.stride) +
      delta_lft * bytes_per_pixel;
  image.width += delta_rgt - delta_lft;
  image.height += delta_btm - delta_top;
  if (flip) {
    image.p_zero_line = GetLineRaw<uint8_t>(image, GetLastYRaw(image));
    image.stride *= -1;
  }
  return MR_MRR;
}


static MINIMGAPI_RAW_API uint8_t* GetImageMemoryPtrRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetImageMemoryPtrRaw);

  return image.stride < 0 ? GetLineRaw<uint8_t>(image, GetLastYRaw(image))
                           : image.p_zero_line;
}


/// Assumes ValidOnMemory
static MINIMGAPI_RAW_API MinResult GetDeltaRectLimitRaw(
    int32_t&      delta_lft,
    int32_t&      delta_top,
    int32_t&      delta_rgt,
    int32_t&      delta_btm,
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetDeltaRectLimitRaw);

  if (!image.p_alloc_info || !image.p_alloc_info->stride_) {
    delta_lft = delta_top = delta_rgt = delta_btm = 0;
    return MR_MRR;
  }

  const MinImgAllocInfo& alloc_info = *image.p_alloc_info;

  const int32_t bits_per_pixel = GetBitsPerPixelRaw(image);

  const uint8_t* const image_begin = GetImageMemoryPtrRaw(image);
  const ptrdiff_t delta_mem_ptr = image_begin - alloc_info.p_land_;

  const int32_t y_lft_top = static_cast<int32_t>(
      delta_mem_ptr / alloc_info.stride_);
  const int32_t x_lft_top = static_cast<int32_t>(
      delta_mem_ptr % alloc_info.stride_ << 3) / bits_per_pixel;
  const int32_t pixels_in_stride = static_cast<int32_t>(
      alloc_info.stride_ << 3) / bits_per_pixel;

  delta_lft = -x_lft_top;
  delta_rgt = pixels_in_stride - image.width - x_lft_top;
  if (image.stride < 0) {
    delta_top = y_lft_top + image.height - alloc_info.height_;
    delta_btm = y_lft_top;
  } else {
    delta_top = -y_lft_top;
    delta_btm = alloc_info.height_ - image.height - y_lft_top;
  }

  return MR_MRR;
}


} // namespace minimg::raw

#endif // #ifndef MINIMGAPI_RAW_GETS_HPP_INCLUDED
