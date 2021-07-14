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
#ifndef MINIMGAPI_SRC_VECTOR_COPY_CHANNELS_INL_H_INCLUDED
#define MINIMGAPI_SRC_VECTOR_COPY_CHANNELS_INL_H_INCLUDED

#include <minutils/smartptr.h>
#include <minbase/crossplat.h>

template<typename T> static MUSTINLINE void vector_deinterleave_4to3(
    T       *p_dst,
    const T *p_src,
    int      len) {
  const T *ps = p_src;
  T *pd = p_dst;
  for (int i = 0; i < len; ++i, ps += 4, pd += 3) {
    pd[0] = ps[0];
    pd[1] = ps[1];
    pd[2] = ps[2];
  }
}

#if defined(USE_SSE_SIMD)
#include "sse/copy_channels-inl.h"
#elif defined(USE_NEON_SIMD)
#include "neon/copy_channels-inl.h"
#endif

#endif // #ifndef MINIMGAPI_SRC_VECTOR_COPY_CHANNELS_INL_H_INCLUDED
