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
#ifndef MINIMGAPI_ALLOCATION_HPP_INCLUDED
#define MINIMGAPI_ALLOCATION_HPP_INCLUDED

#include "minimgapi_base.hpp"

#include "assures_and_compares.hpp"

#include "raw/allocation.hpp"

namespace minimg {


#ifdef MINIMGAPI_STOPWATCH_NORMAL_INTERFACE
DECLARE_MINSTOPWATCH(swAllocImage, "AllocImage");
DECLARE_MINSTOPWATCH(swFreeImage,  "FreeImage");
#endif


static MINIMGAPI_CPP_API MinResult AllocImage(
    MinImg &image,
    int32_t alignment     = 16,
    int32_t address_space = 0) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAllocImage);

  MR_PROPAGATE_ERROR(AssurePrototypeIsValid<CT_PRECONDITION>(image));
  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  const int32_t almost_alignment = alignment - 1;
  if (0 >= alignment || alignment & almost_alignment)
    return MR_CONTRACT_VIOLATION;
  if (image.stride) {
    const int32_t abs_stride = std::abs(image.stride);
    if (abs_stride < minimg_raw::GetBytesPerLineRaw(image))
      return MR_CONTRACT_VIOLATION;
    if (abs_stride & almost_alignment) /// abs_stide % alignment
      return MR_CONTRACT_VIOLATION;
  }

  return minimg_raw::AllocImageRaw(image, alignment, address_space);
}


static MINIMGAPI_CPP_API MinResult FreeImage(
    MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swFreeImage);

  if (image.is_owner)
    MR_PROPAGATE_ERROR(minimg_raw::AssureMinImgInvariantsAreMetRaw(image));

  return minimg_raw::FreeImageRaw(image);
}


} // namespace minimg

#endif // #ifndef MINIMGAPI_ALLOCATION_HPP_INCLUDED
