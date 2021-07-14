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
#ifndef MINIMGAPI_RAW_ALLOCATION_HPP_INCLUDED
#define MINIMGAPI_RAW_ALLOCATION_HPP_INCLUDED

#include <cstring>
#include <cassert>
#include <new>
#include <algorithm>

#include "minimgapi_raw_base.hpp"
#include "gets.hpp"
#include "assures_and_compares.hpp"

namespace minimg_raw {


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swAllocImageRaw, "AllocImageRaw");
DECLARE_MINSTOPWATCH(swFreeImageRaw,  "FreeImageRaw");
#endif


static MINIMGAPI_RAW_API MinResult AllocImageRaw(
    MinImg& image,
    int32_t alignment     = 16,
    int32_t address_space = 0) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swAllocImageRaw);

  if (0 != address_space)
    return MR_NOT_IMPLEMENTED;

  const int32_t line_size = GetBytesPerLineRaw(image);
  assert(line_size >= 0);
  const int32_t almost_alignment = alignment - 1;
  const int32_t alignment_mask = ~almost_alignment;
  if (!image.stride)
    image.stride = (line_size + almost_alignment) & alignment_mask;
  const ptrdiff_t abs_stride = std::abs(image.stride);
  const size_t non_image_buf_size = sizeof(MinImgAllocInfo) + almost_alignment;
  const size_t buf_size = image.height * abs_stride + non_image_buf_size;

  const size_t buf_alignment =
      std::max<size_t>(alignment, alignof(MinImgAllocInfo));

  /// TODO: Recheck how alignment is processed
  void* p_buf = alignedmalloc(buf_size, buf_alignment);
//  void* p_buf = alignedmalloc(buf_size, alignof(MinImgLandInfo));
  if (!p_buf)
    return MR_ENV_ERROR;

  MinImgAllocInfo& alloc_info = *reinterpret_cast<MinImgAllocInfo*>(p_buf);
//  MinImgLand& land = *new (p_buf) MinImgLand;

  alloc_info.p_land_ = reinterpret_cast<uint8_t*>(p_buf) +
      (non_image_buf_size & alignment_mask);

  alloc_info.stride_ = abs_stride;
  alloc_info.height_ = image.height;
  alloc_info.address_space_ = address_space;

  image.is_owner = true;

  image.p_zero_line = image.stride < 0
      ? alloc_info.p_land_ + GetLastYRaw(image) * abs_stride
      : alloc_info.p_land_;

  image.p_alloc_info = &alloc_info;

  return MR_MRR;
}


static MINIMGAPI_RAW_API MinResult FreeImageRaw(MinImg& image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swFreeImageRaw);

  if (image.is_owner) {
    assert(image.p_zero_line && image.p_alloc_info);
    if (0 != image.p_alloc_info->address_space_)
      return MR_NOT_IMPLEMENTED;

    const void* p_buf = reinterpret_cast<const void*>(image.p_alloc_info);
    /// TODO: Isn't there any more elegant solution than const_cast here?
    alignedfree(const_cast<void*>(p_buf));
    image.is_owner = false;
    image.p_zero_line = nullptr;
    image.p_alloc_info = nullptr;
  }

  return MR_MRR;
}


} // namespace minimg::raw

#endif // #ifndef MINIMGAPI_RAW_ALLOCATION_HPP_INCLUDED
