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


static void test_png(MinImg const& original_img)
{
  TestBinaryStream stream;
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, original_img, IFF_PNG));
  {
    DECLARE_GUARDED_MINIMG(loaded_image);
    ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
    ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &original_img));
    ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
    ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));
    ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &original_img));
  }
  if (original_img.channels == 2 || original_img.channels == 4)
  {  // check that user can strip alpha channel
    DECLARE_GUARDED_MINIMG(loaded_image);
    ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
    ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &original_img));
    loaded_image.channels -= 1;
    ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
    ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));

    DECLARE_GUARDED_MINIMG(stripped_img);
    ASSERT_EQ(NO_ERRORS, CloneDimensionedMinImagePrototype(
      &stripped_img, &original_img, original_img.channels - 1));
    const int channel_to_copy[] = { 0, 1, 2 };
    ASSERT_EQ(NO_ERRORS, CopyMinImageChannels(
      &stripped_img, &original_img, channel_to_copy, channel_to_copy,
      original_img.channels - 1));

    ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &stripped_img));
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
  if (GetMinImageType(&original_img) == TYP_UINT16)
  {  // check that user can convert to uint8_t
    DECLARE_GUARDED_MINIMG(loaded_image);
    ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
    ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &original_img));
    ASSERT_EQ(NO_ERRORS, SetMinImageType(&loaded_image, TYP_UINT8));
    ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
    ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));

    DECLARE_GUARDED_MINIMG(typified_img);
    ASSERT_EQ(NO_ERRORS, CloneRetypifiedMinImagePrototype(
      &typified_img, &original_img, TYP_UINT8));
    libpng_uint16_to_uint8(&typified_img, &original_img);
    ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &typified_img));
  }
}

template<typename T>
static void test_png(int const channels)
{
  DECLARE_GUARDED_MINIMG(original_img);
  create_test_image<T>(original_img, channels);
  test_png(original_img);
}

TEST(TestMinimgio, png_uint1_1ch) {
  test_png<bool>(1);
}

TEST(TestMinimgio, png_uint8_1ch) {
  test_png<uint8_t>(1);
}

TEST(TestMinimgio, png_uint8_2ch) {
  test_png<uint8_t>(2);
}

TEST(TestMinimgio, png_uint8_3ch) {
  test_png<uint8_t>(3);
}

TEST(TestMinimgio, png_uint8_4ch) {
  test_png<uint8_t>(4);
}

TEST(TestMinimgio, png_uint16_1ch) {
  test_png<uint16_t>(1);
}

TEST(TestMinimgio, png_uint16_2ch) {
  test_png<uint16_t>(2);
}

TEST(TestMinimgio, png_uint16_3ch) {
  test_png<uint16_t>(3);
}

TEST(TestMinimgio, png_uint16_4ch) {
  test_png<uint16_t>(4);
}

TEST(TestMinimgio, png_memory) {
  DECLARE_GUARDED_MINIMG(original_img);
  create_test_image<uint8_t>(original_img);

  //std::string const fn = std::string(std::tmpnam(NULL)) + ".png";
  //FileRemover file_remover(fn);
  std::vector<uint8_t> memory;
  VectorBinaryStream stream(memory);
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, original_img, IFF_PNG));
  DECLARE_GUARDED_MINIMG(in_memory_img);
  minimgio::BinaryMemoryReadonlyStream mem_stream(&memory[0], memory.size());

  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(in_memory_img, mem_stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&original_img, &in_memory_img));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&in_memory_img));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(in_memory_img, mem_stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImages(&in_memory_img, &original_img));
}

TEST(TestMinimgio, png_props) {
  minimgio_test_props(IFF_PNG);
}

TEST(TestMinimgio, png_check_zero_page) {
  TestBinaryStream stream;
  MinImg img{};
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, img, IFF_PNG, nullptr, 1));
}

TEST(TestMinimgio, png_check_empty_image) {
  TestBinaryStream stream;
  MinImg img{};
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, img, IFF_PNG, nullptr, 0));
  ASSERT_STREQ("Empty image", stream.get_error());
}

TEST(TestMinimgio, png_check_channels) {
  TestBinaryStream stream;
  DECLARE_GUARDED_MINIMG(img);
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&img, 16, 16, 5, TYP_UINT16));
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, img, IFF_PNG, nullptr, 0));
  ASSERT_STREQ("Invalid number of channels. png supports 1-4 channels", stream.get_error());
}

TEST(TestMinimgio, png_check_scalar_type) {
  TestBinaryStream stream;
  DECLARE_GUARDED_MINIMG(img);
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&img, 16, 16, 3, TYP_UINT32));
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, img, IFF_PNG, nullptr, 0));
  ASSERT_STREQ("Unsupported image format", stream.get_error());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
