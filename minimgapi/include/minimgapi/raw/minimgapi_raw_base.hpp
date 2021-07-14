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
#ifndef MINIMGAPI_RAW_MINIMGAPI_RAW_BASE_HPP_INCLUDED
#define MINIMGAPI_RAW_MINIMGAPI_RAW_BASE_HPP_INCLUDED

#include <cstddef>

#include <minbase/crossplat.h>
#include <minbase/minresult.h>
#include <minbase/mintyp.h>
#include <minbase/minimg.h>
#include <minbase/minstd.hpp>

#ifdef MINSTOPWATCH_ENABLED
#include <minstopwatch/stopwatch.hpp>
#endif

#include "../minimgapi_types.h"

#pragma pack(push, 1)
struct MinImgAllocInfo {
  uint8_t*  p_land_;
  ptrdiff_t stride_;
  int32_t   height_;
  int32_t   address_space_;
};
#pragma pack(pop)


#define MINIMGAPI_RAW_API MUSTINLINE


#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
# define MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(sw) \
    DECLARE_MINSTOPWATCH_CTL(sw)
#else
# define MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(sw)
#endif


#endif // #ifndef MINIMGAPI_RAW_MINIMGAPI_RAW_BASE_HPP_INCLUDED
