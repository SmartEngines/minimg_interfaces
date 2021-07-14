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


static void check_interface(
  const bool is_supported,
  const ImgFileFormat iff)
{
  MinImg tmp = {};
  DECLARE_GUARDED_MINIMG(tmp2);
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&tmp2, 1, 1, 3, TYP_UINT8));

  TestBinaryStream stream;

  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::GetFileProps(tmp, stream));
  {
    MinImg empty = {};
    ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::GetFileProps(empty, stream));
  }
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Load(tmp, stream));
  int num_pages = -1;
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::GetNumPages(num_pages, stream));
  ASSERT_EQ(-1, num_pages);
  char page_name[256] = {};
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::GetPageName(page_name, sizeof(page_name), stream, 0));
  if (is_supported) {
    if (minimgio::Save(stream, tmp2, iff) == -3)
      ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, tmp2, iff));
  } else {
    ASSERT_EQ(MR_STRIPPED_VERSION, minimgio::Save(stream, tmp, iff));
  }
  //int num_pages = -1;
//  if (is_supported) {
//    ASSERT_EQ(FILE_ERROR, GetMinImageFileProps(&tmp, filename));
//    ASSERT_EQ(BAD_ARGS, LoadMinImage(NULL, filename));
//    ASSERT_EQ(FILE_ERROR, LoadMinImage(&tmp2, filename));
//    ASSERT_EQ(FILE_ERROR, GetMinImageFilePages(filename));
//  } else {
//    ASSERT_EQ(NOT_SUPPORTED, GetMinImageFileProps(&tmp, filename));
//    ASSERT_EQ(NOT_SUPPORTED, LoadMinImage(NULL, filename));
//    ASSERT_EQ(NOT_SUPPORTED, LoadMinImage(&tmp2, filename));
//    ASSERT_EQ(NOT_SUPPORTED, SaveMinImage(filename, &tmp2));
//  }
}

TEST(TestMinimgio, test_interface_common) {
  // MinImg tmp = {};
  // ASSERT_EQ(GetMinImageFileProps(NULL, NULL), MR_CONTRACT_VIOLATION);
  // ASSERT_EQ(GetMinImageFileProps(&tmp, "non_existing_file.tif"), FILE_ERROR);
  // ASSERT_EQ(LoadMinImage(NULL, NULL), MR_CONTRACT_VIOLATION);
  // ASSERT_EQ(LoadMinImage(&tmp, "non_existing_file.tif"), FILE_ERROR);
  // //int num_pages = -1;
  // ASSERT_EQ(GetMinImageFilePages(NULL), MR_CONTRACT_VIOLATION);
//  ASSERT_EQ(BAD_ARGS, GetMinImageFileProps(NULL, NULL));
//  ASSERT_EQ(FILE_ERROR, GetMinImageFileProps(&tmp, "non_existing_file.mp3"));
//  ASSERT_EQ(BAD_ARGS, LoadMinImage(NULL, NULL));
//  ASSERT_EQ(FILE_ERROR, LoadMinImage(&tmp, "non_existing_file.mp3"));
//  ASSERT_EQ(BAD_ARGS, GetMinImageFilePages(NULL));
}

TEST(TestMinimgio, test_interface_jpeg) {
#ifdef WITH_JPEG
  check_interface(true, IFF_JPEG);
#else
  check_interface(false, IFF_JPEG);
#endif
}

TEST(TestMinimgio, test_interface_png) {
#ifdef WITH_PNG
  check_interface(true, IFF_PNG);
#else
  check_interface(false, IFF_PNG);
#endif
}

TEST(TestMinimgio, test_interface_tiff) {
#ifdef WITH_TIFF
  check_interface(true, IFF_TIFF);
#else
  check_interface(false, IFF_TIFF);
#endif
}

TEST(TestMinimgio, test_interface_webp) {
#ifdef WITH_WEBP
  check_interface(true, IFF_WEBP);
#else
  check_interface(false, IFF_WEBP);
#endif
}

TEST(TestMinimgio, test_pack_and_unpack) {
  DECLARE_GUARDED_MINIMG(src);
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&src, 16, 16, 1, TYP_UINT8));
  ASSERT_EQ(NO_ERRORS, ZeroFillMinImage(&src));
  for (int idx = 0; idx < 16; ++idx)
    GetMinImageLine(&src, idx)[idx] = 0xFFU;
  DECLARE_GUARDED_MINIMG(dst_1bit);
  ASSERT_EQ(NO_ERRORS, CloneRetypifiedMinImagePrototype(&dst_1bit, &src, TYP_UINT1));
  ASSERT_EQ(MR_SUCCESS, minimgio::Pack(dst_1bit, src, 10));
  for (int y = 0; y < src.height; ++y)
  {
    uint8_t const* line = GetMinImageLine(&dst_1bit, y);
    for (int x = 0; x < src.width; ++x)
    {
      uint8_t const the_bit = 0x80 >> x % 8;
      bool const img_has_bit = (line[x / 8] & the_bit) != 0;
      ASSERT_EQ(x == y, img_has_bit);
    }
  }
  DECLARE_GUARDED_MINIMG(dst_8bit);
  ASSERT_EQ(NO_ERRORS, CloneMinImagePrototype(&dst_8bit, &src));
  ASSERT_EQ(MR_SUCCESS, minimgio::Unpack(dst_8bit, dst_1bit));
  ASSERT_EQ(NO_ERRORS, CompareMinImages(&dst_8bit, &src));
}


TEST(TestMinimgio, load_dir) {
  // this tests exists because minimgio hanged on opening directories
  DECLARE_GUARDED_MINIMG(loaded_image);
  minimgio::BinaryFileStream stream(".");
  ASSERT_EQ(MR_ENV_ERROR, minimgio::GetFileProps(loaded_image, stream));
  int num_pages = -1;
  ASSERT_EQ(MR_ENV_ERROR, minimgio::GetNumPages(num_pages, stream));
  ASSERT_EQ(-1, num_pages);
//  ASSERT_EQ(FILE_ERROR, GetMinImageFileProps(&loaded_image, "."));
//  ASSERT_EQ(FILE_ERROR, GetMinImageFilePages("."));
}


TEST(TestMinimgio, lst_pages) {
  static char const ref_names[8][20] = {
    "orientation_1.jpg",
    "orientation_2.jpg",
    "orientation 3.jpg",
    "orientation_4.jpg",
    "orientation_5.jpg",
    "orientation_6.jpg",
    "orientation_7.jpg",
    "orientation_8.jpg"
  };
  const char data[] =
    "orientation_1.jpg\n"
    " orientation_2.jpg\n"
    "orientation 3.jpg \n"
    "orientation_4.jpg\t"
    "\n"
    "orientation_5.jpg\n"
    "\torientation_6.jpg\n"
    "\n"
    "orientation_7.jpg\n"
    "orientation_8.jpg\n"
    ";orientation_1.tif\n";

  int total_pages = -1;
  TestBinaryStream stream;
  stream.write(data, sizeof(data) - 1);

  ASSERT_EQ(MR_SUCCESS, minimgio::GetNumPages(total_pages, stream));
  ASSERT_EQ(8, total_pages);
  for (int page_no = 0; page_no < total_pages; ++page_no) {
    char page_name[256] = {};
    ASSERT_EQ(MR_SUCCESS, minimgio::GetPageName(page_name, sizeof(page_name), stream, page_no));
    ASSERT_STREQ(ref_names[page_no], page_name);
  }
}

TEST(TestMinimgio, lst_pages_tiny) {
  TestBinaryStream stream;
  const char data[] = "a.jpg";
  stream.write(data, sizeof(data) - 1);
  int total_pages = -1;
  ASSERT_EQ(MR_SUCCESS, minimgio::GetNumPages(total_pages, stream));
  ASSERT_EQ(1, total_pages);
  char page_name[8] = {};
  ASSERT_EQ(MR_SUCCESS, minimgio::GetPageName(page_name, sizeof(page_name), stream, 0));
  ASSERT_STREQ("a.jpg", page_name);
}

TEST(TestMinimgio, lst_pages_file_format) {
  {
    TestBinaryStream stream;
    int total_pages = -1;
    ASSERT_EQ(MR_CONTRACT_VIOLATION , minimgio::GetNumPages(total_pages, stream));
    ASSERT_EQ(-1, total_pages);
  }
  {
    TestBinaryStream stream;
    const char data[] = "hello\x01";
    stream.write(data, sizeof(data) - 1);
    int total_pages = -1;
    ASSERT_EQ(MR_CONTRACT_VIOLATION , minimgio::GetNumPages(total_pages, stream));
    ASSERT_EQ(-1, total_pages);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
