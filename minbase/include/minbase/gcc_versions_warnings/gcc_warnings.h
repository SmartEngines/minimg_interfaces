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
#ifndef MINSBASE_GCC_VERSIONS_WARNINGS_GCC_WARNINGS_H_INCLUDED
#define MINSBASE_GCC_VERSIONS_WARNINGS_GCC_WARNINGS_H_INCLUDED

#ifdef __GNUC__

#include "../crossplat.h"
#include "../minpreprocessor.h"


#define MIN_PP_WRN_DISABLE_GCC_4_8_C_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "--all-warnings") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "--extra-warnings") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-W") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wabi") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Waddress") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Waggregate-return") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Waggressive-loop-optimizations") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wall") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Warray-bounds") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wattributes") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wbuiltin-macro-redefined") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcast-align") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcast-qual") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wchar-subscripts") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wclobbered") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcomment") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcomments") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wconversion") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wconversion-null") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcoverage-mismatch") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcpp") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdeprecated") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdisabled-optimization") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdiv-by-zero") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdouble-promotion") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wempty-body") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wendif-labels") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wenum-compare") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wextra") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wfloat-equal") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-contains-nul") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-extra-args") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-nonliteral") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-security") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-y2k") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-zero-length") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wframe-larger-than=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wfree-nonheap-object") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wignored-qualifiers") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winit-self") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winline") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wint-to-pointer-cast") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winvalid-memory-model") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winvalid-pch") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wlarger-than=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wlogical-op") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wlong-long") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmain") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmaybe-uninitialized") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-braces") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-declarations") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-field-initializers") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-include-dirs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmultichar") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnarrowing") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnonnull") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnormalized=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Woverflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Woverlength-strings") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpacked") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpacked-bitfield-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpadded") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wparentheses") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpedantic") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpointer-arith") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpragmas") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wproperty-assign-default") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wprotocol") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wrealloc-lhs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wrealloc-lhs-all") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpsabi") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wredundant-decls") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wreorder") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wreturn-local-addr") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wreturn-type") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wselector") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsequence-point") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsign-compare") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsign-conversion") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsign-promo") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsizeof-pointer-memaccess") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstack-protector") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstack-usage=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-aliasing") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-aliasing=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-null-sentinel") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-overflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-overflow=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-prototypes") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstrict-selector-match") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-attribute=const") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-attribute=format") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-attribute=noreturn") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-attribute=pure") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsurprising") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wswitch") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wswitch-default") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wswitch-enum") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsync-nand") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsynth") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsystem-headers") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtabs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtarget-lifetime") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtraditional") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtraditional-conversion") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtrampolines") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtrigraphs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtype-limits") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wundeclared-selector") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wundef") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunderflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wuninitialized") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunreachable-code") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunsafe-loop-optimizations") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunsuffixed-float-constants") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-but-set-parameter") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-but-set-variable") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-dummy-argument") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-function") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-label") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-local-typedefs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-macros") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-parameter") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-result") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-value") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-variable") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wuseless-cast") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wuse-without-only") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvarargs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvariadic-macros") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvector-operation-performance") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvirtual-move-assign") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvla") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvolatile-register-var") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wwrite-strings") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wzero-as-null-pointer-constant")

#define MIN_PP_WRN_DISABLE_GCC_4_8_C_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wbad-function-cast") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc++-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdeclaration-after-statement") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wimplicit") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wimplicit-function-declaration") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wimplicit-int") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wjump-misses-init") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-parameter-type") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-prototypes") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnested-externs") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wold-style-declaration") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wold-style-definition") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Woverride-init") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpointer-sign") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wassign-intercept") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpointer-to-int-cast")

#define MIN_PP_WRN_DISABLE_GCC_4_8_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wabi-tag") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc++0x-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc++11-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wctor-dtor-privacy") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdelete-non-virtual-dtor") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Weffc++") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winherited-variadic-ctor") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winvalid-offsetof") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wliteral-suffix") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnoexcept") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnon-template-friend") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnon-virtual-dtor") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wold-style-cast") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Woverloaded-virtual") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpmf-conversions")

#define MIN_PP_WRN_DISABLE_GCC_4_9_C_CXX_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_4_8_C_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wopenmp-simd") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdate-time") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wfloat-conversion")

#define MIN_PP_WRN_DISABLE_GCC_4_9_C_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_4_8_C_WARNINGS

#define MIN_PP_WRN_DISABLE_GCC_4_9_CXX_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_4_8_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wconditionally-supported") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdelete-incomplete")

#define MIN_PP_WRN_DISABLE_GCC_5_X_C_CXX_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_4_9_C_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wabi=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Warray-bounds=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wbool-compare") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wchkp") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-signedness") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wlogical-not-parentheses") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmemset-transposed-args") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnormalized") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wodr") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow-ivar") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshift-count-negative") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshift-count-overflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsized-deallocation") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsizeof-array-argument") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-final-methods") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-final-types") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-override") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wswitch-bool")

#define MIN_PP_WRN_DISABLE_GCC_5_X_C_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_4_9_C_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc90-c99-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc99-c11-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdesignated-init") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdiscarded-array-qualifiers") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdiscarded-qualifiers") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wincompatible-pointer-types") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wint-conversion")

#define MIN_PP_WRN_DISABLE_GCC_5_X_CXX_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_4_9_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc++14-compat")

#define MIN_PP_WRN_DISABLE_GCC_6_X_C_CXX_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_5_X_C_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wduplicated-cond") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wframe-address") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Whsa") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wignored-attributes") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wlto-type-mismatch") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmisleading-indentation") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnonnull-compare") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnull-dereference") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wscalar-storage-order") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshift-negative-value") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshift-overflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshift-overflow=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsubobject-linkage") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtautological-compare") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wtemplates") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wterminate") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-const-variable") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunused-const-variable=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvirtual-inheritance")

#define MIN_PP_WRN_DISABLE_GCC_6_X_C_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_5_X_C_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Woverride-init-side-effects") \

#define MIN_PP_WRN_DISABLE_GCC_6_X_CXX_WARNINGS  \
  MIN_PP_WRN_DISABLE_GCC_5_X_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmultiple-inheritance") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnamespaces") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wplacement-new")

#define MIN_PP_WRN_DISABLE_GCC_7_X_C_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_6_X_C_CXX_WARNINGS  \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Walloc-size-larger-than=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Walloc-zero") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Walloca") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Walloca-larger-than=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wbool-operation") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wbuiltin-declaration-mismatch") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdangling-else") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wduplicated-branches") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wexpansion-to-defined") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-overflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-overflow=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-truncation") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wformat-truncation=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wimplicit-fallthrough") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wimplicit-fallthrough=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wint-in-bool-context") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmemset-elt-size") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wnoexcept-type") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpointer-compare") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wrestrict") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow-compatible-local") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow-local") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow=compatible-local") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow=global") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wshadow=local") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstringop-overflow") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstringop-overflow=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wswitch-unreachable") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wvla-larger-than=")

#define MIN_PP_WRN_DISABLE_GCC_7_X_C_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_6_X_C_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wduplicate-decl-specifier")

#define MIN_PP_WRN_DISABLE_GCC_7_X_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_6_X_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Waligned-new") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Waligned-new=") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc++17-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wc++1z-compat") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wregister")

#define MIN_PP_WRN_DISABLE_GCC_8_X_C_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_7_X_C_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wattribute-alias") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcast-align=strict") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcast-function-type") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wif-not-aligned") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmissing-attributes") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wmultistatement-macros") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpacked-not-aligned") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsizeof-pointer-div") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wstringop-truncation") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-attribute=cold") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wsuggest-attribute=malloc")

#define MIN_PP_WRN_DISABLE_GCC_8_X_C_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_7_X_C_WARNINGS

#define MIN_PP_WRN_DISABLE_GCC_8_X_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_7_X_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wcatch-value") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wclass-memaccess") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wextra-semi")

#define MIN_PP_WRN_DISABLE_GCC_9_X_C_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_8_X_C_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Waddress-of-packed-member") \

#define MIN_PP_WRN_DISABLE_GCC_9_X_C_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_8_X_C_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wabsolute-value") \

#define MIN_PP_WRN_DISABLE_GCC_9_X_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_8_X_CXX_WARNINGS \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wdeprecated-copy") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Winit-list-lifetime") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wredundant-move") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wpessimizing-move") \
  MIN_PP_PRAGMA(GCC diagnostic ignored "-Wclass-conversion") \


#define MIN_PP_WRN_LATEST_SPECIFIED_GNUC 9
#define MIN_PP_WRN_LATEST_SPECIFIED_GNUC_MINOR X


#define MIN_PP_WRN_DISABLE_GCC_DEFAULT_C_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_9_X_C_CXX_WARNINGS

#define MIN_PP_WRN_DISABLE_GCC_DEFAULT_C_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_9_X_C_WARNINGS

#define MIN_PP_WRN_DISABLE_GCC_DEFAULT_CXX_WARNINGS \
  MIN_PP_WRN_DISABLE_GCC_9_X_CXX_WARNINGS


#ifndef MIN_PP_WRN_DISABLE_GCC_WARNINGS
# if __GNUC__ >= 5
#   if __GNUC__ > MIN_PP_WRN_LATEST_SPECIFIED_GNUC
#     define MIN_PP_WRN_DISABLE_GCC_WARNINGS \
        MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas")  \
        MIN_PP_CONCAT(MIN_PP_CONCAT(  \
          MIN_PP_WRN_DISABLE_GCC_, DEFAULT), _C_CXX_WARNINGS) \
        MIN_PP_CONCAT(MIN_PP_CONCAT(  \
          MIN_PP_WRN_DISABLE_GCC_, DEFAULT), \
          MIN_PP_IF(MIN_HAVE_CPLUSPLUS, _CXX_WARNINGS, _C_WARNINGS))
#   else
#     define MIN_PP_WRN_DISABLE_GCC_WARNINGS \
        MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas")  \
        MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_WRN_DISABLE_GCC_,  \
          MIN_PP_CONCAT(__GNUC__, _X)), _C_CXX_WARNINGS) \
        MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_WRN_DISABLE_GCC_,  \
          MIN_PP_CONCAT(__GNUC__, _X)), \
          MIN_PP_IF(MIN_HAVE_CPLUSPLUS, _CXX_WARNINGS, _C_WARNINGS))
#   endif
# else
#   define MIN_PP_WRN_DISABLE_GCC_WARNINGS \
      MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas")  \
      MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_WRN_DISABLE_GCC_, \
        MIN_PP_CONCAT(__GNUC__, \
          MIN_PP_CONCAT(_, __GNUC_MINOR__))), _C_CXX_WARNINGS) \
      MIN_PP_CONCAT(MIN_PP_CONCAT(MIN_PP_WRN_DISABLE_GCC_, \
        MIN_PP_CONCAT(__GNUC__, MIN_PP_CONCAT(_, __GNUC_MINOR__))), \
        MIN_PP_IF(MIN_HAVE_CPLUSPLUS, _CXX_WARNINGS, _C_WARNINGS))
# endif
#endif // #ifndef MIN_PP_WRN_DISABLE_GCC_WARNINGS


/*
//#if __GNUC__ >= 5
//# define LOCAL_GNUC_TOKEN MIN_PP_CONCAT(__GNUC__, _X)
//#else
//# define LOCAL_GNUC_TOKEN MIN_PP_CONCAT(__GNUC__, CONCAT_TOKENS(_, __GNUC_MINOR__))
//#endif

//#define LOCAL_DISABLE_PREFIX MIN_PP_CONCAT(MIN_PP_WRN_DISABLE_GCC_, LOCAL_GNUC_TOKEN)

//#ifndef MIN_PP_WRN_DISABLE_GCC_WARNINGS
//# ifdef __cplusplus
//#   define MIN_PP_WRN_DISABLE_GCC_WARNINGS \
//        MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas") \
//        MIN_PP_CONCAT(LOCAL_DISABLE_PREFIX, _C_CXX_WARNINGS) \
//        MIN_PP_CONCAT(LOCAL_DISABLE_PREFIX, _CXX_WARNINGS)
//# else
//#   define MIN_PP_WRN_DISABLE_GCC_WARNINGS \
//        MIN_PP_PRAGMA(GCC diagnostic ignored "-Wunknown-pragmas") \
//        MIN_PP_CONCAT(LOCAL_DISABLE_PREFIX, _C_CXX_WARNINGS) \
//        MIN_PP_CONCAT(LOCAL_DISABLE_PREFIX, _C_WARNINGS)
//# endif
//#endif // #ifndef MIN_PP_WRN_DISABLE_GCC_WARNINGS
*/

#endif // #ifdef __GNUC__

#endif // #ifndef MINSBASE_GCC_VERSIONS_WARNINGS_GCC_WARNINGS_H_INCLUDED

