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
#ifndef MINIMGAPI_ASSURES_AND_COMPARES_HPP_INCLUDED
#define MINIMGAPI_ASSURES_AND_COMPARES_HPP_INCLUDED

#include "minimgapi_base.hpp"

#include "raw/assures_and_compares.hpp"

namespace minimg {


#ifdef MINIMGAPI_STOPWATCH_NORMAL_INTERFACE
DECLARE_MINSTOPWATCH(swAssurePrototypeIsValid,    "AssurePrototypeIsValid");
DECLARE_MINSTOPWATCH(swAssureImageIsZeroSize,     "AssureImageIsZeroSize");
DECLARE_MINSTOPWATCH(swAssureImageIsNotAllocated, "AssureImageIsNotAllocated");
DECLARE_MINSTOPWATCH(swAssureImageIsAllocated,    "AssureImageIsAllocated");
DECLARE_MINSTOPWATCH(swAssureImageIsSolid,        "AssureImageIsSolid");
DECLARE_MINSTOPWATCH(swAssureImageIsScalar,       "AssureImageIsScalar");
DECLARE_MINSTOPWATCH(swAssureImageIsPixel,        "AssureImageIsPixel");
DECLARE_MINSTOPWATCH(swComparePrototypes,         "ComparePrototypes");
DECLARE_MINSTOPWATCH(swCompare2DSizes,            "Compare2DSizes");
DECLARE_MINSTOPWATCH(swCompare3DSizes,            "Compare3DSizes");
DECLARE_MINSTOPWATCH(swComparePixels,             "ComparePixels");
#endif


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssurePrototypeIsValid(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssurePrototypeIsValid);

  return minimg_raw::AssurePrototypeIsValidRaw<check_type>(image);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssureImageIsZeroSize(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssureImageIsZeroSize);

  return minimg_raw::AssureImageIsZeroSizeRaw<check_type>(image);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssureImageIsNotAllocated(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssureImageIsNotAllocated);

  MR_PROPAGATE_ERROR(minimg_raw::AssureMinImgInvariantsAreMetRaw(image));
  return minimg_raw::AssureImageIsNotAllocatedRaw<check_type>(image);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssureImageIsAllocated(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssureImageIsAllocated);

  MR_PROPAGATE_ERROR(minimg_raw::AssureMinImgInvariantsAreMetRaw(image));
  return minimg_raw::AssureImageIsAllocatedRaw<check_type>(image);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssureImageIsSolid(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssureImageIsSolid);

  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(image));

  return minimg_raw::AssureImageIsSolidRaw<check_type>(image);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssureImageIsScalar(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssureImageIsScalar);

  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(image));

  return minimg_raw::AssureImageIsScalarRaw<check_type>(image);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult AssureImageIsPixel(
    const MinImg &image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swAssureImageIsPixel);

  MR_PROPAGATE_ERROR(AssureImageIsAllocated<CT_PRECONDITION>(image));

  return minimg_raw::AssureImageIsPixelRaw<check_type>(image);
}


///// TODO: Better name
//template<CheckType check_type, typename... Ts>
//static MINIMGAPI_CPP_API MinResult AssureImageFits(
//    const MinImg &image,
//    Ts...         args) {
//  /// FIXME: Finish
//  return minimg_raw::AssureImageFits<check_type>(image, args...);
//}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult ComparePrototypes(
    const MinImg &image_a,
    const MinImg &image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swComparePrototypes);

  return minimg_raw::ComparePrototypesRaw<check_type>(image_a, image_b);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult Compare2DSizes(
    const MinImg &image_a,
    const MinImg &image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swCompare2DSizes);

  return minimg_raw::Compare2DSizesRaw<check_type>(image_a, image_b);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult Compare3DSizes(
    const MinImg &image_a,
    const MinImg &image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swCompare3DSizes);

  return minimg_raw::Compare3DSizesRaw<check_type>(image_a, image_b);
}


template<CheckType check_type>
static MINIMGAPI_CPP_API MinResult ComparePixels(
    const MinImg &image_a,
    const MinImg &image_b) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_NORMAL_INTERFACE(swComparePixels);

  return minimg_raw::ComparePixelsRaw<check_type>(image_a, image_b);
}


} // namespace minimg

#endif // #ifndef MINIMGAPI_ASSURES_AND_COMPARES_HPP_INCLUDED
