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
#ifndef MINBASE_WARNINGS_H_INCLUDED
#define MINBASE_WARNINGS_H_INCLUDED


#include "minpreprocessor.h"


/// TODO: Some ifndefs ?
#if defined(_MSC_VER)
# define MIN_WARNINGS_SUPPRESSION_BEGIN MIN_PP_PRAGMA(warning(push, 0))
# define MIN_WARNINGS_SUPPRESSION_END MIN_PP_PRAGMA(warning(pop))
#elif defined(__clang__)
# define MIN_WARNINGS_SUPPRESSION_BEGIN                  \
  MIN_PP_PRAGMA(clang diagnostic push)             \
  MIN_PP_PRAGMA(clang diagnostic ignored "-Wall")  \
  MIN_PP_PRAGMA(clang diagnostic ignored "-Wextra")
# define MIN_WARNINGS_SUPPRESSION_END MIN_PP_PRAGMA(clang diagnostic pop)
#elif defined(__GNUC__)
# include "gcc_versions_warnings/gcc_warnings.h"
# define MIN_WARNINGS_SUPPRESSION_BEGIN    \
  MIN_PP_PRAGMA(GCC diagnostic push) \
  MIN_PP_WRN_DISABLE_GCC_WARNINGS
# define MIN_WARNINGS_SUPPRESSION_END MIN_PP_PRAGMA(GCC diagnostic pop)
/// TODO: #else some error?
#endif


#ifndef MIN_SUPPRESS_UNUSED_VARIABLE
# define MIN_SUPPRESS_UNUSED_VARIABLE(x) (void)(x)
#endif


#endif // #ifndef MINBASE_WARNINGS_H_INCLUDED
