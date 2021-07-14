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
#ifndef MINSBASE_MINMETA_HPP_INCLUDED
#define MINSBASE_MINMETA_HPP_INCLUDED

#include "mintyp.h"
#include "minstd.hpp"

namespace minmeta {

/// the namespace below is added for convenience of experimentation
/// with different approaches by having 'using namespace <cur_version>' below
namespace simpler {


/// For convenient usage of something::type everywhere for types
template<class T> struct TypeTag { using type = T; };


template<class ... Ts> struct TypeList : TypeTag<TypeList<Ts ...> > {};


template<class List> struct Length;

template<class ... Ts> struct Length<TypeList<Ts ...> >
    : std::integral_constant<size_t, sizeof...(Ts)> {};


template<class List> struct Head;

template<class T, class ... Ts>
struct Head<TypeList<T, Ts ...> > : TypeTag<T> {};


template<class List> struct Tail;

template<class T, class ... Ts>
struct Tail<TypeList<T, Ts ...> > : TypeList<Ts ...> {};


/// TODO: Decide which argument order to use (or both)
template<class, class> struct PushFront;

template<class T, class ... Ts>
struct PushFront<T, TypeList<Ts ...> > : TypeList<T, Ts ...> {};

template<class ... Ts, class T>
struct PushFront<TypeList<Ts ...>, T> : TypeList<T, Ts ...> {};


/// For SFINAE usage
template<class ...> struct Void : TypeTag<void> {};


template<template<class ...> class Tpl, class List> class IsAppliable;

template<template<class ...> class Tpl, class ... Types>
class IsAppliable<Tpl, TypeList<Types ...> > {
  /// If Tpl<Types ...> is illegal, this one is used after SFINAE
  template<template<class ...> class F, class List, class = void> struct Probe
      : std::false_type {};
  /// If Tpl<Types ...> is legal, this one is used, as it's more specialized
  template<template<class ...> class F, class ... Ts>
  struct Probe<F, TypeList<Ts ...>,
      typename Void<F<Ts ...> >::type> : std::true_type {};

public:
  static const bool value = Probe<Tpl, TypeList<Types ...> >::value;
};

/// Convenience helper for checking if template is appliable to a single type
/// MAYBE: Use the type even without TypeTag
template<template<class ...> class Tpl, class T>
class IsAppliable<Tpl, TypeTag<T> > : public IsAppliable<Tpl, TypeList<T> > {};


/// Pass arguments of one template to another
/// MAYBE: separate to ExtractArgs (Cast to TypeList)
/// and Apply (Cast from TypeList)
template<template<class ...> class Tpl, class T> struct Cast;

template<template<class ...> class DT,
         template<class ...> class ST,
         class ... Ts>
struct Cast<DT, ST<Ts ...> > : TypeTag<DT<Ts ...> > {};


/// WRN: With excessive arguments SOMETIMES gives very obscure error message
template<template<class ...> class Tpl, class List> struct Curry;

template<template<class ...> class Tpl, class ... Types>
struct Curry<Tpl, TypeList<Types ...> > {
  template<class ... Ts> using tpl = Tpl<Types ..., Ts ...>;
};

/// Convenience helper to apply Curry to a single template argument
/// MAYBE: Use the argument type even without TypeTag
template<template<class ...> class Tpl, class T>
struct Curry<Tpl, TypeTag<T> > : Curry<Tpl, TypeList<T> > {};





} // namespace simpler


namespace alltpl {


template<typename T, T val> using ValTag = std::integral_constant<T, val>;


template<class Val> struct Inc
    : ValTag<typename Val::value_type,
             static_cast<typename Val::value_type>(Val::value + 1)> {};


template<class LVal, class RVal> struct Less
    : ValTag<bool, LVal::value < RVal::value> {};

template<class LVal, class RVal> struct Greater : Less<RVal, LVal> {};

template<class LVal, class RVal> struct Equal
    : ValTag<bool, LVal::value == RVal::value> {};



template<class T> struct TypeTag { using type = T; };


template<class ... Ts> struct TypeList : TypeTag<TypeList<Ts ...> > {};


template<class List> struct Length;

template<class ... Ts> struct Length<TypeList<Ts ...> >
    : ValTag<size_t, sizeof...(Ts)> {};


template<class List> struct Head;

template<class T, class ... Ts> struct Head<TypeList<T, Ts ...> >
    : TypeTag<T> {};


template<class List> struct Tail;

template<class T, class ... Ts> struct Tail<TypeList<T, Ts ...> >
    : TypeList<Ts ...> {};


template<class, class> struct PushFront;

template<class ... Ts, class T> struct PushFront<TypeList<Ts ...>, T>
    :TypeList<T, Ts ...> {};


template<class List> class Reverse;

template<class ... Ts> class Reverse<TypeList<Ts ...> > {
  template<class Dst, class Src> struct Proc
      : Proc<typename PushFront<Dst, typename Head<Src>::type>::type,
             typename Tail<Src>::type>::type {};
  template<class Dst> struct Proc<Dst, TypeList<> >
      : Dst {};
//  template<class Dst, class Src> struct Proc;
//  template<class ... DTs, class T, class ... STs>
//  struct Proc<TypeList<DTs ...>, TypeList<T, STs ...> >
//      : Proc<TypeList<T, DTs ...>, TypeList<STs ...> > {};
//  template<class ... DTs> struct Proc<TypeList<DTs ...>, TypeList<> >
//      : TypeList<DTs ...> {};
public:
  using type = typename Proc<TypeList<>, TypeList<Ts ...> >::type;
};


template<class Begin, class Cond, class Step> class Sequence {
  template<class Val, class C, class List> struct Proc
      : Proc<typename Step::template tpl<Val>::type,
             typename Cond::template tpl<typename Step::template tpl<Val>::type>::type,
             typename PushFront<List, Val>::type> {};
  template<class Val, class List> struct Proc<Val, std::false_type, List>
      : Reverse<List>::type {};
public:
  using type = typename Proc<Begin, typename Cond::template tpl<Begin>::type, TypeList<> >::type;
};



template<class ...> struct Void : TypeTag<void> {};



template<template<class ...> class TT> struct TplTag {
  template<class ... Ts> using tpl = TT<Ts ...>;
};


template<class TT, class List> struct Curry;

template<template<class ...> class Tpl, class ... Types>
struct Curry<TplTag<Tpl>, TypeList<Types ...> > {
  template<class ... Ts> using tpl = Tpl<Types ..., Ts ...>;
};

template<template<class ...> class Tpl, class T>
struct Curry<TplTag<Tpl>, T> : Curry<TplTag<Tpl>, TypeList<T> > {};




template<class Enum, Enum begin, Enum end>
using EnumSequence = typename std::enable_if<std::is_enum<Enum>::value,
Sequence<ValTag<Enum, begin>,
         Curry<TplTag<Greater>, ValTag<Enum, end> >,
         TplTag<Inc> > >::type;


template<class IndicesList, class TT> struct CreateArrayEx;

template<class ... Ts, class TT>
struct CreateArrayEx<TypeList<Ts ...>, TT > {
  using value_type =
      typename TT::template tpl<typename Head<TypeList<Ts ...> >::type>::value_type;
  static const value_type values[];
//  constexpr static const value_type values[] = { TT::template tpl<Ts>::value ... };
};

template<class ... Ts, class TT>
const typename CreateArrayEx<TypeList<Ts ...>, TT>::value_type
CreateArrayEx<TypeList<Ts ...>, TT>::values[] = {
  TT::template tpl<Ts>::value ...
};


//template<class List> using CreateArray = CreateArrayEx<List, TplTag<TypeTag> >;


} // namespace alltpl


using namespace alltpl;

} // namespace minmeta

#endif // #ifndef MINSBASE_MINMETA_HPP_INCLUDED
