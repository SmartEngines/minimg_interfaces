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
#ifndef MINIMGAPI_PROTOTYPE_HPP_INCLUDED
#define MINIMGAPI_PROTOTYPE_HPP_INCLUDED

#include "minimgapi_base.hpp"

#include "assures_and_compares.hpp"

#include "raw/prototype.hpp"

namespace minimg {


#ifdef MINIMGAPI_STOPWATCH_NORMAL_INTERFACE
DECLARE_MINSTOPWATCH(swSetupPrototype,           "SetupPrototype");
DECLARE_MINSTOPWATCH(swClonePrototype,           "ClonePrototype");
DECLARE_MINSTOPWATCH(swCloneChangedPrototype,    "CloneChangedPrototype");
DECLARE_MINSTOPWATCH(swCloneTransposedPrototype, "CloneTransposedPrototype");
DECLARE_MINSTOPWATCH(swCloneFHTPrototype,        "CloneFHTPrototype");
#endif


static MINIMGAPI_CPP_API MinResult SetupPrototype(
    MinImg          &image,
    int32_t          width,
    int32_t          height,
    int32_t          channels,
    MinTyp           scalar_type,
    AllocationOption allocation    = AO_PREALLOCATED,
    int32_t          address_space = 0) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swSetupPrototype);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  /// TODO: Allow to setup invalid prototype if not allocating or not?
  if (AO_PREALLOCATED == allocation &&
      (TYP_INVALID == scalar_type ||
       0 > width || 0 > height || 0 > channels))
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::SetupPrototypeRaw(
      image, width, height, channels, scalar_type, allocation, address_space);
}


static MINIMGAPI_CPP_API MinResult ClonePrototype(
    MinImg          &dst_image,
    const MinImg    &src_image,
    AllocationOption allocation) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swClonePrototype);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(dst_image));
  if (AO_PREALLOCATED == allocation) {
    MR_PROPAGATE_ERROR(AssurePrototypeIsValid<CT_PRECONDITION>(src_image));
  }

  return minimg_raw::ClonePrototypeRaw(dst_image, src_image, allocation);
}


//template<typename... Ts>
//static MINIMGAPI_CPP_API MinResult CloneChangedPrototype(
//    MinImg          &dst_image,
//    const MinImg    &src_image,
//    Ts...            args,
//    AllocationOption allocation) {
//  /// FIXME: Finish

//  return minimg_raw::CloneChangedPrototype(
//      dst_image, src_image, args..., allocation);
//}


static MINIMGAPI_CPP_API MinResult CloneTransposedPrototype(
    MinImg          &dst_image,
    const MinImg    &src_image,
    AllocationOption allocation) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swCloneTransposedPrototype);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(dst_image));
  if (AO_PREALLOCATED == allocation) {
    MR_PROPAGATE_ERROR(AssurePrototypeIsValid<CT_PRECONDITION>(src_image));
  }

  return minimg_raw::CloneTransposedPrototypeRaw(
      dst_image, src_image, allocation);
}


static MINIMGAPI_CPP_API MinResult CloneFHTPrototype(
    MinImg          &dst_hough_image,
    const MinImg    &src_image,
    MinTyp           scalar_type,
    AngleRangeOption angle_range,
    AllocationOption allocation) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swCloneFHTPrototype);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(
      dst_hough_image));
  MR_PROPAGATE_ERROR(AssurePrototypeIsValid<CT_PRECONDITION>(src_image));
  if (TYP_INVALID == scalar_type)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::CloneFHTPrototypeRaw(
      dst_hough_image, src_image, scalar_type, angle_range, allocation);
}


} // namespace minimg

#endif // #ifndef MINIMGAPI_PROTOTYPE_HPP_INCLUDED
