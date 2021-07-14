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

#include <vector>

#include <gtest/gtest.h>

#include <minbase/mintyp.h>
#include <minbase/min_pp_mintyp.h>


TEST(TestMinbaseMinTyp, OrderCorrespondence) {
  std::vector<MinTyp> formed_by_enum;
  for (int i = 0; i < MINTYP_COUNT; ++i)
    formed_by_enum.push_back(static_cast<MinTyp>(i));
  std::vector<MinTyp> formed_by_macro;
#define MIN_PP_ARG_PUSHBACK(ctype) \
  formed_by_macro.push_back(MIN_PP_MAP_ctype_MinTyp(ctype));
  MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_PUSHBACK);
#undef MIN_PP_ARG_PUSHBACK
  EXPECT_EQ(formed_by_macro, formed_by_enum);
}


TEST(TestMinbaseMinTyp, MinFormatOfMinType) {
#define MIN_PP_ARG_CHECK_MinFormatOfMinType(ctype)  \
  EXPECT_EQ(MIN_PP_MAP_ctype_MinFmt(ctype), \
      MinFormatOfMinType(MIN_PP_MAP_ctype_MinTyp(ctype)));

  MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_CHECK_MinFormatOfMinType);

#undef MIN_PP_ARG_CHECK_MinFormatOfMinType

  EXPECT_EQ(FMT_INVALID, MinFormatOfMinType(TYP_INVALID));
}


TEST(TestMinbaseMinTyp, LogBitSizeOfMinType) {
#define MIN_PP_ARG_CHECK_LogBitSizeOfMinType(ctype)  \
  EXPECT_EQ(MIN_PP_MAP_ctype_logbitsize(ctype), \
      LogBitSizeOfMinType(MIN_PP_MAP_ctype_MinTyp(ctype)));

  MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_CHECK_LogBitSizeOfMinType);

#undef MIN_PP_ARG_CHECK_LogBitSizeOfMinType

  EXPECT_EQ(-1, LogBitSizeOfMinType(TYP_INVALID));
}



/// TODO: Do something with "magical" numbers

TEST(TestMinbaseMinTyp, MinTypeByMinFormatAndLogSize) {
#define MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize(ctype)  \
  EXPECT_EQ(MIN_PP_MAP_ctype_MinTyp(ctype), MinTypeByMinFormatAndLogSize( \
      MIN_PP_MAP_ctype_MinFmt(ctype), MIN_PP_MAP_ctype_logbitsize(ctype)));

  MIN_PP_DO_FOR_ALL_TYPES(MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize);

#undef MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize

#define MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize_INVALID_MinFmt(ctype)  \
  EXPECT_EQ(TYP_INVALID, MinTypeByMinFormatAndLogSize(  \
      FMT_INVALID, MIN_PP_MAP_ctype_logbitsize(ctype)));

  MIN_PP_DO_FOR_ALL_TYPES(
      MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize_INVALID_MinFmt);

#undef MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize_INVALID_MinFmt

#define MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize_INVALID_depth(ctype)  \
  EXPECT_EQ(TYP_INVALID,  \
      MinTypeByMinFormatAndLogSize(MIN_PP_MAP_ctype_MinFmt(ctype), -1)); \
  EXPECT_EQ(TYP_INVALID,  \
      MinTypeByMinFormatAndLogSize(MIN_PP_MAP_ctype_MinFmt(ctype), 1)); \
  EXPECT_EQ(TYP_INVALID,  \
      MinTypeByMinFormatAndLogSize(MIN_PP_MAP_ctype_MinFmt(ctype), 2)); \
  EXPECT_EQ(TYP_INVALID,  \
      MinTypeByMinFormatAndLogSize(MIN_PP_MAP_ctype_MinFmt(ctype), 7)); \
  EXPECT_EQ(TYP_INVALID,  \
      MinTypeByMinFormatAndLogSize(MIN_PP_MAP_ctype_MinFmt(ctype), 0xDEADBEEF));

  MIN_PP_DO_FOR_ALL_TYPES(
      MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize_INVALID_depth);

#undef MIN_PP_ARG_CHECK_MinTypeByMinFormatAndLogSize_INVALID_depth

  EXPECT_EQ(TYP_INVALID, MinTypeByMinFormatAndLogSize(FMT_INVALID, -1));
  EXPECT_EQ(TYP_INVALID, MinTypeByMinFormatAndLogSize(FMT_INVALID, 1));
  EXPECT_EQ(TYP_INVALID, MinTypeByMinFormatAndLogSize(FMT_INVALID, 2));
  EXPECT_EQ(TYP_INVALID, MinTypeByMinFormatAndLogSize(FMT_INVALID, 7));
  EXPECT_EQ(TYP_INVALID, MinTypeByMinFormatAndLogSize(FMT_INVALID, 0xDEADBEEF));
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
