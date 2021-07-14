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
#ifndef MINIMGPRC_CONTRIB_H_INCLUDED
#define MINIMGPRC_CONTRIB_H_INCLUDED

#include <minbase/crossplat.h>
#include <minbase/minimg.h>

#include <minimgprc/minimgprc.h>


#ifdef __cplusplus
  extern "C" {
#endif

MINIMGPRC_API int LabelConnectedComponents(
    const MinImg *pDst,
    const MinImg *pSrc,
    uint32_t     *count);

MINIMGPRC_API int AffineTransformImage2(
  const MinImg *p_dst_image,
  const MinImg *p_src_image,
  double        dst_to_src_transform[2][3],
  BorderOption  bo,
  const void   *p_bg_pixel IS_BY_DEFAULT(NULL),
  double        max_space_miss IS_BY_DEFAULT(0.5));

#ifdef __cplusplus
  } // extern "C"
#endif

#endif // #ifndef MINIMGPRC_CONTRIB_H_INCLUDED
