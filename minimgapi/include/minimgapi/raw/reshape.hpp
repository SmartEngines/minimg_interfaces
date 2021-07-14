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
#ifndef MINIMGAPI_RAW_RESHAPE_HPP_INCLUDED
#define MINIMGAPI_RAW_RESHAPE_HPP_INCLUDED

#include <algorithm>

#include "minimgapi_raw_base.hpp"
#include "gets.hpp"
#include "assures_and_compares.hpp"
#include "wrap.hpp"

#include <minbase/minmeta.hpp>

namespace minimg_raw {


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swUnfoldChannelsRaw,         "UnfoldChannelsRaw");
DECLARE_MINSTOPWATCH(swSliceVerticallyRaw,        "SliceVerticallyRaw");
DECLARE_MINSTOPWATCH(swUnrollSolidImageRaw,       "UnrollSolidImageRaw");
DECLARE_MINSTOPWATCH(swUnrollImagesIfAllSolidRaw, "UnrollImagesIfAllSolidRaw");
#endif


static MINIMGAPI_RAW_API MinResult UnfoldChannelsRaw(
    MinImg&       dst_image,
    const MinImg& src_image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swUnfoldChannelsRaw);

  int32_t dl, dt, dr, db;
  MR_PROPAGATE_ERROR(GetDeltaRectLimitRaw(dl, dt, dr, db, src_image));
  MR_PROPAGATE_ERROR(WrapAlignedBufferExRaw(dst_image, src_image.p_zero_line,
      /*src_image.p_alloc_info->address_space_*/ 0, src_image.scalar_type,
      src_image.width * src_image.channels, src_image.height, 1,
      src_image.stride, dl * src_image.channels, dt, db));
  return MR_MRR;
}


static MINIMGAPI_RAW_API MinResult SliceVerticallyRaw(
    MinImg&       dst_image,
    const MinImg& src_image,
    int32_t       begin,
    int32_t       period,
    int32_t       end = -1) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swSliceVerticallyRaw);

  if (end < 0)
    end = src_image.height;
  int32_t dl, dt, dr, db;
  MR_PROPAGATE_ERROR(GetDeltaRectLimitRaw(dl, dt, dr, db, src_image));
  const int32_t dst_height = (end - begin + period - 1) / period;
  MR_PROPAGATE_ERROR(WrapAlignedBufferExRaw(
      dst_image, GetLineRaw<uint8_t>(src_image, begin),
      /*src_image.p_alloc_info->address_space_*/ 0, src_image.scalar_type,
      src_image.width, dst_height, src_image.channels,
      src_image.stride * period,
      dl, (dt - begin) / period, (src_image.height + db - end) / period));
  return MR_MRR;
}


static MINIMGAPI_RAW_API MinResult UnrollSolidImageRaw(
    MinImg&       dst_image,
    const MinImg& src_image) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swUnrollSolidImageRaw);

  MR_PROPAGATE_ERROR(WrapAlignedBufferRaw(dst_image, src_image.p_zero_line,
      src_image.width * src_image.height, 1, src_image.channels,
      src_image.stride * src_image.height));
  dst_image.scalar_type = src_image.scalar_type;
  return MR_MRR;
}


class UnrollImagesIfAllSolidHelper {
  struct OpAssure {
    static MUSTINLINE MinResult Call(const MinImg&, const MinImg& src) {
      return AssureImageIsSolidRaw<CT_INFO>(src);
    }
  };

  struct OpUnroll {
    static MUSTINLINE MinResult Call(MinImg& dst, const MinImg& src) {
      return UnrollSolidImageRaw(dst, src);
    }
  };

  struct OpNoUnroll {
    static MUSTINLINE MinResult Call(MinImg& dst, const MinImg& src) {
      return GetRegionRaw(dst, src, 0, 0, src.width, src.height);
    }
  };

  template<class Op>
  static MUSTINLINE MinResult Proc(MinImg& dst, const MinImg& src) {
    return Op::Call(dst, src);
  }

  template<class Op, typename... Ts>
  static MUSTINLINE MinResult Proc(
      MinImg&       dst0,
      const MinImg& src0,
      Ts&...        rest) {
    MinResult r0 = Proc<Op>(dst0, src0);
    return MR_MRR == r0 ? Proc<Op>(rest...) : r0;
  }

public:
  template<typename... Ts>
  static MUSTINLINE MinResult Call(Ts&... args) {
    static_assert(!(sizeof...(Ts) & 1),
        "This function takes even number of images");
    static_assert(
        std::is_same<
          minmeta::TypeList<MinImg, typename std::decay<Ts>::type...>,
          minmeta::TypeList<typename std::decay<Ts>::type..., MinImg> >::value,
        "All arguments should be of type MinImg");
    const MinResult assure_result = Proc<OpAssure>(args...);
    if (MR_MRR == assure_result) {
      return Proc<OpUnroll>(args...);
    } else {
      MinResult res = Proc<OpNoUnroll>(args...);
      return MR_MRR == res ? MR_NO : res;
    }
  }
};


template<typename... Ts>
static MINIMGAPI_RAW_API MinResult UnrollImagesIfAllSolidRaw(Ts&... args) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swUnrollImagesIfAllSolidRaw);

  return UnrollImagesIfAllSolidHelper::Call(args...);
}


} // namespace minimg::raw

#endif // #ifndef MINIMGAPI_RAW_RESHAPE_HPP_INCLUDED
