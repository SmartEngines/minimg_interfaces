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

#include <type_traits>

#include <gtest/gtest.h>

#include <minbase/introspection.hpp>



/// TODO: Add some struct named X itself

/// TODO: Add virtual functions to test
/// (and later - to check options, if possible)


struct Struct_Undefined;

struct Struct_Nothing {};

struct Struct_WithTypedefInt32 { typedef int32_t X; };
struct Struct_WithTypedefRecursive { typedef Struct_WithTypedefRecursive X; };
struct Struct_WithUsingUint8 { using X = uint8_t; };

struct Struct_DerivedWithTypedefInt32 : Struct_WithTypedefInt32 {};
struct Struct_DerivedWithUsingUint8 : Struct_WithUsingUint8 {};

struct Struct_WithStructUndefined { struct X; };
struct Struct_WithStructDefined { struct X {}; };
struct Struct_WithUnion { union X {}; };
struct Struct_WithEnum { enum X {}; };

struct Struct_DerivedWithStructUndefined : Struct_WithStructUndefined {};
struct Struct_DerivedWithStructDefined : Struct_WithStructDefined {};
struct Struct_DerivedWithUnion : Struct_WithUnion {};
struct Struct_DerivedWithEnum : Struct_WithEnum {};

struct Struct_WithNonstaticDataInt { int X; };
struct Struct_WithNonstaticDataDouble { double X; };
struct Struct_WithNonstaticDataPVoid { void* X; };
struct Struct_WithNonstaticDataPFuncIntInt { int(*X)(int); };
struct Struct_WithStaticDataInt { static int X; };
struct Struct_WithStaticDataDouble { static double X; };
struct Struct_WithStaticDataPVoid { static void* X; };
struct Struct_WithStaticDataPFuncIntInt { static int(*X)(int); };
struct Struct_WithOtherData { int Y; static double Z; };

struct Struct_WithNonstaticDataIntConst { int const X; };

struct Struct_WithAnonUnionDataInt { union { int X; }; };
struct Struct_WithAnonEnumData { enum { X }; };

struct Struct_DerivedWithNonstaticDataInt : Struct_WithNonstaticDataInt {};
struct Struct_DerivedWithNonstaticDataDouble : Struct_WithNonstaticDataDouble {};
struct Struct_DerivedWithNonstaticDataPVoid : Struct_WithNonstaticDataPVoid {};
struct Struct_DerivedWithNonstaticDataPFuncIntInt : Struct_WithNonstaticDataPFuncIntInt {};
struct Struct_DerivedWithStaticDataInt : Struct_WithStaticDataInt {};
struct Struct_DerivedWithStaticDataDouble : Struct_WithStaticDataDouble {};
struct Struct_DerivedWithStaticDataPVoid : Struct_WithStaticDataPVoid {};
struct Struct_DerivedWithStaticDataPFuncIntInt : Struct_WithStaticDataPFuncIntInt {};

struct Struct_WithNonstaticMethodVoidVoid { void X(); };
struct Struct_WithNonstaticMethodVoidIntDouble { void X(int, double); };
struct Struct_WithNonstaticMethodVoidPInt { void X(int*); };
struct Struct_WithNonstaticMethodVoidPConstInt { void X(int const*); };
struct Struct_WithNonstaticMethodVoidPFuncIntInt { void X(int(*)(int)); };
struct Struct_WithNonstaticMethodIntVoid { int X(); };
struct Struct_WithNonstaticMethodIntIntDouble { int X(int, double); };
struct Struct_WithStaticMethodVoidVoid { static void X(); };
struct Struct_WithStaticMethodVoidIntDouble { static void X(int, double); };
struct Struct_WithStaticMethodIntVoid { static int X(); };
struct Struct_WithStaticMethodIntIntDouble { static int X(int, double); };

struct Struct_WithNonstaticMethodVoidVoidConst { void X() const; };

struct Struct_DerivedWithNonstaticMethodVoidVoid : Struct_WithNonstaticMethodVoidVoid {};
struct Struct_DerivedWithNonstaticMethodVoidIntDouble : Struct_WithNonstaticMethodVoidIntDouble {};
struct Struct_DerivedWithNonstaticMethodVoidPFuncIntInt : Struct_WithNonstaticMethodVoidPFuncIntInt {};
struct Struct_DerivedWithNonstaticMethodIntVoid : Struct_WithNonstaticMethodIntVoid {};
struct Struct_DerivedWithNonstaticMethodIntIntDouble : Struct_WithNonstaticMethodIntIntDouble {};
struct Struct_DerivedWithStaticMethodVoidVoid : Struct_WithStaticMethodVoidVoid {};
struct Struct_DerivedWithStaticMethodVoidIntDouble : Struct_WithStaticMethodVoidIntDouble {};
struct Struct_DerivedWithStaticMethodIntVoid : Struct_WithStaticMethodIntVoid {};
struct Struct_DerivedWithStaticMethodIntIntDouble : Struct_WithStaticMethodIntIntDouble {};

class Class_PublicTypedef { public: typedef int X; };
class Class_ProtectedTypedef { protected: typedef int X; };
class Class_PrivateTypedef { typedef int X; };
class Class_PrivateStructUndefined { struct X; };
class Class_PrivateStructDefined { struct X {}; };
class Class_PrivateUnion { union X {}; };
class Class_PrivateEnum { enum X {}; };

class Class_PublicDerivedPublicTypedef : public Class_PublicTypedef {};
class Class_PrivateDerivedPublicTypedef : Class_PublicTypedef {};
class Class_PublicDerivedProtectedTypedef : public Class_ProtectedTypedef {};
class Class_PublicDerivedPrivateTypedef : public Class_PrivateTypedef {};

class Class_PrivateNonstaticDataInt { int X; };
class Class_PrivateStaticDataInt { static int X; };

class Class_PublicDerivedPrivateNonstaticDataInt : public Class_PrivateNonstaticDataInt {};
class Class_PublicDerivedPrivateStaticDataInt : public Class_PrivateStaticDataInt {};

class Class_PrivateNonstaticMethodVoidVoid { void X(); };
class Class_PrivateStaticMethodVoidVoid { static void X(); };

class Class_PublicDerivedPrivateNonstaticMethodVoidVoid : public Class_PrivateNonstaticMethodVoidVoid {};
class Class_PublicDerivedPrivateStaticMethodVoidVoid : public Class_PrivateStaticMethodVoidVoid {};

union Union_WithNonstaticDataInt { int X; };
union Union_WithStaticDataInt { static int X; };
union Union_WithNonstaticMethodVoidVoid { void X(); };
union Union_WithStaticMethodVoidVoid { static void X(); };

enum Enum_WithData { X };

/// To check for possible name clashes
struct X {};
struct Subject {};


struct InStruct {
  struct Struct_Undefined;

  struct Struct_Nothing {};

  struct Struct_WithTypedefInt32 { typedef int32_t X; };
  struct Struct_WithTypedefRecursive { typedef Struct_WithTypedefRecursive X; };
  struct Struct_WithUsingUint8 { using X = uint8_t; };

  struct Struct_DerivedWithTypedef : Struct_WithTypedefInt32 {};
  struct Struct_DerivedWithUsing : Struct_WithUsingUint8 {};

  struct Struct_WithStructUndefined { struct X; };
  struct Struct_WithStructDefined { struct X {}; };
  struct Struct_WithUnion { union X {}; };
  struct Struct_WithEnum { enum X {}; };

  struct Struct_DerivedWithStructUndefined : Struct_WithStructUndefined {};
  struct Struct_DerivedWithStructDefined : Struct_WithStructDefined {};
  struct Struct_DerivedWithUnion : Struct_WithUnion {};
  struct Struct_DerivedWithEnum : Struct_WithEnum {};

  struct Struct_WithNonstaticDataInt { int X; };
  struct Struct_WithNonstaticDataDouble { double X; };
  struct Struct_WithNonstaticDataPVoid { void* X; };
  struct Struct_WithNonstaticDataPFuncIntInt { int(*X)(int); };
  struct Struct_WithStaticDataInt { static int X; };
  struct Struct_WithStaticDataDouble { static double X; };
  struct Struct_WithStaticDataPVoid { static void* X; };
  struct Struct_WithStaticDataPFuncIntInt { static int(*X)(int); };
  struct Struct_WithOtherData { int Y; static double Z; };

  struct Struct_WithNonstaticDataIntConst { int const X; };

  struct Struct_WithAnonUnionDataInt { union { int X; }; };
  struct Struct_WithAnonEnumData { enum { X }; };

  struct Struct_DerivedWithNonstaticDataInt : Struct_WithNonstaticDataInt {};
  struct Struct_DerivedWithNonstaticDataDouble : Struct_WithNonstaticDataDouble {};
  struct Struct_DerivedWithNonstaticDataPVoid : Struct_WithNonstaticDataPVoid {};
  struct Struct_DerivedWithNonstaticDataPFuncIntInt : Struct_WithNonstaticDataPFuncIntInt {};
  struct Struct_DerivedWithStaticDataInt : Struct_WithStaticDataInt {};
  struct Struct_DerivedWithStaticDataDouble : Struct_WithStaticDataDouble {};
  struct Struct_DerivedWithStaticDataPVoid : Struct_WithStaticDataPVoid {};
  struct Struct_DerivedWithStaticDataPFuncIntInt : Struct_WithStaticDataPFuncIntInt {};

  struct Struct_WithNonstaticMethodVoidVoid { void X(); };
  struct Struct_WithNonstaticMethodVoidIntDouble { void X(int, double); };
  struct Struct_WithNonstaticMethodVoidPInt { void X(int*); };
  struct Struct_WithNonstaticMethodVoidPConstInt { void X(int const*); };
  struct Struct_WithNonstaticMethodVoidPFuncIntInt { void X(int(*)(int)); };
  struct Struct_WithNonstaticMethodIntVoid { int X(); };
  struct Struct_WithNonstaticMethodIntIntDouble { int X(int, double); };
  struct Struct_WithStaticMethodVoidVoid { static void X(); };
  struct Struct_WithStaticMethodVoidIntDouble { static void X(int, double); };
  struct Struct_WithStaticMethodIntVoid { static int X(); };
  struct Struct_WithStaticMethodIntIntDouble { static int X(int, double); };

  struct Struct_WithNonstaticMethodVoidVoidConst { void X() const; };

  struct Struct_DerivedWithNonstaticMethodVoidVoid : Struct_WithNonstaticMethodVoidVoid {};
  struct Struct_DerivedWithNonstaticMethodVoidIntDouble : Struct_WithNonstaticMethodVoidIntDouble {};
  struct Struct_DerivedWithNonstaticMethodVoidPFuncIntInt : Struct_WithNonstaticMethodVoidPFuncIntInt {};
  struct Struct_DerivedWithNonstaticMethodIntVoid : Struct_WithNonstaticMethodIntVoid {};
  struct Struct_DerivedWithNonstaticMethodIntIntDouble : Struct_WithNonstaticMethodIntIntDouble {};
  struct Struct_DerivedWithStaticMethodVoidVoid : Struct_WithStaticMethodVoidVoid {};
  struct Struct_DerivedWithStaticMethodVoidIntDouble : Struct_WithStaticMethodVoidIntDouble {};
  struct Struct_DerivedWithStaticMethodIntVoid : Struct_WithStaticMethodIntVoid {};
  struct Struct_DerivedWithStaticMethodIntIntDouble : Struct_WithStaticMethodIntIntDouble {};

  class Class_PublicTypedef { public: typedef int X; };
  class Class_ProtectedTypedef { protected: typedef int X; };
  class Class_PrivateTypedef { typedef int X; };
  class Class_PrivateStructUndefined { struct X; };
  class Class_PrivateStructDefined { struct X {}; };
  class Class_PrivateUnion { union X {}; };
  class Class_PrivateEnum { enum X {}; };

  class Class_PublicDerivedPublicTypedef : public Class_PublicTypedef {};
  class Class_PrivateDerivedPublicTypedef : Class_PublicTypedef {};
  class Class_PublicDerivedProtectedTypedef : public Class_ProtectedTypedef {};
  class Class_PublicDerivedPrivateTypedef : public Class_PrivateTypedef {};

  class Class_PrivateNonstaticDataInt { int X; };
  class Class_PrivateStaticDataInt { static int X; };

  class Class_PublicDerivedPrivateNonstaticDataInt : public Class_PrivateNonstaticDataInt {};
  class Class_PublicDerivedPrivateStaticDataInt : public Class_PrivateStaticDataInt {};

  class Class_PrivateNonstaticMethodVoidVoid { void X(); };
  class Class_PrivateStaticMethodVoidVoid { static void X(); };

  class Class_PublicDerivedPrivateNonstaticMethodVoidVoid : public Class_PrivateNonstaticMethodVoidVoid {};
  class Class_PublicDerivedPrivateStaticMethodVoidVoid : public Class_PrivateStaticMethodVoidVoid {};

  union Union_WithNonstaticDataInt { int X; };
  union Union_WithStaticDataInt { static int X; };
  union Union_WithNonstaticMethodVoidVoid { void X(); };
  union Union_WithStaticMethodVoidVoid { static void X(); };

  enum Enum_WithData { X };

  /// To check for possible name clashes
  struct X {};
  struct Subject {};
};


TEST(TestMinbaseIntrospection, IsDefined) {
  EXPECT_FALSE(IsDefined<Struct_Undefined>::value);
  EXPECT_TRUE(IsDefined<Struct_Nothing>::value);
  EXPECT_TRUE(IsDefined<Struct_WithTypedefInt32::X>::value);
  EXPECT_TRUE(IsDefined<Struct_WithTypedefRecursive::X>::value);
  EXPECT_TRUE(IsDefined<Struct_WithUsingUint8::X>::value);
  EXPECT_TRUE(IsDefined<Struct_DerivedWithTypedefInt32::X>::value);
  EXPECT_FALSE(IsDefined<Struct_WithStructUndefined::X>::value);
  EXPECT_TRUE(IsDefined<Struct_WithStructDefined::X>::value);
  EXPECT_TRUE(IsDefined<Struct_WithUnion::X>::value);
  EXPECT_TRUE(IsDefined<Struct_WithUnion::X>::value);

  EXPECT_TRUE(IsDefined<decltype(Struct_WithNonstaticDataInt::X)>::value);

  EXPECT_TRUE(IsDefined<Union_WithNonstaticDataInt>::value);
  EXPECT_TRUE(IsDefined<Enum_WithData>::value);

  /// Doesn't compile (and shouldn't)
//  EXPECT_TRUE(IsDefined<Struct_WithNonstaticDataInt::X>::value);
//  EXPECT_TRUE(IsDefined<Struct_WithNonstaticMethodVoidVoid::X>::value);
//  EXPECT_TRUE(IsDefined<Class_ProtectedTypedef::X>::value);
//  EXPECT_TRUE(IsDefined<Class_PrivateStructUndefined::X>::value);
//  EXPECT_TRUE(IsDefined<Class_PublicDerivedProtectedTypedef::X>::value);

  EXPECT_FALSE(IsDefined<InStruct::Struct_Undefined>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_Nothing>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_WithTypedefInt32::X>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_WithTypedefRecursive::X>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_WithUsingUint8::X>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_DerivedWithTypedef::X>::value);
  EXPECT_FALSE(IsDefined<InStruct::Struct_WithStructUndefined::X>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_WithStructDefined::X>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_WithUnion::X>::value);
  EXPECT_TRUE(IsDefined<InStruct::Struct_WithUnion::X>::value);

  EXPECT_TRUE(IsDefined<decltype(InStruct::Struct_WithNonstaticDataInt::X)>::value);

  EXPECT_TRUE(IsDefined<InStruct::Union_WithNonstaticDataInt>::value);
  EXPECT_TRUE(IsDefined<InStruct::Enum_WithData>::value);

  /// Doesn't compile (and shouldn't)
//  EXPECT_TRUE(IsDefined<InStruct::Struct_WithNonstaticDataInt::X>::value);
//  EXPECT_TRUE(IsDefined<InStruct::Struct_WithNonstaticMethodVoidVoid::X>::value);
//  EXPECT_TRUE(IsDefined<InStruct::Class_ProtectedTypedef::X>::value);
//  EXPECT_TRUE(IsDefined<InStruct::Class_PrivateStructUndefined::X>::value);
//  EXPECT_TRUE(IsDefined<InStruct::Class_PublicDerivedProtectedTypedef::X>::value);

  EXPECT_TRUE(IsDefined<int>::value);
  EXPECT_TRUE(IsDefined<int*>::value);
  EXPECT_TRUE(IsDefined<void*>::value);
  EXPECT_TRUE(IsDefined<int(*)(int, double)>::value);

  /// not appliable to void
  /// and to function types (but of for function pointers)
}


MIN_INTROSPECTION_DEFINE_HAS_TYPE(X);
MIN_INTROSPECTION_DEFINE_HAS_TYPE(type);
TEST(TestMinbaseIntrospection, HasType) {
  EXPECT_FALSE(HasType_X<Struct_Undefined>::value);

  EXPECT_FALSE(HasType_X<Struct_Nothing>::value);

  EXPECT_TRUE(HasType_X<Struct_WithTypedefInt32>::value);
  EXPECT_TRUE(HasType_X<Struct_WithTypedefRecursive>::value);
  EXPECT_TRUE(HasType_X<Struct_WithUsingUint8>::value);

  EXPECT_TRUE(HasType_X<Struct_DerivedWithTypedefInt32>::value);
  EXPECT_TRUE(HasType_X<Struct_DerivedWithUsingUint8>::value);

  EXPECT_TRUE(HasType_X<Struct_WithStructUndefined>::value);
  EXPECT_TRUE(HasType_X<Struct_WithStructDefined>::value);
  EXPECT_TRUE(HasType_X<Struct_WithUnion>::value);
  EXPECT_TRUE(HasType_X<Struct_WithEnum>::value);

  EXPECT_TRUE(HasType_X<Struct_DerivedWithStructUndefined>::value);
  EXPECT_TRUE(HasType_X<Struct_DerivedWithStructDefined>::value);
  EXPECT_TRUE(HasType_X<Struct_DerivedWithUnion>::value);
  EXPECT_TRUE(HasType_X<Struct_DerivedWithEnum>::value);

  EXPECT_FALSE(HasType_X<Struct_WithNonstaticDataInt>::value);
  EXPECT_FALSE(HasType_X<Struct_WithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE(HasType_X<Struct_WithStaticDataInt>::value);

  EXPECT_FALSE(HasType_X<Struct_WithAnonUnionDataInt>::value);
  EXPECT_FALSE(HasType_X<Struct_WithAnonEnumData>::value);

  EXPECT_FALSE(HasType_X<Struct_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasType_X<Struct_WithStaticMethodVoidVoid>::value);

  EXPECT_TRUE(HasType_X<Class_PublicTypedef>::value);
  EXPECT_FALSE(HasType_X<Class_ProtectedTypedef>::value);
  EXPECT_FALSE(HasType_X<Class_PrivateTypedef>::value);
  EXPECT_FALSE(HasType_X<Class_PrivateStructUndefined>::value);
  EXPECT_FALSE(HasType_X<Class_PrivateStructDefined>::value);
  EXPECT_FALSE(HasType_X<Class_PrivateUnion>::value);
  EXPECT_FALSE(HasType_X<Class_PrivateEnum>::value);

  EXPECT_TRUE(HasType_X<Class_PublicDerivedPublicTypedef>::value);
  EXPECT_FALSE(HasType_X<Class_PrivateDerivedPublicTypedef>::value);
  EXPECT_FALSE(HasType_X<Class_PublicDerivedProtectedTypedef>::value);
  EXPECT_FALSE(HasType_X<Class_PublicDerivedPrivateTypedef>::value);

  EXPECT_FALSE(HasType_X<Union_WithNonstaticDataInt>::value);
  EXPECT_FALSE(HasType_X<Union_WithStaticDataInt>::value);
  EXPECT_FALSE(HasType_X<Union_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasType_X<Union_WithStaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasType_X<Enum_WithData>::value);

  EXPECT_FALSE(HasType_X<InStruct::Struct_Undefined>::value);

  EXPECT_FALSE(HasType_X<InStruct::Struct_Nothing>::value);

  EXPECT_TRUE(HasType_X<InStruct::Struct_WithTypedefInt32>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_WithTypedefRecursive>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_WithUsingUint8>::value);

  EXPECT_TRUE(HasType_X<InStruct::Struct_DerivedWithTypedef>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_DerivedWithUsing>::value);

  EXPECT_TRUE(HasType_X<InStruct::Struct_WithStructUndefined>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_WithStructDefined>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_WithUnion>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_WithEnum>::value);

  EXPECT_TRUE(HasType_X<InStruct::Struct_DerivedWithStructUndefined>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_DerivedWithStructDefined>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_DerivedWithUnion>::value);
  EXPECT_TRUE(HasType_X<InStruct::Struct_DerivedWithEnum>::value);

  EXPECT_FALSE(HasType_X<InStruct::Struct_WithNonstaticDataInt>::value);
  EXPECT_FALSE(HasType_X<InStruct::Struct_WithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE(HasType_X<InStruct::Struct_WithStaticDataInt>::value);

  EXPECT_FALSE(HasType_X<InStruct::Struct_WithAnonUnionDataInt>::value);
  EXPECT_FALSE(HasType_X<InStruct::Struct_WithAnonEnumData>::value);

  EXPECT_FALSE(HasType_X<InStruct::Struct_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasType_X<InStruct::Struct_WithStaticMethodVoidVoid>::value);

  EXPECT_TRUE(HasType_X<InStruct::Class_PublicTypedef>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_ProtectedTypedef>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PrivateTypedef>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PrivateStructUndefined>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PrivateStructDefined>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PrivateUnion>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PrivateEnum>::value);

  EXPECT_TRUE(HasType_X<InStruct::Class_PublicDerivedPublicTypedef>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PrivateDerivedPublicTypedef>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PublicDerivedProtectedTypedef>::value);
  EXPECT_FALSE(HasType_X<InStruct::Class_PublicDerivedPrivateTypedef>::value);

  EXPECT_FALSE(HasType_X<InStruct::Union_WithNonstaticDataInt>::value);
  EXPECT_FALSE(HasType_X<InStruct::Union_WithStaticDataInt>::value);
  EXPECT_FALSE(HasType_X<InStruct::Union_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasType_X<InStruct::Union_WithStaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasType_X<InStruct::Enum_WithData>::value);

  EXPECT_FALSE(HasType_X<int>::value);
  EXPECT_FALSE(HasType_X<int*>::value);
  EXPECT_FALSE(HasType_X<void>::value);
  EXPECT_FALSE(HasType_X<void(int)>::value);
  EXPECT_FALSE(HasType_X<void(*)(int)>::value);
  EXPECT_FALSE(HasType_X<std::vector<int> >::value);
  EXPECT_FALSE((HasType_X<std::conditional<true, int, double> >::value));

  EXPECT_TRUE((HasType_type<std::conditional<true, int, double> >::value));

  /// Test Constraints
  EXPECT_TRUE((HasType_X<Struct_WithTypedefInt32, std::is_integral>::value));
  EXPECT_TRUE((HasType_X<Struct_WithTypedefInt32, std::is_signed>::value));
  EXPECT_TRUE((HasType_X<Struct_WithUsingUint8, std::is_integral>::value));
  EXPECT_FALSE((HasType_X<Struct_WithUsingUint8, std::is_signed>::value));
  EXPECT_FALSE((HasType_X<Struct_WithStructUndefined, std::is_integral>::value));
  EXPECT_FALSE((HasType_X<int, std::is_integral>::value));

  EXPECT_FALSE((HasType_X<Struct_WithStructUndefined, IsDefined>::value));
  EXPECT_TRUE((HasType_X<Struct_WithStructDefined, IsDefined>::value));

  /// Test for possible name clashes
  /// FIXME: PROBLEM HERE (or not)
//  EXPECT_FALSE(HasType_X<struct X>::value);
  EXPECT_FALSE(HasType_X<Subject>::value);
}


MIN_INTROSPECTION_DEFINE_HAS_DATA(X);
MIN_INTROSPECTION_DEFINE_HAS_DATA(value);
MIN_INTROSPECTION_DEFINE_HAS_DATA(first);
TEST(TestMinbaseIntrospection, HasData) {
  using MTA = min::introspection::MemberTraitAnystatic;
  using MTS = min::introspection::MemberTraitStatic;
  using MTN = min::introspection::MemberTraitNonstatic;

  /// TODO: Check const-ness ?

  EXPECT_FALSE(HasData_X<Struct_Undefined>::value);
  EXPECT_FALSE(HasData_X<Struct_Nothing>::value);

  EXPECT_FALSE(HasData_X<Struct_WithTypedefInt32>::value);
  EXPECT_FALSE(HasData_X<Struct_WithStructUndefined>::value);
  EXPECT_FALSE(HasData_X<Struct_WithStructDefined>::value);
  EXPECT_FALSE(HasData_X<Struct_WithUnion>::value);
  EXPECT_FALSE(HasData_X<Struct_WithEnum>::value);

  EXPECT_TRUE(HasData_X<Struct_WithNonstaticDataInt>::value);
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_WithNonstaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithNonstaticDataDouble>::value);
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticDataDouble, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_WithNonstaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithNonstaticDataPVoid>::value);
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticDataPVoid, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_WithNonstaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticDataPFuncIntInt, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_WithNonstaticDataPFuncIntInt, MTN>::value));

//  EXPECT_TRUE(HasData_X<Struct_WithNonstaticDataIntConst>::value);

  EXPECT_TRUE(HasData_X<Struct_WithStaticDataInt>::value);
  EXPECT_TRUE((HasData_X<Struct_WithStaticDataInt, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithStaticDataDouble>::value);
  EXPECT_TRUE((HasData_X<Struct_WithStaticDataDouble, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithStaticDataPVoid>::value);
  EXPECT_TRUE((HasData_X<Struct_WithStaticDataPVoid, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithStaticDataPFuncIntInt>::value);
  EXPECT_TRUE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTN>::value));

  EXPECT_FALSE(HasData_X<Struct_WithOtherData>::value);

  EXPECT_TRUE(HasData_X<Struct_WithAnonUnionDataInt>::value);
  EXPECT_FALSE((HasData_X<Struct_WithAnonUnionDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_WithAnonUnionDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_WithAnonEnumData>::value);
  /// TODO: Check if that's good
  EXPECT_FALSE((HasData_X<Struct_WithAnonEnumData, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_WithAnonEnumData, MTN>::value));

  EXPECT_TRUE(HasData_X<Struct_DerivedWithNonstaticDataInt>::value);
  EXPECT_FALSE((HasData_X<Struct_DerivedWithNonstaticDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_DerivedWithNonstaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithNonstaticDataDouble>::value);
  EXPECT_FALSE((HasData_X<Struct_DerivedWithNonstaticDataDouble, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_DerivedWithNonstaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithNonstaticDataPVoid>::value);
  EXPECT_FALSE((HasData_X<Struct_DerivedWithNonstaticDataPVoid, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_DerivedWithNonstaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE((HasData_X<Struct_DerivedWithNonstaticDataPFuncIntInt, MTS>::value));
  EXPECT_TRUE((HasData_X<Struct_DerivedWithNonstaticDataPFuncIntInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithStaticDataInt>::value);
  EXPECT_TRUE((HasData_X<Struct_DerivedWithStaticDataInt, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_DerivedWithStaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithStaticDataDouble>::value);
  EXPECT_TRUE((HasData_X<Struct_DerivedWithStaticDataDouble, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_DerivedWithStaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithStaticDataPVoid>::value);
  EXPECT_TRUE((HasData_X<Struct_DerivedWithStaticDataPVoid, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_DerivedWithStaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<Struct_DerivedWithStaticDataPFuncIntInt>::value);
  EXPECT_TRUE((HasData_X<Struct_DerivedWithStaticDataPFuncIntInt, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_DerivedWithStaticDataPFuncIntInt, MTN>::value));

  EXPECT_FALSE(HasData_X<Struct_WithNonstaticMethodVoidPFuncIntInt>::value);
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticMethodVoidPFuncIntInt, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticMethodVoidPFuncIntInt, MTN>::value));
  EXPECT_FALSE(HasData_X<Struct_WithStaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasData_X<Struct_WithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticMethodVoidVoid, MTN>::value));
  EXPECT_FALSE(HasData_X<Struct_WithNonstaticMethodVoidVoidConst>::value);
  EXPECT_FALSE(HasData_X<Struct_DerivedWithNonstaticMethodVoidVoid>::value);

  EXPECT_FALSE(HasData_X<Class_PrivateNonstaticDataInt>::value);
  EXPECT_FALSE(HasData_X<Class_PrivateStaticDataInt>::value);

  EXPECT_FALSE(HasData_X<Class_PublicDerivedPrivateNonstaticDataInt>::value);
  EXPECT_FALSE(HasData_X<Class_PublicDerivedPrivateStaticDataInt>::value);
  EXPECT_FALSE(HasData_X<Class_PrivateNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasData_X<Class_PrivateStaticMethodVoidVoid>::value);

  EXPECT_TRUE(HasData_X<Union_WithNonstaticDataInt>::value);
  EXPECT_FALSE((HasData_X<Union_WithNonstaticDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<Union_WithNonstaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<Union_WithStaticDataInt>::value);
  EXPECT_TRUE((HasData_X<Union_WithStaticDataInt, MTS>::value));
  EXPECT_FALSE((HasData_X<Union_WithStaticDataInt, MTN>::value));
  EXPECT_FALSE(HasData_X<Union_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasData_X<Union_WithStaticMethodVoidVoid>::value);

  /// Doesn't compile
//  EXPECT_FALSE(HasData_X<Enum_WithData>::value);

  EXPECT_FALSE(HasData_X<InStruct::InStruct::Struct_Undefined>::value);
  EXPECT_FALSE(HasData_X<InStruct::Struct_Nothing>::value);

  EXPECT_FALSE(HasData_X<InStruct::Struct_WithTypedefInt32>::value);
  EXPECT_FALSE(HasData_X<InStruct::Struct_WithStructUndefined>::value);
  EXPECT_FALSE(HasData_X<InStruct::Struct_WithStructDefined>::value);
  EXPECT_FALSE(HasData_X<InStruct::Struct_WithUnion>::value);
  EXPECT_FALSE(HasData_X<InStruct::Struct_WithEnum>::value);

  EXPECT_TRUE(HasData_X<InStruct::Struct_WithNonstaticDataInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithNonstaticDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithNonstaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithNonstaticDataDouble>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithNonstaticDataDouble, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithNonstaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithNonstaticDataPVoid>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithNonstaticDataPVoid, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithNonstaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithNonstaticDataPFuncIntInt, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithNonstaticDataPFuncIntInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithStaticDataInt>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithStaticDataInt, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithStaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithStaticDataDouble>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithStaticDataDouble, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithStaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithStaticDataPVoid>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithStaticDataPVoid, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithStaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithStaticDataPFuncIntInt>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithStaticDataPFuncIntInt, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithStaticDataPFuncIntInt, MTN>::value));

  EXPECT_FALSE(HasData_X<InStruct::Struct_WithOtherData>::value);

  EXPECT_TRUE(HasData_X<InStruct::Struct_WithAnonUnionDataInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithAnonUnionDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithAnonUnionDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_WithAnonEnumData>::value);
  /// TODO: Check if that's good
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithAnonEnumData, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_WithAnonEnumData, MTN>::value));

  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithNonstaticDataInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithNonstaticDataDouble>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataDouble, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithNonstaticDataPVoid>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataPVoid, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataPFuncIntInt, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithNonstaticDataPFuncIntInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithStaticDataInt>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithStaticDataInt, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithStaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithStaticDataDouble>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithStaticDataDouble, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithStaticDataDouble, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithStaticDataPVoid>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithStaticDataPVoid, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithStaticDataPVoid, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Struct_DerivedWithStaticDataPFuncIntInt>::value);
  EXPECT_TRUE((HasData_X<InStruct::Struct_DerivedWithStaticDataPFuncIntInt, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_DerivedWithStaticDataPFuncIntInt, MTN>::value));

  EXPECT_FALSE(HasData_X<InStruct::Struct_WithNonstaticMethodVoidPFuncIntInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithNonstaticMethodVoidPFuncIntInt, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithNonstaticMethodVoidPFuncIntInt, MTN>::value));
  EXPECT_FALSE(HasData_X<InStruct::Struct_WithStaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Struct_WithStaticMethodVoidVoid, MTN>::value));
  EXPECT_FALSE(HasData_X<InStruct::Struct_WithNonstaticMethodVoidVoidConst>::value);
  EXPECT_FALSE(HasData_X<InStruct::Struct_DerivedWithNonstaticMethodVoidVoid>::value);

  EXPECT_FALSE(HasData_X<InStruct::Class_PrivateNonstaticDataInt>::value);
  EXPECT_FALSE(HasData_X<InStruct::Class_PrivateStaticDataInt>::value);

  EXPECT_FALSE(HasData_X<InStruct::Class_PublicDerivedPrivateNonstaticDataInt>::value);
  EXPECT_FALSE(HasData_X<InStruct::Class_PublicDerivedPrivateStaticDataInt>::value);
  EXPECT_FALSE(HasData_X<InStruct::Class_PrivateNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasData_X<InStruct::Class_PrivateStaticMethodVoidVoid>::value);

  EXPECT_TRUE(HasData_X<InStruct::Union_WithNonstaticDataInt>::value);
  EXPECT_FALSE((HasData_X<InStruct::Union_WithNonstaticDataInt, MTS>::value));
  EXPECT_TRUE((HasData_X<InStruct::Union_WithNonstaticDataInt, MTN>::value));
  EXPECT_TRUE(HasData_X<InStruct::Union_WithStaticDataInt>::value);
  EXPECT_TRUE((HasData_X<InStruct::Union_WithStaticDataInt, MTS>::value));
  EXPECT_FALSE((HasData_X<InStruct::Union_WithStaticDataInt, MTN>::value));
  EXPECT_FALSE(HasData_X<InStruct::Union_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasData_X<InStruct::Union_WithStaticMethodVoidVoid>::value);

  /// Doesn't compile
//  EXPECT_FALSE(HasData_X<InStruct::Enum_WithData>::value);

  EXPECT_FALSE(HasData_X<int>::value);
  EXPECT_FALSE(HasData_X<int*>::value);
  EXPECT_FALSE(HasData_X<void>::value);
  EXPECT_FALSE(HasData_X<void(int)>::value);
  EXPECT_FALSE(HasData_X<void(*)(int)>::value);
  EXPECT_FALSE(HasData_X<std::vector<int> >::value);
  EXPECT_FALSE((HasData_X<std::is_same<int, double> >::value));
  EXPECT_FALSE((HasData_X<std::pair<int, double> >::value));

  EXPECT_TRUE((HasData_value<std::is_same<int, double> >::value));
  EXPECT_TRUE((HasData_value<std::is_same<int, int>, MTS>::value));
  EXPECT_FALSE((HasData_value<std::is_same<int, int>, MTN>::value));
  EXPECT_TRUE((HasData_first<std::pair<int, double> >::value));
  EXPECT_FALSE((HasData_first<std::pair<int, double>, MTS>::value));
  EXPECT_TRUE((HasData_first<std::pair<int, double>, MTN>::value));

  /// Test constraints
  EXPECT_TRUE((HasData_X<Struct_WithNonstaticDataInt, MTA, std::is_integral>::value));
  EXPECT_FALSE((HasData_X<Struct_WithNonstaticDataInt, MTS, std::is_integral>::value));
  EXPECT_TRUE((HasData_X<Struct_WithNonstaticDataInt, MTN, std::is_integral>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTA, std::is_integral>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTS, std::is_integral>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTN, std::is_integral>::value));
  EXPECT_FALSE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTA, std::is_function>::value));
  EXPECT_TRUE((HasData_X<Struct_WithStaticDataPFuncIntInt, MTA, std::is_pointer>::value));
  EXPECT_FALSE((HasData_X<void*, MTA, std::is_class>::value));

  /// Test for possible name clashes
  EXPECT_FALSE(HasData_X<struct X>::value);
  EXPECT_FALSE(HasData_X<Subject>::value);
}


MIN_INTROSPECTION_DEFINE_HAS_FUNCTION(X);
MIN_INTROSPECTION_DEFINE_HAS_FUNCTION(begin);
//MIN_INTROSPECTION_DEFINE_HAS_FUNCTION(SomeStaticFunctionInStlClass);
TEST(TestMinbaseIntrospection, HasFunction) {
//  using MTA = min::introspection::MemberTraitAnystatic;
  using MTS = min::introspection::MemberTraitStatic;
  using MTN = min::introspection::MemberTraitNonstatic;

  /// TODO: Check const-ness ?

  EXPECT_FALSE(HasFunction_X<Struct_Undefined>::value);
  EXPECT_FALSE(HasFunction_X<Struct_Nothing>::value);

  EXPECT_FALSE(HasFunction_X<Struct_WithTypedefInt32>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithStructUndefined>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithStructDefined>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithUnion>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithEnum>::value);

  EXPECT_FALSE(HasFunction_X<Struct_WithNonstaticDataInt>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithStaticDataInt>::value);

  EXPECT_FALSE(HasFunction_X<Struct_WithAnonUnionDataInt>::value);
  EXPECT_FALSE(HasFunction_X<Struct_WithAnonEnumData>::value);

  EXPECT_TRUE(HasFunction_X<Struct_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodVoidVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPInt, int*>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodVoidPInt, MTS, int*>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPConstInt, MTN, int const*>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPConstInt, int const*>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodVoidPConstInt, MTS, int const*>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPInt, MTN, int*>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPFuncIntInt, int(*)(int)>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodVoidPFuncIntInt, MTS, int(*)(int)>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPFuncIntInt, MTN, int(*)(int)>::value));
  EXPECT_TRUE(HasFunction_X<Struct_WithNonstaticMethodIntVoid>::value);
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodIntVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<Struct_WithStaticMethodVoidVoid>::value);
  EXPECT_TRUE((HasFunction_X<Struct_WithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithStaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithStaticMethodVoidIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithStaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithStaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<Struct_WithStaticMethodIntVoid>::value);
  EXPECT_TRUE((HasFunction_X<Struct_WithStaticMethodIntVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithStaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithStaticMethodIntIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithStaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithStaticMethodIntIntDouble, MTN, int, double>::value));

//  EXPECT_TRUE(HasFunction_X<Struct_WithNonstaticMethodVoidVoidConst>::value);

  EXPECT_TRUE(HasFunction_X<Struct_DerivedWithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidPFuncIntInt, int(*)(int)>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidPFuncIntInt, MTS, int(*)(int)>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodVoidPFuncIntInt, MTN, int(*)(int)>::value));
  EXPECT_TRUE(HasFunction_X<Struct_DerivedWithNonstaticMethodIntVoid>::value);
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithNonstaticMethodIntVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodIntIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithNonstaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithNonstaticMethodIntIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<Struct_DerivedWithStaticMethodVoidVoid>::value);
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithStaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithStaticMethodVoidIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithStaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithStaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<Struct_DerivedWithStaticMethodIntVoid>::value);
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithStaticMethodIntVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithStaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithStaticMethodIntIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_DerivedWithStaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<Struct_DerivedWithStaticMethodIntIntDouble, MTN, int, double>::value));

  EXPECT_FALSE(HasFunction_X<Class_PrivateNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasFunction_X<Class_PrivateStaticMethodVoidVoid>::value);

  EXPECT_TRUE(HasFunction_X<Union_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasFunction_X<Union_WithNonstaticMethodVoidVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<Union_WithNonstaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE(HasFunction_X<Union_WithStaticMethodVoidVoid>::value);
  EXPECT_TRUE((HasFunction_X<Union_WithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<Union_WithStaticMethodVoidVoid, MTN>::value));

  /// Doesn't compile
//  EXPECT_FALSE(HasFunction_X<Enum_WithData>::value);

  EXPECT_FALSE(HasFunction_X<InStruct::Struct_Undefined>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_Nothing>::value);

  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithTypedefInt32>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithStructUndefined>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithStructDefined>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithUnion>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithEnum>::value);

  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithNonstaticDataInt>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithNonstaticDataPFuncIntInt>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithStaticDataInt>::value);

  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithAnonUnionDataInt>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Struct_WithAnonEnumData>::value);

  EXPECT_TRUE(HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPInt, int*>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPInt, MTS, int*>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPConstInt, MTN, int const*>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPConstInt, int const*>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPConstInt, MTS, int const*>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPFuncIntInt, int(*)(int)>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPFuncIntInt, MTS, int(*)(int)>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidPFuncIntInt, MTN, int(*)(int)>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Struct_WithNonstaticMethodIntVoid>::value);
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodIntVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodIntIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithNonstaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithNonstaticMethodIntIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Struct_WithStaticMethodVoidVoid>::value);
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithStaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithStaticMethodVoidIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithStaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithStaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Struct_WithStaticMethodIntVoid>::value);
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithStaticMethodIntVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithStaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithStaticMethodIntIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_WithStaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_WithStaticMethodIntIntDouble, MTN, int, double>::value));

//  EXPECT_TRUE(HasFunction_X<InStruct::Struct_WithNonstaticMethodVoidVoidConst>::value);

  EXPECT_TRUE(HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidPFuncIntInt, int(*)(int)>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidPFuncIntInt, MTS, int(*)(int)>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodVoidPFuncIntInt, MTN, int(*)(int)>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodIntVoid>::value);
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodIntVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodIntIntDouble, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithNonstaticMethodIntIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Struct_DerivedWithStaticMethodVoidVoid>::value);
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodVoidIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodVoidIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodVoidIntDouble, MTN, int, double>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Struct_DerivedWithStaticMethodIntVoid>::value);
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodIntVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodIntVoid, MTN>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodIntIntDouble, int, double>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodIntIntDouble, MTS, int, double>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Struct_DerivedWithStaticMethodIntIntDouble, MTN, int, double>::value));

  EXPECT_FALSE(HasFunction_X<InStruct::Class_PrivateNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE(HasFunction_X<InStruct::Class_PrivateStaticMethodVoidVoid>::value);

  EXPECT_TRUE(HasFunction_X<InStruct::Union_WithNonstaticMethodVoidVoid>::value);
  EXPECT_FALSE((HasFunction_X<InStruct::Union_WithNonstaticMethodVoidVoid, MTS>::value));
  EXPECT_TRUE((HasFunction_X<InStruct::Union_WithNonstaticMethodVoidVoid, MTN>::value));
  EXPECT_TRUE(HasFunction_X<InStruct::Union_WithStaticMethodVoidVoid>::value);
  EXPECT_TRUE((HasFunction_X<InStruct::Union_WithStaticMethodVoidVoid, MTS>::value));
  EXPECT_FALSE((HasFunction_X<InStruct::Union_WithStaticMethodVoidVoid, MTN>::value));

  /// Doesn't compile
//  EXPECT_FALSE(HasFunction_X<InStruct::Enum_WithData>::value);

  EXPECT_FALSE(HasFunction_X<int>::value);
  EXPECT_FALSE(HasFunction_X<int*>::value);
  EXPECT_FALSE(HasFunction_X<void>::value);
  EXPECT_FALSE(HasFunction_X<void(int)>::value);
  EXPECT_FALSE(HasFunction_X<void(*)(int)>::value);
  EXPECT_FALSE(HasFunction_X<std::vector<int> >::value);
  EXPECT_FALSE((HasFunction_X<std::is_same<int, double> >::value));
  EXPECT_FALSE((HasFunction_X<std::pair<int, double> >::value));

  EXPECT_TRUE((HasFunction_begin<std::vector<int> >::value));
  EXPECT_FALSE((HasFunction_begin<std::vector<int>, MTS>::value));
  EXPECT_TRUE((HasFunction_begin<std::vector<int>, MTN>::value));

  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodIntVoid, int>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithStaticMethodIntVoid, int>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, int>::value));
  EXPECT_FALSE((HasFunction_X<Struct_WithStaticMethodIntIntDouble, int>::value));

  EXPECT_FALSE((HasFunction_X<Struct_WithNonstaticMethodVoidPInt, int const*>::value));

  /// Implicitly convertible argument types result in true
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, int8_t, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, uint32_t, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, uint64_t, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, double, double>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodIntIntDouble, int, int>::value));
  EXPECT_TRUE((HasFunction_X<Struct_WithNonstaticMethodVoidPConstInt, int*>::value));


  /// Test for possible name clashes
  EXPECT_FALSE(HasFunction_X<struct X>::value);
  EXPECT_FALSE(HasFunction_X<Subject>::value);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
