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
#ifndef MINIMGAPI_WRAP_HPP_INCLUDED
#define MINIMGAPI_WRAP_HPP_INCLUDED

#include "minimgapi_base.hpp"

#include "assures_and_compares.hpp"

#include "raw/wrap.hpp"

namespace minimg {


#ifdef MINIMGAPI_STOPWATCH_NORMAL_INTERFACE
DECLARE_MINSTOPWATCH(swWrapAlignedBuffer,    "WrapAlignedBuffer");
DECLARE_MINSTOPWATCH(swWrapSolidBuffer,      "WrapSolidBuffer");
DECLARE_MINSTOPWATCH(swWrapPixel,            "WrapPixel");
DECLARE_MINSTOPWATCH(swWrapScalar,           "WrapScalar");
DECLARE_MINSTOPWATCH(swWrapPixelVector,      "WrapPixelVector");
DECLARE_MINSTOPWATCH(swWrapScalarVector,     "WrapScalarVector");
#endif


template<typename T>
static MINIMGAPI_CPP_API MinResult WrapAlignedBuffer(
    MinImg &image,
    T      *p_buffer,
    int32_t width,
    int32_t height,
    int32_t channels,
    int32_t stride) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swWrapAlignedBuffer);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  if (!p_buffer || 0 > width || 0 > height || 0 > channels)
    return MR_CONTRACT_VIOLATION;
  if (stride &&
      std::abs(stride) < width * channels * static_cast<int32_t>(sizeof(T)))
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::WrapAlignedBufferRaw(
      image, p_buffer, width, height, channels, stride);
}


template<typename T>
static MINIMGAPI_CPP_API MinResult WrapSolidBuffer(
    MinImg &image,
    T      *p_buffer,
    int32_t width,
    int32_t height,
    int32_t channels) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swWrapSolidBuffer);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  if (!p_buffer || 0 > width || 0 > height || 0 > channels)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::WrapSolidBufferRaw(
      image, p_buffer, width, height, channels);
}


template<typename T>
static MINIMGAPI_CPP_API MinResult WrapPixel(
    MinImg &image,
    T      *p_pixel,
    int32_t channels) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swWrapPixel);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  if (!p_pixel || 0 > channels)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::WrapPixelRaw(image, p_pixel, channels);
}


template<typename T>
static MINIMGAPI_CPP_API MinResult WrapScalar(
    MinImg &image,
    T      *p_scalar) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swWrapScalar);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  if (!p_scalar)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::WrapScalarRaw(image, p_scalar);
}


template<typename T>
static MINIMGAPI_CPP_API MinResult WrapPixelVector(
    MinImg         &image,
    T              *p_vector,
    int32_t         size,
    DirectionOption direction,
    int32_t         channels) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swWrapPixelVector);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  if (!p_vector || 0 > size || DO_BOTH == direction || 0 > channels)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::WrapPixelVectorRaw(
      image, p_vector, size, direction, channels);
}


template<typename T>
static MINIMGAPI_CPP_API MinResult WrapScalarVector(
    MinImg         &image,
    T              *p_vector,
    int32_t         size,
    DirectionOption direction) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swWrapScalarVector);

  MR_PROPAGATE_ERROR(AssureImageIsNotAllocated<CT_PRECONDITION>(image));
  if (!p_vector || 0 > size || DO_BOTH == direction)
    return MR_CONTRACT_VIOLATION;

  return minimg_raw::WrapScalarVectorRaw(image, p_vector, size, direction);
}


} // namespace minimg

#endif // #ifndef MINIMGAPI_WRAP_HPP_INCLUDED
