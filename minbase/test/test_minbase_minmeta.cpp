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

#include <gtest/gtest.h>

#include <minbase/minmeta.hpp>


/// TODO: Remove this include
#include <iostream>


using namespace minmeta::simpler;



/// TODO: Create better tests when the structure is clear

template<class, class, class, class, class, class, class, class> struct Tpl8 {};

typedef Tpl8<
    uint8_t, int32_t, real32_t, void,
    uint8_t, real32_t, int32_t, int32_t> FullySpec;

typedef Curry<Tpl8, TypeList<uint8_t, int32_t, real32_t, void, uint8_t> > Spec1;
typedef Curry<Spec1::template tpl, TypeTag<real32_t> > Spec2;

TEST(TestMinbaseMinmeta, TestTest) {
  EXPECT_TRUE((std::is_same<
      TypeTag<int>::type, Head<TypeList<int> >::type>::value));
  EXPECT_TRUE((std::is_same<
      TypeTag<int>::type, Head<TypeList<int, double> >::type>::value));
  EXPECT_TRUE((std::is_same<
      Tail<TypeList<int, double, uint8_t> >::type,
      PushFront<TypeList<uint8_t>, double>::type>::value));
  EXPECT_EQ(2, (Length<TypeList<int, double> >::value));
  EXPECT_EQ(0, (Length<TypeList<> >::value));

  EXPECT_TRUE((IsAppliable<std::is_same, TypeList<int, int> >::value));
  EXPECT_TRUE((IsAppliable<std::is_same, TypeList<int, double> >::value));
  EXPECT_FALSE((IsAppliable<std::is_same, TypeList<int, int, int> >::value));

  EXPECT_TRUE((std::is_same<
      std::is_same<int, double>,
      Curry<std::is_same, TypeTag<int> >::template tpl<double> >::value));

  ASSERT_TRUE((IsAppliable<
      Spec2::template tpl, TypeList<int32_t, int32_t> >::value));
  ASSERT_FALSE((IsAppliable<
      Spec2::template tpl, TypeList<real32_t, int32_t, int32_t> >::value));
  ASSERT_FALSE((IsAppliable<
      Spec2::template tpl, TypeList<int32_t> >::value));
  EXPECT_TRUE((std::is_same<
      FullySpec, Spec2::template tpl<int32_t, int32_t> >::value));
  EXPECT_FALSE((std::is_same<
      FullySpec, Spec2::template tpl<uint32_t, int32_t> >::value));
  /// This specific form gives obscure error message
//  EXPECT_TRUE((std::is_same<
//      FullySpec, Spec2::template tpl<real32_t, int32_t, int32_t> >::value));
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
