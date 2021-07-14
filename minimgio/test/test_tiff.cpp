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

#include "common.hpp"
#include "quantile_diff.hpp"
#include <vector>

static void test_tiff(MinImg const& original_img)
{
  TestBinaryStream stream;
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, original_img, IFF_TIFF));
  DECLARE_GUARDED_MINIMG(loaded_image);
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &original_img));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &original_img));
  if (original_img.channels == 2)  // to not check for all possible channels
  {  // test `page` support
    const int page = 1;
    MinImg original_img_region = {};
    ASSERT_EQ(NO_ERRORS, GetMinImageRegion(
      &original_img_region, &original_img, 0, 0, 8, 9));
    ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, original_img_region, IFF_TIFF, nullptr, page));
    DECLARE_GUARDED_MINIMG(loaded_image2);
    ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image2, stream, nullptr, page));
    ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image2, &original_img_region));
    ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image2));
    ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image2, stream, page));
    ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image2, &original_img_region));
  }

  if (GetMinImageType(&original_img) == TYP_UINT1)
  { // check that user can convert to uint8_t
    DECLARE_GUARDED_MINIMG(loaded_image);
    ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
    ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &original_img));
    ASSERT_EQ(NO_ERRORS, SetMinImageType(&loaded_image, TYP_UINT8));
    ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
    ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));

    DECLARE_GUARDED_MINIMG(unpacked_img);
    ASSERT_EQ(NO_ERRORS, CloneRetypifiedMinImagePrototype(
      &unpacked_img, &original_img, TYP_UINT8));
    ASSERT_EQ(MR_SUCCESS, minimgio::Unpack(unpacked_img, original_img));
    ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &unpacked_img));
  }
}

template<typename T>
static void test_tiff(int const channels)
{
  DECLARE_GUARDED_MINIMG(original_img);
  create_test_image<T>(original_img, channels);
  test_tiff(original_img);
}

#define MINIMGIO_TYPE_CHECK(type) \
TEST(TestMinimgio, tiff_##type) { \
  test_tiff<type>(1);        \
  test_tiff<type>(2);        \
  test_tiff<type>(3);        \
  test_tiff<type>(4);        \
  test_tiff<type>(17);       \
}

MINIMGIO_TYPE_CHECK(bool);
MINIMGIO_TYPE_CHECK(uint8_t);
MINIMGIO_TYPE_CHECK(int8_t);
MINIMGIO_TYPE_CHECK(uint16_t);
MINIMGIO_TYPE_CHECK(int16_t);
MINIMGIO_TYPE_CHECK(uint32_t);
MINIMGIO_TYPE_CHECK(int32_t);
MINIMGIO_TYPE_CHECK(real32_t);
MINIMGIO_TYPE_CHECK(uint64_t);
MINIMGIO_TYPE_CHECK(int64_t);
MINIMGIO_TYPE_CHECK(real64_t);

TEST(TestMinimgio, tiff_props) {
  minimgio_test_props(IFF_TIFF);
}

static void _tiff_compression(MinImg const& img, ImgFileComp comp)
{
  TestBinaryStream stream;
  ExtImgProps props = {};
  props.iff = IFF_TIFF;
  props.comp = comp;
  props.qty = 100;
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, img, IFF_TIFF, &props));
  DECLARE_GUARDED_MINIMG(loaded_image);
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &img));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));
  if (comp == IFC_JPEG) {
    uint8_t the_diff;
    quantile_diff(&the_diff, loaded_image, img);
    ASSERT_LE(the_diff, 3);
  } else {
    ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &img));
  }
}

template<typename T>
static void _tiff_compression(ImgFileComp comp, int channels)
{
  DECLARE_GUARDED_MINIMG(original_img);
  create_test_image<T>(original_img, channels);
  _tiff_compression(original_img, comp);
}

#define MINIMGIO_COMP_TEST(COMP, TYPE, CHANNELS) \
TEST(TestMinimgio, tiff_compression_##COMP) {    \
  _tiff_compression<TYPE>(IFC_##COMP, CHANNELS); \
}                                                \

MINIMGIO_COMP_TEST(NONE, uint8_t, 2);
#ifdef WITH_TIFF_LZW
MINIMGIO_COMP_TEST(LZW, uint8_t, 2);
#endif
#ifdef WITH_TIFF_ZIP
MINIMGIO_COMP_TEST(DEFLATE, uint8_t, 2);
#endif
MINIMGIO_COMP_TEST(PACKBITS, uint8_t, 2);
#ifdef WITH_TIFF_JPEG
MINIMGIO_COMP_TEST(JPEG, uint8_t, 2);
#endif
// Explanation: RLE, GROUP3 and GROUP4 are buggy when channels != 1
MINIMGIO_COMP_TEST(RLE, bool, 1);
MINIMGIO_COMP_TEST(GROUP3, bool, 1);
MINIMGIO_COMP_TEST(GROUP4, bool, 1);

char *images_dir = NULL;

static void _test_orientation(
    std::string fn,
    int scale=15,
    int channels=1,
    int alignment=16) {
  SKIP_IF(!images_dir);
  DECLARE_GUARDED_MINIMG(loaded_image);
  fn = images_dir + fn;
  minimgio::BinaryFileStream stream(fn.c_str());
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image, alignment));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));
  DECLARE_GUARDED_MINIMG(expected_img);
  ASSERT_EQ(NO_ERRORS, create_f_image(&expected_img, scale, channels));
  ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &expected_img));
}

TEST(TestMinimgio, tiff_orientation_1) {
  _test_orientation("orientation_1.tif");
}
TEST(TestMinimgio, tiff_orientation_2) {
  _test_orientation("orientation_2.tif");
}
TEST(TestMinimgio, tiff_orientation_3) {
  _test_orientation("orientation_3.tif");
}
TEST(TestMinimgio, tiff_orientation_4) {
  _test_orientation("orientation_4.tif");
}
TEST(TestMinimgio, tiff_orientation_5) {
  _test_orientation("orientation_5.tif");
}
TEST(TestMinimgio, tiff_orientation_6) {
  _test_orientation("orientation_6.tif");
}
TEST(TestMinimgio, tiff_orientation_7) {
  _test_orientation("orientation_7.tif");
}
TEST(TestMinimgio, tiff_orientation_8) {
  _test_orientation("orientation_8.tif");
}
TEST(TestMinimgio, tiff_orientation_YCbCr_1) {
  _test_orientation("orientation_YCbCr_1.tif", 16, 4);
  _test_orientation("orientation_YCbCr_1.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_2) {
  _test_orientation("orientation_YCbCr_2.tif", 16, 4);
  _test_orientation("orientation_YCbCr_2.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_3) {
  _test_orientation("orientation_YCbCr_3.tif", 16, 4);
  _test_orientation("orientation_YCbCr_3.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_4) {
  _test_orientation("orientation_YCbCr_4.tif", 16, 4);
  _test_orientation("orientation_YCbCr_4.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_5) {
  _test_orientation("orientation_YCbCr_5.tif", 16, 4);
  _test_orientation("orientation_YCbCr_5.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_6) {
  _test_orientation("orientation_YCbCr_6.tif", 16, 4);
  _test_orientation("orientation_YCbCr_6.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_7) {
  _test_orientation("orientation_YCbCr_7.tif", 16, 4);
  _test_orientation("orientation_YCbCr_7.tif", 16, 4, 128);
}
TEST(TestMinimgio, tiff_orientation_YCbCr_8) {
  _test_orientation("orientation_YCbCr_8.tif", 16, 4);
  _test_orientation("orientation_YCbCr_8.tif", 16, 4, 128);
}

TEST(TestMinimgio, tiff_memory_scheme) {
  DECLARE_GUARDED_MINIMG(original_img);
  create_test_image<uint8_t>(original_img, 1, 100, 100);
  std::vector<uint8_t> memory;
  VectorBinaryStream stream(memory);
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, original_img, IFF_TIFF));
  minimgio::BinaryMemoryReadonlyStream mem_stream(&memory[0], memory.size());
  DECLARE_GUARDED_MINIMG(loaded_img);
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_img, mem_stream));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_img));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_img, mem_stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImages(&original_img, &loaded_img));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  if (argc == 2)
    images_dir = argv[1];
  return RUN_ALL_TESTS();
}
