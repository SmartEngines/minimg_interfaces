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
#ifndef MINIMGAPI_RAW_PROTOTYPE_HPP_INCLUDED
#define MINIMGAPI_RAW_PROTOTYPE_HPP_INCLUDED

#include "minimgapi_raw_base.hpp"
#include "allocation.hpp"

namespace minimg_raw {


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swSetupPrototypeRaw,           "SetupPrototypeRaw");
DECLARE_MINSTOPWATCH(swClonePrototypeRaw,           "ClonePrototypeRaw");
DECLARE_MINSTOPWATCH(swCloneChangedPrototypeRaw,    "CloneChangedPrototypeRaw");
DECLARE_MINSTOPWATCH(swCloneTransposedPrototypeRaw, "CloneTransposedPrototypeRaw");
DECLARE_MINSTOPWATCH(swCloneFHTPrototypeRaw,        "CloneFHTPrototypeRaw");
#endif


static MINIMGAPI_RAW_API MinResult SetupPrototypeRaw(
    MinImg&          image,
    int32_t          width,
    int32_t          height,
    int32_t          channels,
    MinTyp           scalar_type,
    AllocationOption allocation    = AO_PREALLOCATED,
    int32_t          address_space = 0) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swSetupPrototypeRaw);

  image.width = width;
  image.height = height;
  image.channels = channels;
  image.scalar_type = scalar_type;
  image.stride = 0;
  image.is_owner = false;
  image.p_zero_line = nullptr;
  image.p_alloc_info = nullptr;
  return AO_PREALLOCATED == allocation ? AllocImageRaw(image, 16, address_space)
                                       : MR_MRR;
}


static MINIMGAPI_RAW_API MinResult ClonePrototypeRaw(
    MinImg&          dst_image,
    const MinImg&    src_image,
    AllocationOption allocation) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swClonePrototypeRaw);

  if (&dst_image == &src_image)
    return MR_MRR;
  /// TODO: Sort out after decision about inlining
  return SetupPrototypeRaw(dst_image,
      src_image.width, src_image.height, src_image.channels,
      src_image.scalar_type, allocation,
      src_image.p_alloc_info ? src_image.p_alloc_info->address_space_ : 0);
}


//template<typename... Ts>
//static MINIMGAPI_RAW_API MinResult CloneChangedPrototypeRaw(
//    MinImg&          dst_image,
//    const MinImg&    src_image,
//    Ts...            args,
//    AllocationOption allocation) {
//  /// FIXME: Finish
//  return MR_NOT_IMPLEMENTED;
//}


static MINIMGAPI_RAW_API MinResult CloneTransposedPrototypeRaw(
    MinImg&          dst_image,
    const MinImg&    src_image,
    AllocationOption allocation) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swCloneTransposedPrototypeRaw);

  /// TODO: Rework via Changed
  /// TODO: Sort out after decision about inlining
  return SetupPrototypeRaw(dst_image,
      src_image.height, src_image.width, src_image.channels,
      src_image.scalar_type, allocation,
      src_image.p_alloc_info ? src_image.p_alloc_info->address_space_ : 0);
}


static MINIMGAPI_RAW_API MinResult CloneFHTPrototypeRaw(
    MinImg          &dst_hough_image,
    const MinImg    &src_image,
    MinTyp           scalar_type,
    AngleRangeOption angle_range,
    AllocationOption allocation) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swCloneFHTPrototypeRaw);

  const int32_t dst_wd = src_image.width + src_image.height;
  int32_t dst_ht = 0;

  switch (angle_range) {
  case ARO_315_0:   /// fallthrough
  case ARO_0_45:    /// fallthrough
  case ARO_CTR_VER:
    dst_ht = src_image.height;
    break;
  case ARO_45_90:   /// fallthrough
  case ARO_90_135:  /// fallthrough
  case ARO_CTR_HOR:
    dst_ht = src_image.width;
    break;
  case ARO_315_45:
    dst_ht = 2 * src_image.height - 1;
    break;
  case ARO_45_135:
    dst_ht = 2 * src_image.width - 1;
    break;
  case ARO_315_135:
    dst_ht = 2 * (src_image.width + src_image.height) - 3;
    break;
  default:
    break;
  }

  /// TODO: Use CloneChangedPrototype
  return SetupPrototypeRaw(dst_hough_image, dst_wd, dst_ht, src_image.channels,
      scalar_type, allocation,
      src_image.p_alloc_info ? src_image.p_alloc_info->address_space_ : 0);
}


} // namespace minimg::raw

#endif // #ifndef MINIMGAPI_RAW_PROTOTYPE_HPP_INCLUDED
