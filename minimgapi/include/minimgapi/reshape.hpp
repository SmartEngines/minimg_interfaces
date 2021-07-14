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
#ifndef MINIMGAPI_RESHAPE_HPP_INCLUDED
#define MINIMGAPI_RESHAPE_HPP_INCLUDED

#include "minimgapi_base.hpp"

#include "assures_and_compares.hpp"

#include "raw/reshape.hpp"

namespace minimg {


#ifdef MINIMGAPI_STOPWATCH_NORMAL_INTERFACE
DECLARE_MINSTOPWATCH(swUnfoldChannels,         "UnfoldChannels");
DECLARE_MINSTOPWATCH(swSliceVertically,        "SliceVertically");
DECLARE_MINSTOPWATCH(swUnrollSolidImage,       "UnrollSolidImage");
#endif


static MINIMGAPI_CPP_API MinResult UnfoldChannels(
    MinImg       &dst_image,
    const MinImg &src_image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swUnfoldChannels);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(dst_image));
  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(src_image));

  return minimg_raw::UnfoldChannelsRaw(dst_image, src_image);
}


static MINIMGAPI_CPP_API MinResult SliceVertically(
    MinImg       &dst_image,
    const MinImg &src_image,
    int32_t       begin,
    int32_t       period,
    int32_t       end = -1) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swSliceVertically);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(dst_image));
  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(src_image));
  /// MAYBE: Is period < 0 ok?
  if (0 > begin || src_image.height <= begin || src_image.height < end ||
      0 >= period)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::SliceVerticallyRaw(dst_image, src_image, begin, period, end);
}


static MINIMGAPI_CPP_API MinResult UnrollSolidImage(
    MinImg       &dst_image,
    const MinImg &src_image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swUnrollSolidImage);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(dst_image));
  MR_PROPAGATE_ERROR(AssureImageIsSolid<CT_PRECONDITION>(src_image));

  return minimg_raw::UnrollSolidImageRaw(dst_image, src_image);
}


} // namespace minimg

#endif // #ifndef MINIMGAPI_RESHAPE_HPP_INCLUDED
