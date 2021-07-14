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

static void test_jpeg_quality(const MinImg &original_img, const int quality, const int err)
{
  TestBinaryStream stream;
  const ExtImgProps props = {
    IFF_JPEG, IFC_NONE, 0.f, 0.f, quality
  };
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, original_img, IFF_JPEG, &props));
  DECLARE_GUARDED_MINIMG(loaded_image);
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &original_img));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));
  uint8_t the_diff = 0;
  quantile_diff(&the_diff, loaded_image, original_img);
  ASSERT_LE(the_diff, err);
}

static void test_jpeg(const MinImg &original_img)
{
  test_jpeg_quality(original_img, 100, 2);
  test_jpeg_quality(original_img, 75, 3);
  test_jpeg_quality(original_img, 50, 5);
  test_jpeg_quality(original_img, 25, 7);
  test_jpeg_quality(original_img, 1, 30);
}

char *images_dir = NULL;

template<typename T>
static void test_jpeg(int const channels)
{
  DECLARE_GUARDED_MINIMG(original_img);
  create_test_image<T>(original_img, channels);
  test_jpeg(original_img);
}

TEST(TestMinimgio, jpeg_grayscale) {
  test_jpeg<uint8_t>(1);
}

TEST(TestMinimgio, jpeg_rgb) {
  test_jpeg<uint8_t>(3);
}

TEST(TestMinimgio, tiff_props) {
  minimgio_test_props(IFF_JPEG);
}

static void _test_orientation(
  std::string fn,
  int scale = 16,
  int channels = 1,
  int alignment = 16) {
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

TEST(TestMinimgio, tiff_jpeg_orientation_1) {
  _test_orientation("orientation_1.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_2) {
  _test_orientation("orientation_2.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_3) {
  _test_orientation("orientation 3.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_4) {
  _test_orientation("orientation_4.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_5) {
  _test_orientation("orientation_5.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_6) {
  _test_orientation("orientation_6.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_7) {
  _test_orientation("orientation_7.jpg");
}
TEST(TestMinimgio, tiff_jpeg_orientation_8) {
  _test_orientation("orientation_8.jpg");
}

TEST(TestMinimgio, tiff_jpeg_focus) {
  SKIP_IF(!images_dir);
  DECLARE_GUARDED_MINIMG(loaded_image);
  ExtImgProps props = {};
  std::string const fn = std::string(images_dir) + "exif-canon-ixus.jpg";
  minimgio::BinaryFileStream stream(fn.c_str());
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream, &props));
  ASSERT_EQ(180, props.xDPI);
  ASSERT_EQ(180, props.yDPI);
  ASSERT_EQ(346U, props.focal_length_num);
  ASSERT_EQ(65546U, props.focal_length_den);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  if (argc == 2)
    images_dir = argv[1];
  return RUN_ALL_TESTS();
}
