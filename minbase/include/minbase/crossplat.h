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
 * @file   crossplat.h
 * @brief  Different cross-platform declarations.
*/

#pragma once
#ifndef MINSBASE_CROSSPLAT_H_INCLUDED
#define MINSBASE_CROSSPLAT_H_INCLUDED


/**
 * @defgroup MinBase_Crossplat Cross-platform Declarations
 * @brief    The module contains macros and functions which provide
 *           cross-platform compiling of the library.
 */


#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>


#if defined(_MSC_VER) && defined(MINBASE_EXPORTS)
# define MINBASE_API __declspec(dllexport)
#elif defined(__clang__) || defined(__GNUC__)
# define MINBASE_API  \
  __attribute__ ((visibility ("default")))  \
  __attribute__ ((noinline))
#else
# define MINBASE_API
#endif


#if !defined(MIN_HAVE_CPLUSPLUS) && !defined(MIN_HAVE_CPLUSPLUS_11)
# ifdef __cplusplus
#   define MIN_HAVE_CPLUSPLUS 1
#   if __cplusplus >= 201100L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#     define MIN_HAVE_CPLUSPLUS_11 1
#   else
#     define MIN_HAVE_CPLUSPLUS_11 0
#   endif
# else
#   define MIN_HAVE_CPLUSPLUS 0
#   define MIN_HAVE_CPLUSPLUS_11 0
# endif
#endif


/**
 * @def     MUSTINLINE
 * @brief   Specifiers a cross-platform instruct the compiler to insert a copy
 *          of the function body into each place the function is called
 * @ingroup MinBase_Crossplat
 */
#ifndef NO_INLINE
# ifndef MUSTINLINE
#   ifdef _MSC_VER
#     define MUSTINLINE inline __forceinline
#   else
#     define MUSTINLINE inline __attribute__((always_inline))
#   endif
# endif
#else
# ifndef MUSTINLINE
#   define MUSTINLINE inline
# endif
#endif // NO_INLINE


/**
 * @def     STATIC_SPECIAL
 * @brief   On MSVC a specialization of template function does not inherit
 *          storage class. So one has to set storage class manually which is
 *          forbidden by C++ Standard and is not compilable on g++.
 * @ingroup MinBase_Crossplat
 */
#ifndef STATIC_SPECIAL
# if !defined(__clang__) && (defined(_MSC_VER) || \
      (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 3)))
#   define STATIC_SPECIAL static
# else
#   define STATIC_SPECIAL
# endif
#endif // STATIC_SPECIAL


/**
 * @def     IS_BY_DEFAULT(a)
 * @brief   Specifies a default value for a parameter of a function.
 * @ingroup MinBase_Crossplat
 */
#ifdef __cplusplus
# define IS_BY_DEFAULT(a) = a
#else
# define IS_BY_DEFAULT(a)
#endif

/**
 * @def     alignedmalloc(size, alignment)
 * @brief   Specifies cross-platform aligned allocation.
 * @ingroup MinBase_Crossplat
 */

/**
 * @def     alignedfree(ptr)
 * @brief   Specifies cross-platform aligned deallocations.
 * @ingroup MinBase_Crossplat
 */
#if MIN_HAVE_CPLUSPLUS
extern "C" {
#endif
static MUSTINLINE void* min_aligned_malloc(size_t size, size_t alignment) {
  void* p_buf = malloc(size + alignment);
  if (!p_buf)
    return NULL;
  void* p_aligned_buf =
      (void*)(((ptrdiff_t)p_buf + alignment) & ~(alignment - 1));
  *((uint8_t*)p_aligned_buf - 1) =
      (uint8_t)((ptrdiff_t)p_aligned_buf - (ptrdiff_t)p_buf);
  return p_aligned_buf;
}

static MUSTINLINE void min_aligned_free(void* p_aligned_buf) {
  if (p_aligned_buf)
    free((uint8_t*)p_aligned_buf - *((uint8_t*)p_aligned_buf - 1));
}
#if MIN_HAVE_CPLUSPLUS
}
#endif

#if defined(__MINGW32__)
# include <malloc.h>
# define alignedmalloc(size, alignment) __mingw_aligned_malloc(size, alignment)
# define alignedfree(ptr)               __mingw_aligned_free(ptr)
#elif defined(USE_SSE_SIMD)
# include <emmintrin.h>
# define alignedmalloc(size, alignment) _mm_malloc(size, alignment)
# define alignedfree(ptr)               _mm_free(ptr)
#else
# include <stdlib.h>
# define alignedmalloc(size, alignment) min_aligned_malloc(size, alignment)
# define alignedfree(ptr)               min_aligned_free(ptr)
#endif



#if defined(_MSC_VER)
# define MIN_ALIGNED(N) __declspec(align(N))
#else
# define MIN_ALIGNED(N) __attribute__((aligned(N)))
#endif


#if defined(__GNUC__)
# define MIN_HAS_BUILTIN_PREFETCH
#elif defined(__clang__)
# if defined(__has_builtin) && __has_builtin(__builtin_prefetch)
#   define MIN_HAS_BUILTIN_PREFETCH
# endif
#endif

#ifdef MIN_HAS_BUILTIN_PREFETCH
# define MIN_PREFETCH(ADDR, MODE) __builtin_prefetch(ADDR, MODE)
#else
# define MIN_PREFETCH(ADDR, MODE)
#endif


#if (defined _MSC_VER) && (_MSC_VER < 1900)
# define snprintf _snprintf
#endif


#ifdef _MSC_VER
# define MIN_FUNCNAME_MACRO __FUNCSIG__
#else // Assume GCC-compatible compiler here.
# define MIN_FUNCNAME_MACRO __PRETTY_FUNCTION__
#endif


#endif // #ifndef MINSBASE_CROSSPLAT_H_INCLUDED
