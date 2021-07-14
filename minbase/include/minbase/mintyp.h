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
 * @file   mintyp.h
 * @brief  Definition of acceptable image types.
 */

#pragma once

#ifndef MINSBASE_MINTYP_H_INCLUDED
#define MINSBASE_MINTYP_H_INCLUDED


#include "crossplat.h"
#include <stdint.h>

/**
 * @brief   Specifies pack of 8 1-bit fields.
 * @details The struct @c uint1_t represents pack of 8 1-bit fields
 *          incompatible with uint8_t.
 * @ingroup MinUtils_MinImg
 */
typedef struct uint1_t {
  uint8_t b0 : 1;
  uint8_t b1 : 1;
  uint8_t b2 : 1;
  uint8_t b3 : 1;
  uint8_t b4 : 1;
  uint8_t b5 : 1;
  uint8_t b6 : 1;
  uint8_t b7 : 1;
} uint1_t;


/**
 * @brief   Specifies half-precision floating point.
 * @details The struct @c real16 represents half-precision floating point.
 * @ingroup MinUtils_MinImg
 */
typedef struct real16_t
{
  uint16_t significand : 10;  ///< The mantissa of the number.
  uint16_t exponent    : 5;   ///< The magnitude of the number.
  uint16_t sign        : 1;   ///< The sign of the number.
} real16_t;


typedef float   real32_t;  ///<  Specifies @c float as @c real32_t type.
typedef double  real64_t;  ///<  Specifies @c double as @c real64_t type.


/**
 * @brief   Specifies acceptable element formats of each individual channel.
 * @details The enum specifies acceptable element formats of each
 *          individual channel.
 * @ingroup MinUtils_MinImg
 */
typedef enum MinFmt {
  FMT_INVALID = -1, ///< Value representing error while figuring out format,
                    ///  or, currently, to unspecified requirements on format.

  FMT_UINT,         ///< Unsigned integer.
  FMT_INT,          ///< Signed integer.
  FMT_REAL,         ///< Floating point.

  MINFMT_COUNT      ///< Count of non-invalid values of @c #MinFmt enum.
} MinFmt;


/**
 * @brief   Specifies acceptable element types of each individual channel.
 * @details The enum specifies acceptable element types (that is format + size)
 *          of each individual channel.
 * @ingroup MinUtils_MinImg
 */
typedef enum MinTyp {
  TYP_INVALID = -1, ///< Value representing error while figuring out type,
                    ///  or, currently, to unspecified requirements on type.

  TYP_UINT1,        ///< 1-bit logical.
  TYP_UINT8,        ///< Unsigned 8-bit integer.
  TYP_UINT16,       ///< Unsigned 16-bit integer.
  TYP_UINT32,       ///< Unsigned 32-bit integer.
  TYP_UINT64,       ///< Unsigned 64-bit integer.

  TYP_INT8,         ///< Signed 8-bit integer.
  TYP_INT16,        ///< Signed 16-bit integer.
  TYP_INT32,        ///< Signed 32-bit integer.
  TYP_INT64,        ///< Signeincompatible with uint8_td 64-bit integer.

  TYP_REAL16,       ///< Half-precision floating point.
  TYP_REAL32,       ///< Single-precision floating point.
  TYP_REAL64,       ///< Double-precision floating point.

  MINTYP_COUNT      ///< Count of non-invalid values of @c #MinTyp enum.
} MinTyp;

#ifdef __cplusplus
extern "C" {
#endif

MINBASE_API MinFmt MinFormatOfMinType(
    MinTyp typ);


MINBASE_API int32_t LogBitSizeOfMinType(
    MinTyp typ);


MINBASE_API int32_t ByteSizeOfMinType(
    MinTyp typ);


MINBASE_API MinTyp MinTypeByMinFormatAndLogSize(
    MinFmt format,
    int32_t   size_logbits);

#ifdef __cplusplus
} // extern "C"
#endif


#if MIN_HAVE_CPLUSPLUS

#include "min_pp_mintyp.h"

namespace mintyp_traits {


template<typename> struct mintyp;
#define MIN_PP_ARG_SPEC(type) template<> struct mintyp<type> {  \
  static const MinTyp value = MIN_PP_MAP_ctype_MinTyp(type);  \
};
MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_SPEC)
#undef MIN_PP_ARG_SPEC


template<typename> struct minfmt;
#define MIN_PP_ARG_SPEC(type) template<> struct minfmt<type> {  \
  static const MinFmt value = MIN_PP_MAP_ctype_MinFmt(type);  \
};
MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_SPEC)
#undef MIN_PP_ARG_SPEC


template<typename> struct logbitdepth;
#define MIN_PP_ARG_SPEC(type) template<> struct logbitdepth<type> {  \
  static const int32_t value = MIN_PP_MAP_ctype_logbitsize(type);  \
};
MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_SPEC)
#undef MIN_PP_ARG_SPEC


} // namespace mintyp_traits

#endif // MIN_HAVE_CPLUSPLUS

#endif // #ifndef MINSBASE_MINTYP_H_INCLUDED
