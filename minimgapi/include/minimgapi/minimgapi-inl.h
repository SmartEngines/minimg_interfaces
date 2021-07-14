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

/**
 * @file   minimgapi-inl.h
 * @brief  MinImgAPI inlining interface.
 *
 * This header contains inline versions of some MinimgAPI functions.
 * See description of the functions in file minimgapi.h
 */

#pragma once
#ifndef MINIMGAPI_MINIMGAPI_INL_H_INCLUDED
#define MINIMGAPI_MINIMGAPI_INL_H_INCLUDED

#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <cmath>
#include <algorithm>

#include <minbase/minresult.h>
#include <minbase/crossplat.h>
#include <minbase/min_pp_mintyp.h>
#include <minimgapi/minimgapi.h>
#include <minimgapi/imgguard.hpp>

#include <minimgapi/minimgapi.hpp>

MUSTINLINE int _GetMinImageType(
    const MinImg *p_image) {
  if (!p_image)
    return BAD_ARGS;
  return p_image->scalar_type;
}

MUSTINLINE int _SetMinImageType(
    MinImg *p_image,
    MinTyp  element_type) {
  if (!p_image || p_image->p_zero_line)
    return BAD_ARGS;

  p_image->scalar_type = element_type;

  return NO_ERRORS;
}

MUSTINLINE int _AssureMinImagePrototypeIsValid(
    const MinImg *p_image) {
  PROPAGATE_ERROR(_GetMinImageType(p_image));
  if (p_image->width < 0 || p_image->height < 0 || p_image->channels < 0)
    return BAD_ARGS;
  return NO_ERRORS;
}

MUSTINLINE int _GetMinImageBitsPerPixel(
    const MinImg *p_image) {
  PROPAGATE_ERROR(_AssureMinImagePrototypeIsValid(p_image));

  int32_t size_logbits = LogBitSizeOfMinType(p_image->scalar_type);
  PROPAGATE_ERROR(size_logbits);
  return p_image->channels * (1 << size_logbits);
}

MUSTINLINE int _GetMinImageBytesPerLine(
    const MinImg *p_image) {
  int bits_per_pixel = GetMinImageBitsPerPixel(p_image);
  PROPAGATE_ERROR(bits_per_pixel);
  return (p_image->width * bits_per_pixel + 7) >> 3;
}

MUSTINLINE int _AssureMinImageIsValid(
    const MinImg *p_image) {
  PROPAGATE_ERROR(_AssureMinImagePrototypeIsValid(p_image));
  if (!p_image->width || !p_image->height || !p_image->channels)
    return NO_ERRORS;
  if (!p_image->p_zero_line)
    return BAD_ARGS;
  if (p_image->height > 1 &&
      std::abs(p_image->stride) < _GetMinImageBytesPerLine(p_image))
    return BAD_ARGS;
  return NO_ERRORS;
}

MUSTINLINE int _AssureMinImageIsEmpty(
    const MinImg *p_image) {
  PROPAGATE_ERROR(_AssureMinImagePrototypeIsValid(p_image));
  return p_image->width && p_image->height && p_image->channels;
}

MUSTINLINE int _AssureMinImageIsSolid(
    const MinImg *p_image) {
  PROPAGATE_ERROR(_AssureMinImageIsValid(p_image));

  int bits_per_pixel = _GetMinImageBitsPerPixel(p_image);
  return p_image->height > 1 &&
         p_image->stride * 8 != p_image->width * bits_per_pixel;
}

MUSTINLINE int _AssureMinImageFits(
    const MinImg *p_image,
    MinTyp        element_type,
    int           channels = -1,
    int           width = -1,
    int           height = -1) {
  PROPAGATE_ERROR(_AssureMinImageIsValid(p_image));

  int retVal = 0;
  PROPAGATE_ERROR(retVal = _GetMinImageType(p_image));
  if (element_type >= 0 && element_type != retVal)
    return 1;
  if (channels >= 0 && channels != p_image->channels)
    return 1;
  if (width >= 0 && width != p_image->width)
    return 1;
  if (height >= 0 && height != p_image->height)
    return 1;

  return NO_ERRORS;
}

MUSTINLINE int _AssureMinImageIsScalar(
    const MinImg *p_image) {
  return _AssureMinImageFits(p_image, static_cast<MinTyp>(-1), 1, 1, 1);
}

MUSTINLINE int _AssureMinImageIsPixel(
    const MinImg *p_image) {
  return _AssureMinImageFits(p_image, static_cast<MinTyp>(-1), -1, 1, 1);
}

MUSTINLINE uint8_t *_GetMinImageLineUnsafe(
    const MinImg *p_image,
    int           y,
    BorderOption  border   = BO_VOID,
    void         *p_canvas = NULL) {
  const int ht = p_image->height;
  if (y < 0 || y >= ht)
    switch (border) {
      case BO_REPEAT: {
        y = std::min(std::max(0, y), ht - 1);
        break;
      }
      case BO_CYCLIC: {
        y = (y % ht + ht) % ht;
        break;
      }
      case BO_SYMMETRIC: {
        int ht2 = ht * 2;
        y = (y % ht2 + ht2) % ht2;
        y = std::min(y, ht2 - 1 - y);
        break;
      }
      case BO_CONSTANT: {
        return reinterpret_cast<uint8_t *>(p_canvas);
      }
      case BO_VOID: {
        return NULL;
      }
      default: {
      }
    }

  return p_image->p_zero_line + static_cast<ptrdiff_t>(p_image->stride) * y;
}

MUSTINLINE uint8_t *_GetMinImageLine(
    const MinImg *p_image,
    int           y,
    BorderOption  border   = BO_VOID,
    void         *p_canvas = NULL) {
  if (_AssureMinImageIsValid(p_image) != NO_ERRORS)
    return NULL;

  if (_AssureMinImageIsEmpty(p_image) == NO_ERRORS)
    switch (border) {
    case BO_CONSTANT:
      return reinterpret_cast<uint8_t *>(p_canvas);
    case BO_IGNORE:
      return p_image->p_zero_line ? p_image->p_zero_line + static_cast<ptrdiff_t>(p_image->stride) * y : NULL;
    default:
      return NULL;
    }
  return _GetMinImageLineUnsafe(p_image, y, border, p_canvas);
}

MUSTINLINE int _CloneMinImagePrototype(
    MinImg          *p_dst_image,
    const MinImg    *p_src_image,
    AllocationOption allocation = AO_PREALLOCATED) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  return minimg::ClonePrototype(*p_dst_image, *p_src_image, allocation);

//  if (!p_dst_image || !p_src_image || p_dst_image->p_zero_line)
//    return BAD_ARGS;

//  if (p_dst_image != p_src_image) {
//    *p_dst_image = *p_src_image;
//    p_dst_image->stride = 0;
//    p_dst_image->p_zero_line = 0;
////    p_dst_image->p_landlord = 0;
//  }
//  if (allocation == AO_PREALLOCATED)
//    PROPAGATE_ERROR(AllocMinImage(p_dst_image));

//  return NO_ERRORS;
}

MUSTINLINE int _CloneResizedMinImagePrototype(
    MinImg          *p_dst_image,
    const MinImg    *p_src_image,
    int              width,
    int              height,
    AllocationOption allocation = AO_PREALLOCATED) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  return minimg::SetupPrototype(*p_dst_image,
      width, height, p_src_image->channels, p_src_image->scalar_type,
      allocation,
      p_src_image->p_alloc_info ? p_src_image->p_alloc_info->address_space_ : 0);

//  if (width < 0 || height < 0)
//    return BAD_ARGS;

//  PROPAGATE_ERROR(_CloneMinImagePrototype(p_dst_image, p_src_image, AO_EMPTY));
//  p_dst_image->width = width;
//  p_dst_image->height = height;
//  if (allocation == AO_PREALLOCATED)
//    PROPAGATE_ERROR(AllocMinImage(p_dst_image));

//  return NO_ERRORS;
}

MUSTINLINE int _CloneTransposedMinImagePrototype(
    MinImg          *p_dst_image,
    const MinImg    *p_src_image,
    AllocationOption allocation = AO_PREALLOCATED) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  return minimg::CloneTransposedPrototype(
      *p_dst_image, *p_src_image, allocation);

//  PROPAGATE_ERROR(_CloneResizedMinImagePrototype(p_dst_image, p_src_image,
//                          p_src_image->height, p_src_image->width, allocation));

//  return NO_ERRORS;
}

MUSTINLINE int _CloneRetypifiedMinImagePrototype(
    MinImg          *p_dst_image,
    const MinImg    *p_src_image,
    MinTyp           type,
    AllocationOption allocation = AO_PREALLOCATED) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  return minimg::SetupPrototype(*p_dst_image,
      p_src_image->width, p_src_image->height, p_src_image->channels, type,
      allocation,
      p_src_image->p_alloc_info ? p_src_image->p_alloc_info->address_space_ : 0);

//  PROPAGATE_ERROR(_CloneMinImagePrototype(p_dst_image, p_src_image, AO_EMPTY));
//  PROPAGATE_ERROR(_SetMinImageType(p_dst_image, type));
//  if (allocation == AO_PREALLOCATED)
//    PROPAGATE_ERROR(AllocMinImage(p_dst_image));

//  return NO_ERRORS;
}

MUSTINLINE int _CloneDimensionedMinImagePrototype(
    MinImg          *p_dst_image,
    const MinImg    *p_src_image,
    int              channels,
    AllocationOption allocation = AO_PREALLOCATED) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  return minimg::SetupPrototype(*p_dst_image,
      p_src_image->width, p_src_image->height, channels,
      p_src_image->scalar_type, allocation,
      p_src_image->p_alloc_info ? p_src_image->p_alloc_info->address_space_ : 0);

//  if (channels < 0)
//    return BAD_ARGS;

//  PROPAGATE_ERROR(_CloneMinImagePrototype(p_dst_image, p_src_image, AO_EMPTY));
//  p_dst_image->channels = channels;
//  if (allocation == AO_PREALLOCATED)
//    PROPAGATE_ERROR(AllocMinImage(p_dst_image));

//  return NO_ERRORS;
}

MUSTINLINE int _CompareMinImagePrototypes(
    const MinImg *p_image_a,
    const MinImg *p_image_b) {
  MinImg prototype_a = {};
  PROPAGATE_ERROR(_CloneMinImagePrototype(&prototype_a, p_image_a, AO_EMPTY));
  MinImg prototype_b = {};
  PROPAGATE_ERROR(_CloneMinImagePrototype(&prototype_b, p_image_b, AO_EMPTY));
  return ::memcmp(&prototype_a, &prototype_b, sizeof(prototype_a)) != 0;
}

MUSTINLINE int _CompareMinImage2DSizes(
    const MinImg *p_image_a,
    const MinImg *p_image_b) {
  if (!p_image_a || !p_image_b)
    return BAD_ARGS;

  return p_image_a->width != p_image_b->width ||
         p_image_a->height != p_image_b->height;
}

MUSTINLINE int _CompareMinImage3DSizes(
    const MinImg *p_image_a,
    const MinImg *p_image_b) {
  if (!p_image_a || !p_image_b)
    return BAD_ARGS;

  return p_image_a->width != p_image_b->width ||
         p_image_a->height != p_image_b->height ||
         p_image_a->channels != p_image_b->channels;
}

MUSTINLINE int _CompareMinImageTypes(
    const MinImg *p_image_a,
    const MinImg *p_image_b) {
  if (!p_image_a || !p_image_b)
    return BAD_ARGS;

  return p_image_a->scalar_type != p_image_b->scalar_type;
}

MUSTINLINE int _CompareMinImagePixels(
    const MinImg *p_image_a,
    const MinImg *p_image_b) {
  if (!p_image_a || !p_image_b)
    return BAD_ARGS;

  return _CompareMinImageTypes(p_image_a, p_image_b) ||
         p_image_a->channels != p_image_b->channels;
}

MUSTINLINE int _CompareMinImages(
    const MinImg *p_image_a,
    const MinImg *p_image_b) {
  PROPAGATE_ERROR(_AssureMinImageIsValid(p_image_a));
  PROPAGATE_ERROR(_AssureMinImageIsValid(p_image_b));
  int proto_res = _CompareMinImagePrototypes(p_image_a, p_image_b);
  if (proto_res)
    return proto_res;
  int height = p_image_a->height;
  int byte_line_width = _GetMinImageBytesPerLine(p_image_a);
  for (int y = 0; y < height; ++y) {
    const uint8_t* p_line_a = minimg_raw::GetLineRaw<uint8_t>(*p_image_a, y);
    const uint8_t* p_line_b = minimg_raw::GetLineRaw<uint8_t>(*p_image_b, y);
    if (::memcmp(p_line_a, p_line_b, byte_line_width))
      return 1;
  }
  return NO_ERRORS;
}

MUSTINLINE int _WrapAlignedBufferWithMinImage(
    MinImg      *p_image,
    void        *p_buffer,
    int          width,
    int          height,
    int          channels,
    MinTyp       element_type,
    int          stride,
    RulesOption  rules = RO_STRICT) {
  if (!p_image)
    return BAD_ARGS;
  if (RO_REUSE_CONTAINER & rules) {
    PROPAGATE_ERROR(minimg::FreeImage(*p_image));
    *p_image = {};
  }
  switch (element_type) {
#define MIN_PP_ARG_CASE(type) case MIN_PP_MAP_ctype_MinTyp(type):  \
  return minimg::WrapAlignedBuffer(*p_image,  \
      reinterpret_cast<type*>(p_buffer), width, height, channels, stride);

  MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_CASE)

#undef MIN_PP_ARG_CASE
  default:
    return MR_INTERNAL_ERROR;
  }

//  if (!p_image || !p_buffer)
//    return BAD_ARGS;
//  if (width < 0 || height < 0 || channels < 0)
//    return BAD_ARGS;
//  if (!(rules & RO_REUSE_CONTAINER) && p_image->p_zero_line)
//    return BAD_ARGS;

//  // TODO: Add check of std::abs(stride);
//  // TODO: Add check of stride being >= width * channels (by abs value);

//  ::memset(p_image, 0, sizeof(*p_image));
//  PROPAGATE_ERROR(_SetMinImageType(p_image, element_type));
//  p_image->height = height;
//  p_image->width = width;
//  p_image->channels = channels;
//  p_image->stride = stride ? stride : _GetMinImageBytesPerLine(p_image);
//  p_image->p_zero_line = reinterpret_cast<uint8_t *>(p_buffer);

//  return NO_ERRORS;
}

MUSTINLINE int _WrapSolidBufferWithMinImage(
    MinImg      *p_image,
    void        *p_buffer,
    int          width,
    int          height,
    int          channels,
    MinTyp       element_type,
    RulesOption  rules = RO_STRICT) {
  return _WrapAlignedBufferWithMinImage(p_image, p_buffer, width, height,
                                        channels, element_type, 0, rules);
}

MUSTINLINE int _WrapScalarWithMinImage(
    MinImg      *p_image,
    void        *p_scalar,
    MinTyp       element_type,
    RulesOption  rules = RO_STRICT) {
  return _WrapSolidBufferWithMinImage(p_image, p_scalar, 1, 1, 1,
                                      element_type, rules);
}

MUSTINLINE int _WrapPixelWithMinImage(
    MinImg      *p_image,
    void        *p_pixel,
    int          channels,
    MinTyp       element_type,
    RulesOption  rules = RO_STRICT) {
  return _WrapSolidBufferWithMinImage(p_image, p_pixel, 1, 1,
                                      channels, element_type, rules);
}

MUSTINLINE int _WrapScalarVectorWithMinImage(
    MinImg          *p_image,
    void            *p_vector,
    int              size,
    DirectionOption  direction,
    MinTyp           element_type,
    RulesOption  rules = RO_STRICT) {
  if (direction == DO_BOTH)
    return BAD_ARGS;
  return _WrapSolidBufferWithMinImage(p_image, p_vector,
                                      direction == DO_VERTICAL ? 1 : size,
                                      direction == DO_VERTICAL ? size : 1,
                                      1, element_type, rules);
}

MUSTINLINE int _WrapPixelVectorWithMinImage(
    MinImg          *p_image,
    void            *p_vector,
    int              size,
    DirectionOption  direction,
    int              channels,
    MinTyp           element_type,
    RulesOption  rules = RO_STRICT) {
  if (direction == DO_BOTH)
    return BAD_ARGS;
  return _WrapSolidBufferWithMinImage(p_image, p_vector,
                                      direction == DO_VERTICAL ? 1 : size,
                                      direction == DO_VERTICAL ? size : 1,
                                      channels, element_type, rules);
}

MUSTINLINE int _GetMinImageRegion(
    MinImg       *p_dst_image,
    const MinImg *p_src_image,
    int           x0,
    int           y0,
    int           width,
    int           height,
    RulesOption   rules = RO_STRICT) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  if (p_dst_image == p_src_image) {
    if (RO_REUSE_CONTAINER & rules) {
//      DECLARE_GUARDED_MINIMG(tmp);
//      tmp = *p_src_image;
//      *p_dst_image = {};
//      return minimg::GetRegion(*p_dst_image, tmp, x0, y0, width, height);
      return minimg::ChangeRegion(*p_dst_image, x0, y0,
          width - p_src_image->width + x0, height - p_src_image->height + y0);
    } else {
      return BAD_ARGS;
    }
  }
  if (RO_REUSE_CONTAINER & rules) {
    PROPAGATE_ERROR(minimg::FreeImage(*p_dst_image));
    *p_dst_image = {};
  }
  return minimg::GetRegion(*p_dst_image, *p_src_image, x0, y0, width, height);

//  PROPAGATE_ERROR(_AssureMinImageIsValid(p_src_image));
//  if (!p_dst_image || width < 0 || height < 0)
//    return BAD_ARGS;
//  if (!(rules & RO_REUSE_CONTAINER) && p_dst_image->p_zero_line)
//    return BAD_ARGS;
//  if (!(rules & RO_IGNORE_BORDERS)) {
//    if (x0 < 0 || static_cast<uint32_t>(x0) + static_cast<uint32_t>(width) >
//        static_cast<uint32_t>(p_src_image->width))
//      return BAD_ARGS;
//    if (y0 < 0 || static_cast<uint32_t>(y0) + static_cast<uint32_t>(height) >
//        static_cast<uint32_t>(p_src_image->height))
//      return BAD_ARGS;
//  }

//  *p_dst_image = *p_src_image;
//  p_dst_image->width = width;
//  p_dst_image->height = height;

//  int bit_shift = x0 * _GetMinImageBitsPerPixel(p_dst_image);
//  if (bit_shift & 0x07U)
//    return BAD_ARGS;
//  p_dst_image->p_zero_line = _GetMinImageLine(p_src_image, y0, BO_IGNORE) +
//                        (bit_shift >> 3);
//  if (!p_dst_image->p_zero_line)
//    return INTERNAL_ERROR;
//  p_dst_image->stride = p_src_image->stride;

//  return NO_ERRORS;
}

MUSTINLINE int _FlipMinImageVertically(
    MinImg       *p_dst_image,
    const MinImg *p_src_image,
    RulesOption   rules = RO_STRICT) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  if (p_dst_image == p_src_image) {
    if (RO_REUSE_CONTAINER & rules) {
//      DECLARE_GUARDED_MINIMG(tmp);
//      tmp = *p_src_image;
//      *p_dst_image = {};
//      return minimg::GetRegion(
//          *p_dst_image, tmp, 0, 0, tmp.width, tmp.height, true);
      return minimg::ChangeRegion(*p_dst_image, 0, 0, 0, 0, true);
    } else {
      return BAD_ARGS;
    }
  }
  if (RO_REUSE_CONTAINER & rules) {
    PROPAGATE_ERROR(minimg::FreeImage(*p_dst_image));
    *p_dst_image = {};
  }
  return minimg::GetRegion(*p_dst_image, *p_src_image,
      0, 0, p_src_image->width, p_src_image->height, true);

//  if (!p_dst_image)
//    return BAD_ARGS;
//  if (!(rules & RO_REUSE_CONTAINER) && p_dst_image->p_zero_line)
//    return BAD_ARGS;
//  PROPAGATE_ERROR(_AssureMinImageIsValid(p_src_image));

//  *p_dst_image = *p_src_image;
//  uint8_t *p = _GetMinImageLine(p_dst_image, p_dst_image->height - 1);
//  if (!p)
//    return INTERNAL_ERROR;
//  p_dst_image->p_zero_line = p;
//  p_dst_image->stride *= -1;

//  return NO_ERRORS;
}

MUSTINLINE int _UnfoldMinImageChannels(
    MinImg       *p_dst_image,
    const MinImg *p_src_image,
    RulesOption   rules = RO_STRICT) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  if (p_dst_image == p_src_image) {
    if (RO_REUSE_CONTAINER & rules) {
      DECLARE_GUARDED_MINIMG(tmp);
      /*MinImg*/ tmp = *p_src_image;
      *p_dst_image = {};
      return minimg::UnfoldChannels(*p_dst_image, tmp);
    } else {
      return BAD_ARGS;
    }
  }
  if (RO_REUSE_CONTAINER & rules) {
    PROPAGATE_ERROR(minimg::FreeImage(*p_dst_image));
    *p_dst_image = {};
  }
  return minimg::UnfoldChannels(*p_dst_image, *p_src_image);

//  if (!p_dst_image)
//    return BAD_ARGS;
//  if (!(rules & RO_REUSE_CONTAINER) && p_dst_image->p_zero_line)
//    return BAD_ARGS;
//  PROPAGATE_ERROR(_AssureMinImageIsValid(p_src_image));

//  *p_dst_image = *p_src_image;
//  p_dst_image->width *= p_dst_image->channels;
//  p_dst_image->channels = 1;

//  return NO_ERRORS;
}

MUSTINLINE int _SliceMinImageVertically(
    MinImg       *p_dst_image,
    const MinImg *p_src_image,
    int           begin,
    int           period,
    int           end = -1,
    RulesOption   rules = RO_STRICT) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  if (p_dst_image == p_src_image) {
    if (RO_REUSE_CONTAINER & rules) {
      DECLARE_GUARDED_MINIMG(tmp);
      /*MinImg*/ tmp = *p_src_image;
      *p_dst_image = {};
      return minimg::SliceVertically(*p_dst_image, tmp, begin, period, end);
    } else {
      return BAD_ARGS;
    }
  }
  if (RO_REUSE_CONTAINER & rules) {
    PROPAGATE_ERROR(minimg::FreeImage(*p_dst_image));
    *p_dst_image = {};
  }
  return minimg::SliceVertically(
      *p_dst_image, *p_src_image, begin, period, end);

//  if (!p_dst_image || p_dst_image->p_zero_line)
//    return BAD_ARGS;
//  if (!(rules & RO_REUSE_CONTAINER) && p_dst_image->p_zero_line)
//    return BAD_ARGS;
//  PROPAGATE_ERROR(_AssureMinImageIsValid(p_src_image));
//  if (end < 0)
//    end = p_src_image->height;
//  if (begin < 0 || end < begin || p_src_image->height < end || period <= 0)
//    return BAD_ARGS;


//  *p_dst_image = *p_src_image;
//  p_dst_image->p_zero_line = _GetMinImageLine(p_dst_image, begin);
//  if (!p_dst_image->p_zero_line)
//    return INTERNAL_ERROR;
//  p_dst_image->stride *= period;
//  p_dst_image->height = (end - begin + period - 1) / period;

//  return NO_ERRORS;
}

MUSTINLINE int _UnrollSolidMinImage(
    MinImg       *p_dst_image,
    const MinImg *p_src_image,
    RulesOption   rules = RO_STRICT) {
  if (!p_dst_image || !p_src_image)
    return BAD_ARGS;
  if (p_dst_image == p_src_image) {
    if (RO_REUSE_CONTAINER & rules) {
      DECLARE_GUARDED_MINIMG(tmp);
      /*MinImg*/ tmp = *p_src_image;
      p_dst_image = {};
      return minimg::UnrollSolidImage(*p_dst_image, tmp);
    } else {
      return BAD_ARGS;
    }
  }
  if (RO_REUSE_CONTAINER & rules) {
    PROPAGATE_ERROR(minimg::FreeImage(*p_dst_image));
    *p_dst_image = {};
  }
  return minimg::UnrollSolidImage(*p_dst_image, *p_src_image);

//  if (!p_dst_image)
//    return BAD_ARGS;
//  if (!(rules & RO_REUSE_CONTAINER) && p_dst_image->p_zero_line)
//    return BAD_ARGS;
//  PROPAGATE_ERROR(_AssureMinImageIsValid(p_src_image));
//  if (_AssureMinImageIsSolid(p_src_image) != NO_ERRORS)
//    return BAD_ARGS;

//  *p_dst_image = *p_src_image;
//  p_dst_image->width *= p_dst_image->height;
//  p_dst_image->height = 1;

//  return NO_ERRORS;
}

MUSTINLINE int _SplitImageIntoRegions(
    int             n_regions,
    MinImg *const  *pp_regions,
    const MinImg   *p_image,
    DirectionOption split_direction) {
  NEED_NO_ERRORS(_AssureMinImageIsValid(p_image));

  const int32_t width = p_image->width;
  const int32_t height = p_image->height;

  switch (split_direction) {
  case DO_BOTH: {
    return BAD_ARGS;
  }
  case DO_HORIZONTAL: {
    const int32_t shift = (width + n_regions - 1) / n_regions;
    for (int k = 0; k < n_regions; ++k) {
      const int32_t x0 = k * shift;
      PROPAGATE_ERROR(_GetMinImageRegion(pp_regions[k], p_image,
          x0, 0, std::min(shift, width - x0), height));
    }
    break;
  }
  case DO_VERTICAL: {
    const int32_t shift = (height + n_regions - 1) / n_regions;
    for (int k = 0; k < n_regions; ++k) {
      const int32_t y0 = k * shift;
      PROPAGATE_ERROR(_GetMinImageRegion(pp_regions[k], p_image,
          0, y0, width, std::min(shift, height - y0)));
    }
    break;
  }
  default:
    return NOT_IMPLEMENTED;
  }

  return NO_ERRORS;
}

#endif // #ifndef MINIMGAPI_MINIMGAPI_INL_H_INCLUDED
