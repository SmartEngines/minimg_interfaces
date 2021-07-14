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
#ifndef MINSBASE_MIN_PP_MINTYP_H_INCLUDED
#define MINSBASE_MIN_PP_MINTYP_H_INCLUDED


#include "minpreprocessor.h"


/// TODO: #ifndef's ?
/// TODO: 'ctype' or 'type'?

#define MIN_PP_MAP_ctype_MinTyp_uint1_t       TYP_UINT1
#define MIN_PP_MAP_ctype_MinFmt_uint1_t       FMT_UINT
#define MIN_PP_MAP_ctype_logbitsize_uint1_t   0

#define MIN_PP_MAP_ctype_MinTyp_uint8_t       TYP_UINT8
#define MIN_PP_MAP_ctype_MinFmt_uint8_t       FMT_UINT
#define MIN_PP_MAP_ctype_logbitsize_uint8_t   3

#define MIN_PP_MAP_ctype_MinTyp_uint16_t      TYP_UINT16
#define MIN_PP_MAP_ctype_MinFmt_uint16_t      FMT_UINT
#define MIN_PP_MAP_ctype_logbitsize_uint16_t  4

#define MIN_PP_MAP_ctype_MinTyp_uint32_t      TYP_UINT32
#define MIN_PP_MAP_ctype_MinFmt_uint32_t      FMT_UINT
#define MIN_PP_MAP_ctype_logbitsize_uint32_t  5

#define MIN_PP_MAP_ctype_MinTyp_uint64_t      TYP_UINT64
#define MIN_PP_MAP_ctype_MinFmt_uint64_t      FMT_UINT
#define MIN_PP_MAP_ctype_logbitsize_uint64_t  6

#define MIN_PP_MAP_ctype_MinTyp_int8_t        TYP_INT8
#define MIN_PP_MAP_ctype_MinFmt_int8_t        FMT_INT
#define MIN_PP_MAP_ctype_logbitsize_int8_t    3

#define MIN_PP_MAP_ctype_MinTyp_int16_t       TYP_INT16
#define MIN_PP_MAP_ctype_MinFmt_int16_t       FMT_INT
#define MIN_PP_MAP_ctype_logbitsize_int16_t   4

#define MIN_PP_MAP_ctype_MinTyp_int32_t       TYP_INT32
#define MIN_PP_MAP_ctype_MinFmt_int32_t       FMT_INT
#define MIN_PP_MAP_ctype_logbitsize_int32_t   5

#define MIN_PP_MAP_ctype_MinTyp_int64_t       TYP_INT64
#define MIN_PP_MAP_ctype_MinFmt_int64_t       FMT_INT
#define MIN_PP_MAP_ctype_logbitsize_int64_t   6

#define MIN_PP_MAP_ctype_MinTyp_real16_t      TYP_REAL16
#define MIN_PP_MAP_ctype_MinFmt_real16_t      FMT_REAL
#define MIN_PP_MAP_ctype_logbitsize_real16_t  4

#define MIN_PP_MAP_ctype_MinTyp_real32_t      TYP_REAL32
#define MIN_PP_MAP_ctype_MinFmt_real32_t      FMT_REAL
#define MIN_PP_MAP_ctype_logbitsize_real32_t  5

#define MIN_PP_MAP_ctype_MinTyp_real64_t      TYP_REAL64
#define MIN_PP_MAP_ctype_MinFmt_real64_t      FMT_REAL
#define MIN_PP_MAP_ctype_logbitsize_real64_t  6


#define MIN_PP_MAP_ctype_MinTyp(ctype)  \
  MIN_PP_CONCAT(MIN_PP_MAP_ctype_MinTyp_, ctype)
#define MIN_PP_MAP_ctype_MinFmt(ctype)  \
  MIN_PP_CONCAT(MIN_PP_MAP_ctype_MinFmt_, ctype)
#define MIN_PP_MAP_ctype_logbitsize(ctype)  \
  MIN_PP_CONCAT(MIN_PP_MAP_ctype_logbitsize_,  ctype)


/// The order of types in this macro must match the order of MinTyp values.
/// This correspondence is verified by TestMinbaseMinTyp_OrderCorrespondence
#ifndef MIN_PP_DO_FOR_ALL_TYPES_EX
# define MIN_PP_DO_FOR_ALL_TYPES_EX(MACRO, SEP) \
  MACRO(uint1_t)  SEP \
  MACRO(uint8_t)  SEP \
  MACRO(uint16_t) SEP \
  MACRO(uint32_t) SEP \
  MACRO(uint64_t) SEP \
  MACRO(int8_t)   SEP \
  MACRO(int16_t)  SEP \
  MACRO(int32_t)  SEP \
  MACRO(int64_t)  SEP \
  MACRO(real16_t) SEP \
  MACRO(real32_t) SEP \
  MACRO(real64_t)
#endif


#ifndef MIN_PP_DO_FOR_ALL_TYPES
# define MIN_PP_DO_FOR_ALL_TYPES(MACRO) MIN_PP_DO_FOR_ALL_TYPES_EX(MACRO, MIN_PP_ARG_NIL)
#endif




/// Alternative way to map tokens
/// Allows neater map "filling", setting several mappings in one line,
/// but doesnt work (in current form, for some reason) in some applications,
/// particularly in test.

/// TODO: #ifndef's?
#define MIN_PP_TRAIT_type_uint1_t  TYP_UINT1,  FMT_UINT, 0
#define MIN_PP_TRAIT_type_uint8_t  TYP_UINT8,  FMT_UINT, 3
#define MIN_PP_TRAIT_type_uint16_t TYP_UINT16, FMT_UINT, 4
#define MIN_PP_TRAIT_type_uint32_t TYP_UINT32, FMT_UINT, 5
#define MIN_PP_TRAIT_type_uint64_t TYP_UINT64, FMT_UINT, 6
#define MIN_PP_TRAIT_type_int8_t   TYP_INT8,   FMT_INT,  3
#define MIN_PP_TRAIT_type_int16_t  TYP_INT16,  FMT_INT,  4
#define MIN_PP_TRAIT_type_int32_t  TYP_INT32,  FMT_INT,  5
#define MIN_PP_TRAIT_type_int64_t  TYP_INT64,  FMT_REAL, 6
#define MIN_PP_TRAIT_type_real16_t TYP_REAL16, FMT_REAL, 4
#define MIN_PP_TRAIT_type_real32_t TYP_REAL32, FMT_REAL, 5
#define MIN_PP_TRAIT_type_real64_t TYP_REAL64, FMT_REAL, 6


#define MIN_PP_TRAIT_type_MinTyp(MinTyp, MinFmt, logbitsize) MinTyp
#define MIN_PP_TRAIT_type_MinFmt(MinTyp, MinFmt, logbitsize) MinFmt
#define MIN_PP_TRAIT_type_logbitsize(MinTyp, MinFmt, logbitsize) logbitsize


#endif // #ifndef MINSBASE_MIN_PP_MINTYP_H_INCLUDED
