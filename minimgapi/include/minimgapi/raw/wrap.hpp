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
#ifndef MINIMGAPI_RAW_WRAP_INCLUDED
#define MINIMGAPI_RAW_WRAP_INCLUDED

#include <new>

#include "minimgapi_raw_base.hpp"
#include "gets.hpp"

namespace minimg_raw {


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swWrapAlignedBufferRaw,    "WrapAlignedBufferRaw");
DECLARE_MINSTOPWATCH(swWrapAlignedBufferExRaw,  "WrapAlignedBufferExRaw");
DECLARE_MINSTOPWATCH(swWrapSolidBufferRaw,      "WrapSolidBufferRaw");
DECLARE_MINSTOPWATCH(swWrapPixelRaw,            "WrapPixelRaw");
DECLARE_MINSTOPWATCH(swWrapScalarRaw,           "WrapScalarRaw");
DECLARE_MINSTOPWATCH(swWrapPixelVectorRaw,      "WrapPixelVectorRaw");
DECLARE_MINSTOPWATCH(swWrapScalarVectorRaw,     "WrapScalarVectorRaw");
#endif


template<typename T>
static MINIMGAPI_RAW_API MinResult WrapAlignedBufferRaw(
    MinImg& image,
    T*      p_buffer,
    int32_t width,
    int32_t height,
    int32_t channels,
    int32_t stride) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapAlignedBufferRaw);

//  MinImgAllocInfo* p_alloc_info = reinterpret_cast<MinImgAllocInfo*>(
//      alignedmalloc(sizeof(MinImgAllocInfo), alignof(MinImgAllocInfo)));
//  if (!p_alloc_info)
//    return MR_ENV_ERROR;
  image.scalar_type = mintyp_traits::mintyp<T>::value;
  image.width = width;
  image.height = height;
  image.channels = channels;
  image.stride = stride ? stride : GetBytesPerLineRaw(image);
  image.is_owner = false;
//  image.is_owner = true;
  image.p_zero_line = reinterpret_cast<uint8_t*>(p_buffer);
//  p_alloc_info->p_land_ = image.p_zero_line;
//  p_alloc_info->stride_ = std::abs(image.stride);
//  p_alloc_info->height_ = height;
//  p_alloc_info->address_space_ = 0;
  image.p_alloc_info = nullptr;
//  image.p_alloc_info = p_alloc_info;
  return MR_MRR;
}


static MINIMGAPI_RAW_API MinResult WrapAlignedBufferExRaw(
    MinImg&  image,
    uint8_t* p_buffer,
    int32_t  /*address_space*/,
    MinTyp   scalar_type,
    int32_t  width,
    int32_t  height,
    int32_t  channels,
    int32_t  stride,
    int32_t  /*dl_limit*/,
    int32_t  /*dt_limit*/,
    int32_t  /*db_limit*/) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapAlignedBufferExRaw);

//  MinImgAllocInfo* p_alloc_info = reinterpret_cast<MinImgAllocInfo*>(
//      alignedmalloc(sizeof(MinImgAllocInfo), alignof(MinImgAllocInfo)));
//  if (!p_alloc_info)
//    return MR_ENV_ERROR;
  image.scalar_type = scalar_type;
  image.width = width;
  image.height = height;
  image.channels = channels;
  const int32_t bits_per_pixel = GetBitsPerPixelRaw(image);
  image.stride = stride ? stride : bits_per_pixel * width >> 3;
  image.is_owner = false;
//  image.is_owner = true;
  image.p_zero_line = p_buffer;
//  p_alloc_info->p_land_ =
//      GetLineRaw<uint8_t>(image, dt_limit) + (dl_limit * bits_per_pixel >> 3);
//  p_alloc_info->stride_ = std::abs(image.stride);
//  p_alloc_info->height_ = image.height + db_limit - dt_limit;
//  p_alloc_info->address_space_ = address_space;
  image.p_alloc_info = nullptr;
//  image.p_alloc_info = p_alloc_info;
  return MR_MRR;
}


template<typename T>
static MINIMGAPI_RAW_API MinResult WrapSolidBufferRaw(
    MinImg& image,
    T*      p_buffer,
    int32_t width,
    int32_t height,
    int32_t channels) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapSolidBufferRaw);

  /// TODO: Sort out after decision about inlining
  return WrapAlignedBufferRaw(image, p_buffer, width, height, channels, 0);
}


template<typename T>
static MINIMGAPI_RAW_API MinResult WrapPixelRaw(
    MinImg& image,
    T*      p_pixel,
    int32_t channels) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapPixelRaw);

  /// TODO: Sort out after decision about inlining
  return WrapSolidBufferRaw(image, p_pixel, 1, 1, channels);
}


template<typename T>
static MINIMGAPI_RAW_API MinResult WrapScalarRaw(
    MinImg&     image,
    T*          p_scalar) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapScalarRaw);

  /// TODO: Sort out after decision about inlining
  return WrapSolidBufferRaw(image, p_scalar, 1, 1, 1);
}


template<typename T>
static MINIMGAPI_RAW_API MinResult WrapPixelVectorRaw(
    MinImg&         image,
    T*              p_vector,
    int32_t         size,
    DirectionOption direction,
    int32_t         channels) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapPixelVectorRaw);

  /// TODO: Sort out after decision about inlining
  return WrapSolidBufferRaw(image, p_vector,
      direction == DO_VERTICAL ? 1 : size, direction == DO_VERTICAL ? size : 1,
      channels);
}


template<typename T>
static MINIMGAPI_RAW_API MinResult WrapScalarVectorRaw(
    MinImg&         image,
    T*              p_vector,
    int32_t         size,
    DirectionOption direction) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swWrapScalarVectorRaw);

  /// TODO: Sort out after decision about inlining
  return WrapSolidBufferRaw(image, p_vector,
      direction == DO_VERTICAL ? 1 : size, direction == DO_VERTICAL ? size : 1,
      1);
}


} // namespace minimg::raw

#endif // #ifndef MINIMGAPI_RAW_WRAP_INCLUDED
