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
#ifndef MINIMGAPI_RAW_ASSURES_AND_COMPARES_HPP_INCLUDED
#define MINIMGAPI_RAW_ASSURES_AND_COMPARES_HPP_INCLUDED

#include <cstddef>
#include <cassert>
#include <cstdlib>

#include "minimgapi_raw_base.hpp"
#include "gets.hpp"

namespace minimg_raw {


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swAssurePrototypeIsValidRaw,       "AssurePrototypeIsValidRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsZeroSizeRaw,        "AssureImageIsZeroSizeRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsValidOnMemoryRaw,   "AssureImageIsValidOnMemoryRaw");
DECLARE_MINSTOPWATCH(swAssureOverrunPossibleRaw,        "AssureOverrunPossibleRaw");
DECLARE_MINSTOPWATCH(swAssureMinImgInvariantsAreMetRaw, "AssureMinImgInvariantsAreMetRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsNotAllocatedRaw,    "AssureImageIsNotAllocatedRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsAllocatedRaw,       "AssureImageIsAllocatedRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsSolidRaw,           "AssureImageIsSolidRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsScalarRaw,          "AssureImageIsScalarRaw");
DECLARE_MINSTOPWATCH(swAssureImageIsPixelRaw,           "AssureImageIsPixelRaw");
DECLARE_MINSTOPWATCH(swComparePrototypesRaw,            "ComparePrototypesRaw");
DECLARE_MINSTOPWATCH(swCompare2DSizesRaw,               "Compare2DSizesRaw");
DECLARE_MINSTOPWATCH(swCompare3DSizesRaw,               "Compare3DSizesRaw");
DECLARE_MINSTOPWATCH(swComparePixelsRaw,                "ComparePixelsRaw");
#endif


template<CheckType> struct NegativeResult;
template<> struct NegativeResult<CT_PRECONDITION> {
  static const MinResult value = MR_CONTRACT_VIOLATION;
};
template<> struct NegativeResult<CT_INFO> {
  static const MinResult value = MR_NO;
};
template<> struct NegativeResult<CT_POSTCONDITION> {
  static const MinResult value = MR_INTERNAL_ERROR;
};


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssurePrototypeIsValidRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssurePrototypeIsValidRaw);

  return 0 > image.width || 0 > image.height || 0 > image.channels ||
      TYP_INVALID == image.scalar_type ? NegativeResult<check_type>::value
                                        : MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssureImageIsZeroSizeRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsZeroSizeRaw);

  return image.width && image.height && image.channels
      ? NegativeResult<check_type>::value
      : MR_MRR;
}


static MINIMGAPI_RAW_API MinResult AssureImageIsValidOnMemoryRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsValidOnMemoryRaw);

  const int32_t bits_per_pixel = GetBitsPerPixelRaw(image);
  const int32_t bytes_per_line = (image.width * bits_per_pixel + 7) >> 3;
  const ptrdiff_t abs_stride = std::abs(image.stride);
  /// TODO: Check stride or not if height <= 1?
  if (/*1 < image.height && */abs_stride < bytes_per_line)
    return MR_CONTRACT_VIOLATION;

  if (!image.p_alloc_info)
//    return MR_CONTRACT_VIOLATION;
    return MR_MRR;
  const MinImgAllocInfo& alloc_info = *image.p_alloc_info;
  if (abs_stride != alloc_info.stride_)
    return MR_CONTRACT_VIOLATION;

  const uint8_t* const land_begin = alloc_info.p_land_;
  if (!land_begin)
    return MR_CONTRACT_VIOLATION;
  const ptrdiff_t delta_mem_ptr = GetImageMemoryPtrRaw(image) - land_begin;

  if (!abs_stride) {
    MR_PROPAGATE_ERROR(AssureImageIsZeroSizeRaw<CT_PRECONDITION>(image));
    return delta_mem_ptr ? MR_CONTRACT_VIOLATION : MR_MRR;
  }

  const ptrdiff_t y_lft_top = delta_mem_ptr / abs_stride;
  const ptrdiff_t byte_x_lft_top = delta_mem_ptr % abs_stride;
  if ((byte_x_lft_top << 3) % bits_per_pixel)
    return MR_CONTRACT_VIOLATION;
  if (y_lft_top < 0 ||
      y_lft_top + image.height > alloc_info.height_ ||
      byte_x_lft_top < 0 ||
      byte_x_lft_top + bytes_per_line > abs_stride)
    return MR_CONTRACT_VIOLATION;

  return MR_MRR;
}


static MINIMGAPI_RAW_API MinResult AssureOverrunPossibleRaw(
    const MinImg& image,
    int32_t       dl,
    int32_t       dt,
    int32_t       dr,
    int32_t       db) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureOverrunPossibleRaw);

  if (!image.p_alloc_info)
    return MR_MRR;
  int32_t dl_limit, dt_limit, dr_limit, db_limit;
  MR_SHOULD_WORK(GetDeltaRectLimitRaw(
      dl_limit, dt_limit, dr_limit, db_limit, image));
  if (dl < dl_limit || dt < dt_limit || dr > dr_limit || db > db_limit)
    return MR_CONTRACT_VIOLATION;
  return dl < dl_limit || dt < dt_limit || dr > dr_limit || db > db_limit
      ? MR_CONTRACT_VIOLATION : MR_MRR;
}


static MINIMGAPI_RAW_API MinResult AssureMinImgInvariantsAreMetRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureMinImgInvariantsAreMetRaw);

  if (image.p_zero_line) {
    MR_PROPAGATE_ERROR(AssurePrototypeIsValidRaw<CT_PRECONDITION>(image));
    MR_PROPAGATE_ERROR(AssureImageIsValidOnMemoryRaw(image));
  } else {
    if (image.p_alloc_info || image.is_owner)
      return MR_CONTRACT_VIOLATION;
  }
  return MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssureImageIsNotAllocatedRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsNotAllocatedRaw);

  return image.p_zero_line ? NegativeResult<check_type>::value : MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssureImageIsAllocatedRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsAllocatedRaw);

  return image.p_zero_line ? MR_MRR : NegativeResult<check_type>::value;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssureImageIsSolidRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsSolidRaw);

  /// TODO: What about height == 1?
  /// NOTE: Usually used as "need/can unroll?", so maybe that's a right check
  return image.height > 1 &&
      image.stride * 8 != image.width * GetBitsPerPixelRaw(image)
      ? NegativeResult<check_type>::value : MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssureImageIsScalarRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsScalarRaw);

  return image.width == 1 && image.height == 1 && image.channels == 1
      ? MR_MRR : NegativeResult<check_type>::value;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult AssureImageIsPixelRaw(
    const MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAssureImageIsPixelRaw);

  return image.width == 1 && image.height == 1
      ? MR_MRR : NegativeResult<check_type>::value;
}


///// TODO: Better name
//template<CheckType check_type, typename... Ts>
//static MINIMGAPI_RAW_API MinResult AssureImageFitsRaw(
//    const MinImg& image,
//    Ts...         args) {
//  /// FIXME: Finish
//  return MR_NOT_IMPLEMENTED;
//}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult ComparePrototypesRaw(
    const MinImg& image_a,
    const MinImg& image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swComparePrototypesRaw);

  return image_a.width != image_b.width ||
      image_a.height != image_b.height ||
      image_a.channels != image_b.channels ||
      image_a.scalar_type != image_b.scalar_type
      ? NegativeResult<check_type>::value
      : MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult Compare2DSizesRaw(
    const MinImg& image_a,
    const MinImg& image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swCompare2DSizesRaw);

  return image_a.width != image_b.width ||
      image_a.height != image_b.height
      ? NegativeResult<check_type>::value
      : MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult Compare3DSizesRaw(
    const MinImg& image_a,
    const MinImg& image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swCompare3DSizesRaw);

  return image_a.width != image_b.width ||
      image_a.height != image_b.height ||
      image_a.channels != image_b.channels
      ? NegativeResult<check_type>::value
      : MR_MRR;
}


template<CheckType check_type>
static MINIMGAPI_RAW_API MinResult ComparePixelsRaw(
    const MinImg& image_a,
    const MinImg& image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swComparePixelsRaw);

  return image_a.channels != image_b.channels ||
      image_a.scalar_type != image_b.scalar_type
      ? NegativeResult<check_type>::value
      : MR_MRR;
}


} // namespace minimg::raw

#endif // #ifndef MINIMGAPI_RAW_ASSURES_AND_COMPARES_HPP_INCLUDED
