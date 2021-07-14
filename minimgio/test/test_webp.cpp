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

TEST(TestMinimgio, webp_uint8_3ch) {
  DECLARE_GUARDED_MINIMG(test_image);
  create_test_image<uint8_t>(test_image, 3, 100, 100);

  TestBinaryStream stream;
  const ExtImgProps props = {
    IFF_WEBP, IFC_NONE, 0.f, 0.f, 100
  };
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, test_image, IFF_WEBP, &props));

  DECLARE_GUARDED_MINIMG(loaded_image);
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&loaded_image, &test_image));
  ASSERT_EQ(NO_ERRORS, AllocMinImage(&loaded_image));
  ASSERT_EQ(MR_SUCCESS, minimgio::Load(loaded_image, stream));
  ASSERT_EQ(NO_ERRORS, CompareMinImages(&loaded_image, &test_image));
  ASSERT_STREQ("", stream.get_error());
}

TEST(TestMinimgio, webp_invalid_channels) {
  DECLARE_GUARDED_MINIMG(test_image);
  TestBinaryStream stream;
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&test_image, 1, 1, 1, TYP_UINT8, 0, AO_PREALLOCATED));
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, test_image, IFF_WEBP, NULL));
  ASSERT_STREQ("not 3 or 4 channels", stream.get_error());
}

TEST(TestMinimgio, webp_invalid_type) {
  DECLARE_GUARDED_MINIMG(test_image);
  TestBinaryStream stream;
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&test_image, 1, 1, 3, TYP_UINT16, 0, AO_PREALLOCATED));
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, test_image, IFF_WEBP, NULL));
  ASSERT_STREQ("not uint8", stream.get_error());
}

TEST(TestMinimgio, webp_invalid_zero_line) {
  MinImg test_image = {};
  TestBinaryStream stream;
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&test_image, 1, 1, 3, TYP_UINT8, 0, AO_EMPTY));
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, test_image, IFF_WEBP, NULL));
  ASSERT_STREQ("", stream.get_error());
}

TEST(TestMinimgio, webp_invalid_size) {
  DECLARE_GUARDED_MINIMG(test_image);
  TestBinaryStream stream;
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(&test_image, 70000, 1, 3, TYP_UINT8, 0, AO_PREALLOCATED));
  ASSERT_EQ(MR_CONTRACT_VIOLATION, minimgio::Save(stream, test_image, IFF_WEBP, NULL));
  ASSERT_STREQ("picture has invalid width/height", stream.get_error());
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
