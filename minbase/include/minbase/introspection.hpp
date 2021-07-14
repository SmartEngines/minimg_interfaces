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
#ifndef MINSBASE_INTROSPECTION_HPP_INCLUDED
#define MINSBASE_INTROSPECTION_HPP_INCLUDED

#include "minpreprocessor.h"
#include "minstd.hpp"


/// TODO: Maybe add namespace?

/// TODO: Check on other compilers


///
/// Check of the type being defined (not an introspection actually)
///

/// To check existence of some class, it needs to be declared first
template<class T>
class IsDefined {
  template<class U> static std::true_type C(
      typename std::enable_if< (sizeof(U) > 0) >::type*);
  template<class U> static std::false_type C(...);
public:
  static const bool value = decltype(C<T>(nullptr))::value;
};




/// Problem: I'd like to keep it c++03, but then it's troublesome to specialize
/// both for just a sigle class and with additional check without depending
/// on minmeta.
/// But I'd prefer minmeta to depend on introspection, not vise versa.
///
/// Sure, it's not c++03 now, for decltype's and nullptr's,
/// but that can be fixed with sizeof stuff.
///
/// Data introspection without specifing type seems not implementable in c++03


///
/// Helper(s)
///
namespace min { namespace introspection {

template<class T> struct AlwaysTrue {
  static const bool value = true;
};

/// TODO: Add traits for const / nonconst for methods

struct MemberTraitStatic;
struct MemberTraitNonstatic;
struct MemberTraitAnystatic;

//enum MemberTrait {
//  MTAnyStaticAnyConst = 0,
//  MTAnyStaticNonConst = 1 << 0,
//  MTAnyStaticConst    = 1 << 1,
//  MTNonStaticAnyConst = 1 << 2,
//  MTStaticAnyConst    = 1 << 3
//};

//template<class ...> class Args;

} }


///
/// Inner type introspection
///

#ifndef MIN_INTROSPECTION_DEFINE_HAS_TYPE
# define MIN_INTROSPECTION_DEFINE_HAS_TYPE(name)  \
  template<class Subject, template<class> class Constraint  \
      = ::min::introspection::AlwaysTrue>  \
  class MIN_PP_CONCAT(HasType_, name) {  \
    template<class T> static std::true_type Probe(typename  \
        std::enable_if<Constraint<typename T::name>::value>::type*); \
    template<class T> static std::false_type Probe(...);  \
    public: \
    typedef decltype(Probe<Subject>(nullptr)) type; \
    static const bool value = type::value;  \
  }
#endif


///
/// Data introspection (general, static, and member)
///

/// FIXME: / TODO: For union reports correctly, for enum doesn't compile
/// For struct with anon union and for struct with anon union reports nonstatic
/// Need at least to check if that can be considered correct behaviour

#ifndef MIN_INTROSPECTION_DEFINE_HAS_DATA
# define MIN_INTROSPECTION_DEFINE_HAS_DATA(name)  \
  template<class Subject, \
           class StaticMod = ::min::introspection::MemberTraitAnystatic,  \
           template<class> class Constraint = ::min::introspection::AlwaysTrue> \
  class MIN_PP_CONCAT(HasData_, name) {  \
    template<class T> static std::true_type Probe(  \
        typename std::enable_if<Constraint<decltype(T::name)>::value && \
            !std::is_function<decltype(T::name)>::value>::type*); \
    template<class T> static std::false_type Probe(...);  \
    \
    template<class P> struct Helper : std::true_type {};  \
    template<class T, class D> struct Helper<D T::*> : std::false_type {};  \
    \
    template<class C> using ModNotSame =  \
        std::integral_constant<bool, !std::is_same<StaticMod, C>::value>; \
    \
    template<class T> \
    static ModNotSame<::min::introspection::MemberTraitNonstatic> ProbeStatic(  \
        typename std::enable_if<decltype(Probe<T>(nullptr))::value && \
                                Helper<decltype(&T::name)>::value>::type*); \
    template<class T> \
    static ModNotSame<::min::introspection::MemberTraitStatic> ProbeStatic(...);  \
    \
  public: \
    typedef std::integral_constant<bool,  \
        decltype(Probe<Subject>(nullptr))::value && \
        decltype(ProbeStatic<Subject>(nullptr))::value> type; \
    static const bool value = type::value;  \
  }
#endif

/// Another way to implement this

///// Works now, but ugly (and probably not implementable without decltype)
//template<class Subject> class HasStaticData_value {
//  template<class T> struct TypeTpl {
//    template<decltype(T::value)*> struct PtrTpl;
//  };
//  template<class T> static std::true_type Probe(
//      typename TypeTpl<T>::template PtrTpl<&T::value>*);
//  template<class T> static std::false_type Probe(...);
//public: static const bool value = decltype(Probe<Subject>(nullptr))::value; };



///
/// Method introspection
///

/// TODO: Probably reimplement with "named template args"
/// (class templates declared to serve only to pass arg inside,
/// to distinguish between different cases explicitly


#ifndef MIN_INTROSPECTION_DEFINE_HAS_FUNCTION
# define MIN_INTROSPECTION_DEFINE_HAS_FUNCTION(name)  \
  template<class Subject, class ... Args> \
  class MIN_PP_CONCAT(HasFunction_, name) {  \
    template<class T> static std::true_type ProbeNonstatic( \
        decltype(std::declval<T>().name(std::declval<Args>() ...))*);  \
    template<class T> static std::false_type ProbeNonstatic(...); \
  \
  public: \
    typedef decltype(ProbeNonstatic<Subject>(nullptr)) type;  \
    static const bool value = type::value;  \
  };  \
  \
  template<class Subject, class ... Args> \
  class MIN_PP_CONCAT(HasFunction_, name)< \
      Subject, ::min::introspection::MemberTraitAnystatic, Args ...>  \
      : public MIN_PP_CONCAT(HasFunction_, name)<Subject, Args ...> {};  \
  \
  template<class Subject, class ... Args> \
  class MIN_PP_CONCAT(HasFunction_, name)< \
      Subject, ::min::introspection::MemberTraitNonstatic, Args ...> {  \
  public: \
    typedef std::integral_constant<bool,  \
        MIN_PP_CONCAT(HasFunction_, name)<Subject, Args ...>::value && \
        !MIN_PP_CONCAT(HasFunction_, name)<Subject,  \
            ::min::introspection::MemberTraitStatic, Args ...>::value> type;  \
    static const bool value = type::value;  \
  };  \
  \
  template<class Subject, class ... Args> \
  class MIN_PP_CONCAT(HasFunction_, name)< \
      Subject, ::min::introspection::MemberTraitStatic, Args ...> { \
    template<class T> static std::true_type ProbeStatic(  \
        decltype(T::name(std::declval<Args>() ...))*); \
    template<class T> static std::false_type ProbeStatic(...);  \
  \
  public: \
    typedef decltype(ProbeStatic<Subject>(nullptr)) type; \
    static const bool value = type::value;  \
  }
#endif





/// Previous attempts

/*

//#ifndef MIN_INTROSPECTION_DEFINE_HAS_FUNCTION
//# define MIN_INTROSPECTION_DEFINE_HAS_FUNCTION(name)  \
//  template< \
//      class, class = ::min::introspection::MemberTraitAnystatic, class ...>  \
//  class MIN_PP_CONCAT(HasFunction_, name); \
//  \
//  template<class Subject, class StaticMod>  \
//  class MIN_PP_CONCAT(HasFunction_, name)<Subject, StaticMod> {  \
//    template<class C> using ModNotSame =  \
//        std::integral_constant<bool, !std::is_same<StaticMod, C>::value>; \
//    \
//    template<class T> \
//    static ModNotSame<::min::introspection::MemberTraitStatic> Probe( \
//        typename std::enable_if<std::is_member_function_pointer<  \
//            decltype(&T::name)>::value>::type*); \
//    template<class T> \
//    static ModNotSame<::min::introspection::MemberTraitNonstatic> Probe(  \
//        typename std::enable_if<  \
//            std::is_function<decltype(T::name)>::value>::type*);  \
//    template<class T> static std::false_type Probe(...);  \
//    \
//  public: \
//    typedef decltype(Probe<Subject>(nullptr)) type; \
//    static const bool value = type::value;  \
//  }; \
//  \
//  template<class Subject, class StaticMod, class TRet, class ... TArgs> \
//  class MIN_PP_CONCAT(HasFunction_, name)<Subject, StaticMod, TRet(TArgs ...)> { \
//    template<class T> static std::true_type Probe(typename std::enable_if<  \
//            MIN_PP_CONCAT(HasFunction_, name)<T, StaticMod>::value &&  \
//            std::is_same<TRet, decltype(std::declval<T>().name( \
//                std::declval<TArgs>() ...))>::value>::type*); \
//    template<class T> static std::false_type Probe(...);  \
//    \
//  public: \
//    static const bool value = decltype(Probe<Subject>(nullptr))::value; \
//  }

//#endif

///// TODO: Add check of const-ness
//template<class, class = ::min::introspection::MemberTraitAnystatic, class ...>
//class HasMethod_X;

//template<class Subject, class StaticMod>
//class HasMethod_X<Subject, StaticMod> {
//  template<class C> using ModNotSame =
//      std::integral_constant<bool, !std::is_same<StaticMod, C>::value>;

//  template<class T>
//  static ModNotSame<::min::introspection::MemberTraitStatic> Probe(
//      typename std::enable_if<
//          std::is_member_function_pointer<decltype(&T::X)>::value>::type*);
//  template<class T>
//  static ModNotSame<::min::introspection::MemberTraitNonstatic> Probe(
//      typename std::enable_if<
//          std::is_function<decltype(T::X)>::value>::type*);
//  template<class T> static std::false_type Probe(...);
//public: typedef decltype(Probe<Subject>(nullptr)) type;
//        static const bool value = type::value; };

//template<class Subject, class StaticMod, class TRet, class ... TArgs>
//class HasMethod_X<Subject, StaticMod, TRet(TArgs ...)> {
//  template<class T> static std::true_type Probe(
//      typename std::enable_if<HasMethod_X<T, StaticMod>::value &&
//          std::is_same<TRet, decltype(
//              std::declval<T>().X(std::declval<TArgs>() ...))>::value>::type*);
//  template<class T> static std::false_type Probe(...);
//public:
//  static const bool value = decltype(Probe<Subject>(nullptr))::value;
//};



///// UNFINISHED. Finish and check
//template<class Subject, class ...> class HasMethod_Foo;

//template<class Subject> class HasMethod_Foo<Subject> {
//  template<class T> static std::true_type P1(decltype(T::Foo)*);
//  template<class T> static std::false_type P1(...);

//  template<class F> struct P2 : std::false_type {};
//  template<class T, class Ret, class ... Args> struct P2<Ret(T::*)(Args ...)>
//    : std::true_type {};

//  template<class T> static std::true_type P3(
//      typename std::enable_if<decltype(P1<T>(nullptr))::value &&
//                              P2<decltype(&T::Foo)>::value>::type*);
//  template<class T> static std::true_type P3(...);

//public: static const bool value = decltype(P3<Subject>(nullptr))::value; };

//template<class Subject, class TRet, class ... TArgs>
//class HasMethod_Foo<Subject, TRet(TArgs ...)> {
//  //SOMETHING WITH SIG CHECK
//};

*/


///
/// Inner template introspection
///

/// To be implemented sometime later


#endif // #ifndef MINSBASE_INTROSPECTION_HPP_INCLUDED
