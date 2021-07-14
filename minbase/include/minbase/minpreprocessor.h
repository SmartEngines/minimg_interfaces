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
#ifndef MINSBASE_MINPREPROCESSOR_H_INCLUDED
#define MINSBASE_MINPREPROCESSOR_H_INCLUDED


#ifndef MIN_PPI_STRINGIFY
# define MIN_PPI_STRINGIFY(x) #x
#endif

#ifndef MIN_PP_STRINGIFY
# define MIN_PP_STRINGIFY(x) MIN_PPI_STRINGIFY(x)
#endif


#ifndef MIN_PPI_CONCAT
# define MIN_PPI_CONCAT(x, y) x ## y
#endif

#ifndef MIN_PP_CONCAT
# define MIN_PP_CONCAT(x, y) MIN_PPI_CONCAT(x, y)
#endif


#ifndef MIN_PP_PRAGMA
# if defined(_MSC_VER)
#   define MIN_PP_PRAGMA(x) __pragma(x)
# else
#   define MIN_PP_PRAGMA(x) _Pragma(MIN_PP_STRINGIFY(x))
# endif
#endif


#ifndef MIN_PPI_IF_1
# define MIN_PPI_IF_1(x, y) x
#endif

#ifndef MIN_PPI_IF_0
# define MIN_PPI_IF_0(x, y) y
#endif

#ifndef MIN_PP_IF
# define MIN_PP_IF(cond, x, y) MIN_PP_CONCAT(MIN_PPI_IF_, cond)(x, y)
#endif


#ifndef MIN_PP_APPLY
# define MIN_PP_APPLY(MACRO, arg) MACRO(arg)
#endif


#ifndef MIN_PP_ARG_NIL
# define MIN_PP_ARG_NIL
#endif


#ifndef MIN_PP_MAP
/// This works fine (e.g. with MIN_PP_mintyp.h):
// MinTyp typ = MIN_PP_MAP(type, MinTyp)(uint8_t);
/// But is it useful?
#define MIN_PP_MAP(from, to) \
  MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_MAP_, from), MIN_PP_CONCAT(_, to))
#endif


#ifndef MIN_PP_TRAIT
/// Alternative way to map tokens
/// Allows neater map "filling", setting several mappings in one line,
/// but doesnt work (in current form, for some reason) in some applications,
/// particularly in test.
# define MIN_PP_TRAIT(category, key, trait) MIN_PP_APPLY(  \
  MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_TRAIT_, category), MIN_PP_CONCAT(_, trait)), \
  MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_TRAIT_, category), MIN_PP_CONCAT(_, key)))
#endif


#endif // #ifndef MINSBASE_MINPREPROCESSOR_H_INCLUDED
