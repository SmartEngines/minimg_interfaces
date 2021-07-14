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
#include <minbase/minresult.h>
#include <minimgapi/minimgapi.h>
#include <minimgapi/minimgapi-inl.h>
#include <minimgapi/imgguard.hpp>

TEST(TestMinimgapi, TestCompareMinImages) {
  uint8_t data_a[14], data_b[15];
  for (int i = 0; i < 14; ++i)
    data_a[i] = rand() & 0xFFU;
  ::memcpy(data_b, data_a, 14);
  DECLARE_GUARDED_MINIMG(image_a);
  DECLARE_GUARDED_MINIMG(image_b);
  ASSERT_EQ(NO_ERRORS, _WrapAlignedBufferWithMinImage(&image_a, data_a,
                                                      2, 2, 3, TYP_UINT8, 8));
  EXPECT_GT(CompareMinImages(&image_a, &image_b), 0);
  ASSERT_EQ(NO_ERRORS, _WrapAlignedBufferWithMinImage(&image_b, data_b,
                                                      2, 2, 3, TYP_UINT8, 7));
  EXPECT_GT(CompareMinImages(&image_a, &image_b), 0);
  ASSERT_EQ(NO_ERRORS, CopyMinImage(&image_b, &image_a));
  EXPECT_EQ(NO_ERRORS, CompareMinImages(&image_a, &image_b));
  ++data_b[11];
  EXPECT_GT(CompareMinImages(&image_a, &image_b), 0);
}

TEST(TestMinimgapi, TestCopyMinImageFragment) {
  DECLARE_GUARDED_MINIMG(dst_image);
  DECLARE_GUARDED_MINIMG(src_image);
  EXPECT_EQ(NO_ERRORS, NewMinImagePrototype(&dst_image, 100, 70, 3, TYP_UINT8));
  EXPECT_EQ(NO_ERRORS, NewMinImagePrototype(&src_image, 200, 60, 3, TYP_UINT8));
  EXPECT_EQ(NO_ERRORS, CopyMinImageFragment(&dst_image, &src_image,
                                                         7, 23, 55, 14, 3, 18));
}

TEST(TestMinimgapi, TestCheckMinImagesTangle) {
  MinImg src = {};
  MinImg dst = {};
  uint32_t result = 0;
  EXPECT_EQ(NO_ERRORS, CheckMinImagesTangle(&result, &src, &dst));
  EXPECT_EQ(TCR_SAME_IMAGE, result);

  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&src, 13, 100, 1, TYP_UINT8, 0, AO_EMPTY));
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&dst, 13, 100, 1, TYP_UINT8, 0, AO_EMPTY));

  EXPECT_EQ(BAD_ARGS, CheckMinImagesTangle(&result, &src, &dst));
  src.stride = dst.stride = dst.width;

  uint8_t *original_p_zero_line = reinterpret_cast<uint8_t *>(0x4000000);
  src.p_zero_line = original_p_zero_line;
  dst.p_zero_line = original_p_zero_line + dst.stride;

  EXPECT_EQ(NO_ERRORS, CheckMinImagesTangle(&result, &src, &dst));
  EXPECT_EQ(static_cast<uint32_t>(TCR_FORWARD_PASS_POSSIBLE |
                                  TCR_INDEPENDENT_LINES), result);

  EXPECT_EQ(NO_ERRORS, CheckMinImagesTangle(&result, &dst, &src));
  EXPECT_EQ(TCR_TANGLED_IMAGES, result);

  src.p_zero_line = original_p_zero_line + 1;
  src.stride = 14;
  dst.p_zero_line = original_p_zero_line;
  src.stride = 14;

  EXPECT_EQ(NO_ERRORS, CheckMinImagesTangle(&result, &dst, &src));
  EXPECT_EQ(TCR_FORWARD_PASS_POSSIBLE, result);
}

TEST(TestMinimgapi, TestGetMinImgLine) {
  if (sizeof(void*) == 8) {
    MinImg src = {};
    ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&src, 1, 512, 1, TYP_UINT8, 0, AO_EMPTY));
    src.p_zero_line = reinterpret_cast<uint8_t *>(0x4000000);
    src.stride = std::numeric_limits<int32_t>::max();
    EXPECT_EQ(src.p_zero_line, GetMinImageLine(&src, 0));
    EXPECT_EQ(src.p_zero_line + static_cast<size_t>(src.stride) * 511, GetMinImageLine(&src, 511));
    EXPECT_EQ(src.p_zero_line + static_cast<size_t>(src.stride) * 511, GetMinImageLine(&src, 511, BO_REPEAT));
    EXPECT_EQ(src.p_zero_line + static_cast<size_t>(src.stride) * 512, GetMinImageLine(&src, 512, BO_IGNORE));
    src.width = 0;
    EXPECT_EQ(src.p_zero_line + static_cast<size_t>(src.stride) * 512, GetMinImageLine(&src, 512, BO_IGNORE));
  }
}

TEST(TestMinimgapi, TestCopyMinImageChannels34) {
  DECLARE_GUARDED_MINIMG(dst);
  DECLARE_GUARDED_MINIMG(src);
  // 1200x589 - Size matters!
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&dst, 1200, 589, 4, TYP_UINT8));
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&src, 1200, 589, 3, TYP_UINT8));
  ASSERT_EQ(NO_ERRORS, ZeroFillMinImage(&dst));
  const uint8_t fill = 0xF0U;
  ASSERT_EQ(NO_ERRORS, FillMinImage(&src, &fill, sizeof(fill)));
  const int channels[] = { 0, 1, 2 };
  ASSERT_EQ(NO_ERRORS, CopyMinImageChannels(&dst, &src, channels, channels, 3));
  for (int y = 0; y < dst.height; ++y) {
    const uint8_t *px = dst.p_zero_line + dst.stride * y;
    for (int x = 0; x < dst.width; ++x, px +=4) {
      ASSERT_TRUE(
        px[0] == fill && px[1] == fill && px[2] == fill && px[3] == 0);
    }
  }
}

TEST(TestMinimgapi, TestCopyMinImageChannels43) {
  DECLARE_GUARDED_MINIMG(dst);
  DECLARE_GUARDED_MINIMG(src);
  // 1200x589 - Size matters!
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&dst, 1200, 589, 3, TYP_UINT8));
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&src, 1200, 589, 4, TYP_UINT8));
  ASSERT_EQ(NO_ERRORS, ZeroFillMinImage(&dst));
  const uint8_t fill = 0xF1U;
  ASSERT_EQ(NO_ERRORS, FillMinImage(&src, &fill, sizeof(fill)));
  const int channels[] = { 0, 1, 2 };
  ASSERT_EQ(NO_ERRORS, CopyMinImageChannels(&dst, &src, channels, channels, 3));
  for (int y = 0; y < dst.height; ++y) {
    const uint8_t *px = dst.p_zero_line + dst.stride * y;
    for (int x = 0; x < dst.width; ++x, px += 3) {
      ASSERT_TRUE(px[0] == fill && px[1] == fill && px[2] == fill);
    }
  }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
