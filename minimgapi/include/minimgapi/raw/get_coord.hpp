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
#ifndef MINIMGAPI_RAW_GET_COORD_HPP_INCLUDED
#define MINIMGAPI_RAW_GET_COORD_HPP_INCLUDED

#include <cassert>

#include <minbase/warnings.h>

#include "minimgapi_raw_base.hpp"

namespace minimg_raw {

#ifdef MINIMGAPI_STOPWATCH_RAW_INTERFACE
DECLARE_MINSTOPWATCH(swGetCoordRaw, "GetCoordRaw");
#endif

enum CoordPrior {
  CP_NOWHERE        = 0,
  CP_BELOW_ZERO     = 1,
  CP_IN_RANGE       = 2,
  CP_ABOVE_SIZE     = 4,
  CP_ANYWHERE       = CP_BELOW_ZERO | CP_IN_RANGE | CP_ABOVE_SIZE,
  CP_NOT_BELOW_ZERO = CP_ANYWHERE ^ CP_BELOW_ZERO,
  CP_NOT_IN_RANGE   = CP_ANYWHERE ^ CP_IN_RANGE,
  CP_NOT_ABOVE_SIZE = CP_ANYWHERE ^ CP_ABOVE_SIZE
};


/// Class to hide helper stuff from namespace
class GetCoordHelper {
  /// Helper trait
  /// to shortcut evaluation for BOs that don't actually depend on borders
  template<BorderOption bo, CoordPrior pr> struct CorrectedCoordPrior {
    static const CoordPrior value = pr;
  };
  template<CoordPrior pr> struct CorrectedCoordPrior<BO_IGNORE, pr> {
    /// CP_NOWHERE is used as a dummy value here, as it has no other use for now
    static const CoordPrior value = CP_NOWHERE;
  };

  /// Conditions to select the overload with required checks
  /// The order of checks is fixed:
  /// if needed, check for below-zero; then if needed, check for above-size
  template<CoordPrior pr> using CheckBelow =
      typename std::enable_if<pr & CP_BELOW_ZERO &&
                              pr & CP_NOT_BELOW_ZERO, bool>::type;
  template<CoordPrior pr> using CheckAbove =
      typename std::enable_if<pr == CP_NOT_BELOW_ZERO, bool>::type;
  template<CoordPrior pr> using NoChecks =
      typename std::enable_if<!(pr & pr - 1), bool>::type;

  /// Implementation (overloads) of actual call
  /// Check below-zero
  template<BorderOption bo, CoordPrior pr, CheckBelow<pr> = true>
  static MUSTINLINE int32_t CheckAndGet(int32_t c, int32_t sz) {
    static const CoordPrior next_prior =
        static_cast<CoordPrior>(pr & CP_NOT_BELOW_ZERO);
    return c < 0 ? CheckAndGet<bo, CP_BELOW_ZERO>(c, sz)
                 : CheckAndGet<bo, next_prior>(c, sz);
  }
  /// Check above-size
  template<BorderOption bo, CoordPrior pr, CheckAbove<pr> = true>
  static MUSTINLINE int32_t CheckAndGet(int32_t c, int32_t sz) {
    return c >= sz ? CheckAndGet<bo, CP_ABOVE_SIZE>(c, sz)
                   : CheckAndGet<bo, CP_IN_RANGE>(c, sz);
  }
  /// No further decision-informing checks required, only input validation
  /// This function should be in the call path for all cases
  template<BorderOption bo, CoordPrior pr, NoChecks<pr> = true>
  static MUSTINLINE int32_t CheckAndGet(int32_t c, int32_t sz) {
    assert(sz >= 0);
    AssertValidInputForPrior<pr>(c, sz);
    return Get<bo, pr>(c, sz);
  }

  /// Guard against the case of user call with specified prior
  /// and incompatible runtime arguments
  /// This function is specialized outside the class for each prior
  template<CoordPrior pr>
  static MUSTINLINE void AssertValidInputForPrior(int32_t c, int32_t sz);

  /// All checks done, this function does the actual job
  /// It is specialized outside the class for specific BO and priors
  /// The general specialization here is used for CP_IN_RANGE and/or BO_IGNORE
  template<BorderOption bo, CoordPrior pr> static MUSTINLINE
  typename std::enable_if<!(pr & pr - 1), int32_t>::type
  Get(int32_t c, int32_t /*sz*/) {
    return c;
  }

public:
  /// Public Call checks (compile-time) for validnes of template arguments,
  /// then uses CorrectedCoordPrior helper trait to shortcut for BO_IGNORE
  template<BorderOption border, CoordPrior prior>
  static MUSTINLINE int32_t Call(int32_t c, int32_t sz) {
    static_assert(prior != CP_NOWHERE,
        "GetCoord can't get prior CP_NOWHERE");
    static_assert(border != BO_VOID && border != BO_CONSTANT,
        "GetCoord can't get border options "
        "not representable in same coord system"); /// FIXME: Wording
    return
        CheckAndGet<border, CorrectedCoordPrior<border, prior>::value>(c, sz);
  }
};


/// Specializations to do asserts that runtime arguments don't contradict prior
template<> STATIC_SPECIAL MUSTINLINE
void GetCoordHelper::AssertValidInputForPrior<CP_BELOW_ZERO>(
    int32_t c, int32_t /*sz*/) {
  MIN_SUPPRESS_UNUSED_VARIABLE(c);
  assert(c < 0);
}
template<> STATIC_SPECIAL MUSTINLINE
void GetCoordHelper::AssertValidInputForPrior<CP_IN_RANGE>(
    int32_t c, int32_t sz) {
  MIN_SUPPRESS_UNUSED_VARIABLE(c); MIN_SUPPRESS_UNUSED_VARIABLE(sz);
  assert(c >= 0 && c < sz);
}
template<> STATIC_SPECIAL MUSTINLINE
void GetCoordHelper::AssertValidInputForPrior<CP_ABOVE_SIZE>(
    int32_t c, int32_t sz) {
  MIN_SUPPRESS_UNUSED_VARIABLE(c); MIN_SUPPRESS_UNUSED_VARIABLE(sz);
  assert(c >= sz);
}
template<> STATIC_SPECIAL MUSTINLINE
void GetCoordHelper::AssertValidInputForPrior<CP_NOWHERE>(
    int32_t /*c*/, int32_t /*sz*/) {
}


/// Specializations to get specific evaluations
/// of coord below-zero or above-range, depending on BO
template<> STATIC_SPECIAL MUSTINLINE
int32_t GetCoordHelper::Get<BO_REPEAT, CP_BELOW_ZERO>(
    int32_t /*c*/, int32_t /*sz*/) {
  return 0;
}
template<> STATIC_SPECIAL MUSTINLINE
int32_t GetCoordHelper::Get<BO_REPEAT, CP_ABOVE_SIZE>(
    int32_t /*c*/, int32_t sz) {
  return sz - 1;
}
template<> STATIC_SPECIAL MUSTINLINE
int32_t GetCoordHelper::Get<BO_SYMMETRIC, CP_BELOW_ZERO>(
    int32_t c, int32_t sz) {
  const int32_t q = (c + 1) / sz;
  const int32_t r = c + 1 - sz * q; // (c + 1) % sz;
  return q & 1 ? sz + r - 1 : -r;
}
template<> STATIC_SPECIAL MUSTINLINE
int32_t GetCoordHelper::Get<BO_SYMMETRIC, CP_ABOVE_SIZE>(
    int32_t c, int32_t sz) {
  const int32_t q = c / sz;
  const int32_t r = c - sz * q; // c % sz;
  return q & 1 ? sz - r - 1 : r;
}
template<> STATIC_SPECIAL MUSTINLINE
int32_t GetCoordHelper::Get<BO_CYCLIC, CP_BELOW_ZERO>(int32_t c, int32_t sz) {
  return (c + 1) % sz + sz - 1;
}
template<> STATIC_SPECIAL MUSTINLINE
int32_t GetCoordHelper::Get<BO_CYCLIC, CP_ABOVE_SIZE>(int32_t c, int32_t sz) {
  return c % sz;
}


/// Interface function template
template<BorderOption border, CoordPrior prior = CP_ANYWHERE>
static MINIMGAPI_RAW_API int32_t GetCoordRaw(int32_t c, int32_t sz) {
  MINIMGAPI_DECLARE_STOPWATCH_CTL_RAW_INTERFACE(swGetCoordRaw);

  return GetCoordHelper::Call<border, prior>(c, sz);
}


} // namespace minimg::raw


#endif // #ifndef MINIMGAPI_RAW_GET_COORD_HPP_INCLUDED
