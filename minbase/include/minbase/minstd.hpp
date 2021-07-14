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
#ifndef MINBASE_MINSTD_HPP_INCLUDED
#define MINBASE_MINSTD_HPP_INCLUDED

#include <minbase/crossplat.h>


#if MIN_HAVE_CPLUSPLUS


# if defined(__GNUC__) && !defined(__clang__)
#   if defined(ANDROID) || defined(PLATFORM_ELBRUS)
#include <string>
#include <sstream>
#   endif // defined(ANDROID) || defined(PLATFORM_ELBRUS)
#   if defined(ANDROID) || defined(PLATFORM_ELBRUS) || ((__GNUC__ == 4) && (__GNUC_MINOR <= 8))
#include <stdexcept>
#   endif // defined(ANDROID) || defined(PLATFORM_ELBRUS) || ((__GNUC__ == 4) && (__GNUC_MINOR <= 8))
#endif // defined(__GNUC__) && !defined(__clang__)


# if MIN_HAVE_CPLUSPLUS_11

# include <type_traits>

# else // MIN_HAVE_CPLUSPLUS_11

namespace std {


template<class T, T v>
struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant type;
  operator value_type() const { return value; }
};


typedef integral_constant<bool, true> true_type;

typedef integral_constant<bool, false> false_type;


template<class T, class U>
struct is_same : std::false_type {};

template<class T>
struct is_same<T, T> : std::true_type {};


template<bool cond, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };


template<bool B, class T, class F>
struct conditional { typedef T type; };

template<class T, class F>
struct conditional<false, T, F> { typedef F type; };


} // namespace std

# endif // MIN_HAVE_CPLUSPLUS_11 #else


# if defined(__GNUC__) && !defined(__clang__)

#   if defined(ANDROID) || defined(PLATFORM_ELBRUS)

namespace std {

template <typename T>
std::string to_string(T value) {
  std::ostringstream os;
  os << value;
  return os.str();
}

static double stod(const std::string& str) {
  std::istringstream istr(str);
  double ret = 0.0;
  istr >> ret;
  return ret;
}

} // namespace std

#   endif // defined(ANDROID) || defined(PLATFORM_ELBRUS)

#   if defined(ANDROID)

namespace std {

static int stoi(const std::string& str) {
  std::istringstream istr(str);
  int ret = 0;
  istr >> ret;
  return ret;
}

static unsigned long stoul(const std::string& str) {
  std::istringstream istr(str);
  unsigned long ret = 0;
  istr >> ret;
  return ret;
}

template<typename T>
T cbrt(T value) {
  return std::pow(value, 1.0 / 3.0);
}

} // namespace std

#   endif // defined(ANDROID)

#   if defined(ANDROID) || ((__GNUC__ == 4) && (__GNUC_MINOR <= 8))

namespace std {

static std::ios_base& hexfloat(std::ios_base& base) {
  throw std::runtime_error(
      "minstd impl of std::hexfloat on android is not supported");
}

} // namespace std

#   endif // defined(ANDROID) || ((__GNUC__ == 4) && (__GNUC_MINOR <= 8))

#   if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR <= 8))

namespace std {

static inline void* align(
    std::size_t alignment,
    std::size_t size,
    void*& ptr,
    std::size_t& space) {
  void* dst = reinterpret_cast<void*>((reinterpret_cast<ptrdiff_t>(ptr) + alignment - 1) /
    alignment * alignment);
  ptrdiff_t shift = reinterpret_cast<ptrdiff_t>(dst) - reinterpret_cast<ptrdiff_t>(ptr);
  if (space < shift + size)
    return nullptr;
  space -= shift;
  ptr = dst;
  return ptr;
}

} // namespace std

#   endif // (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR <= 8))

# endif // defined(__GNUC__) && !defined(__clang__)


#endif // MIN_HAVE_CPLUSPLUS

#endif // #ifndef MINBASE_MINSTD_HPP_INCLUDED
