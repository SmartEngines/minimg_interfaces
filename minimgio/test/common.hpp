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
#include <minimgapi/imgguard.hpp>
#include <minimgapi/minimgapi-helpers.hpp>
#include <minimgio/minimgio.hpp>
#include <minutils/smartptr.h>
#include <limits>


// LETTERs for image
static const uint8_t LETTER_a[] = { 0x00, 0x00, 0xF8, 0x0C, 0x7C, 0xCC, 0xF6, 0x00 };
static const uint8_t LETTER_B[] = { 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00 };
static const uint8_t LETTER_C[] = { 0x7E, 0xC6, 0xC0, 0xC0, 0xC2, 0xC6, 0x7C, 0x00 };
static const uint8_t LETTER_d[] = { 0x1C, 0x0C, 0x6C, 0xDC, 0xCC, 0xCC, 0x76, 0x00 };
static const uint8_t LETTER_e[] = { 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0x7C, 0x00 };
static const uint8_t LETTER_l[] = { 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00 };
static const uint8_t LETTER_n[] = { 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0xEE, 0x00 };
static const uint8_t LETTER_o[] = { 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00 };
static const uint8_t LETTER_R[] = { 0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00 };
static const uint8_t LETTER_u[] = { 0x00, 0x00, 0xEE, 0x66, 0x66, 0x66, 0x3A, 0x00 };
static const uint8_t LETTER_w[] = { 0x00, 0x00, 0xE6, 0xC2, 0xDA, 0x6C, 0x64, 0x00 };
static const uint8_t LETTER_y[] = { 0x00, 0x00, 0xE6, 0x62, 0x34, 0x18, 0x08, 0x70 };
static const uint8_t LETTER_Y[] = { 0xE6, 0xC2, 0x6C, 0x68, 0x30, 0x30, 0x78, 0x00 };

template<typename T>
void draw_letters(const MinImg *img, const uint8_t* const *letters, const size_t n)
{
  for (int y = 0; y < 8; ++y) {
    T* line = GetMinImageLineAs<T>(img, y, BO_IGNORE);
    for (size_t letter = 0; letter < n; ++letter) {
      uint8_t chr = letters[letter][y];
      for (int x = 0; x < 8; ++x)
        if (chr & (0x80 >> x))
          for (int c = 0; c < img->channels; ++c)
            line[x * img->channels + c] = 0;
      line += 8 * img->channels;
    }
  }
}

#define SKIP_IF(CHECK) if (CHECK) { \
  std::cout << "[  SKIPPED ] " << #CHECK << "\n"; return;} \

static void libpng_uint16_to_uint8(MinImg const *dst, MinImg const *src)
{
  ASSERT_EQ(TYP_UINT16, GetMinImageType(src));
  ASSERT_EQ(TYP_UINT8, GetMinImageType(dst));
  ASSERT_EQ(NO_ERRORS, CompareMinImage3DSizes(dst, src));
  for (int y = 0; y < src->height; ++y)
  {
    const uint16_t *src_line = GetMinImageLineAs<uint16_t>(src, y);
    uint8_t* dst_line = GetMinImageLineAs<uint8_t>(dst, y);
    for (int x = 0; x < src->width * src->channels; ++x)
      dst_line[x] = static_cast<uint8_t>(src_line[x] >> 8);
  }
}

template<typename T>
STATIC_SPECIAL void create_test_image(
  MinImg &img, const int channels=3, const int width=600, const int height=300)
{
  const T min_val = std::numeric_limits<T>::min();
  const T max_val = std::numeric_limits<T>::max();
  const real32_t range =
    static_cast<real32_t>(max_val) - static_cast<real32_t>(min_val);

  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(
    &img, width, height, channels, GetMinTypByCType<T>()));
  scoped_cpp_array<T> R(new T[img.width]);
  for (int x = 0; x < width; ++x)
    R[x] = static_cast<T>(
      static_cast<real32_t>(x) / static_cast<real32_t>(width - 1) * range);
  for (int y = 0; y < height; ++y) {
    const T G = static_cast<T>((static_cast<real32_t>(y) / static_cast<real32_t>(height - 1)) * range);
    T *dst_line = GetMinImageLineAs<T>(&img, y);
    for (int x = 0; x < width; ++x)
    {
      T *pix = dst_line + x * channels;
      for (int c = 0; c < channels; ++c) {
        switch (c) {
        case 0:  pix[0] = R[x]; break;
        case 1:  pix[1] = G; break;
        case 2:  pix[2] = R[img.width - x - 1]; break;
        default: pix[c] = max_val;
        }
      }
    }
  }
  MinImg text_region = {};
  ASSERT_EQ(NO_ERRORS, GetMinImageRegion(&text_region, &img, 3, 3, 0, 8, RO_REUSE_CONTAINER));
  const uint8_t *blue[] = { LETTER_B, LETTER_l, LETTER_u, LETTER_e };
  draw_letters<T>(&text_region, blue, 4);
  ASSERT_EQ(NO_ERRORS, GetMinImageRegion(&text_region, &img, img.width - 8 * 3 - 3, 3, 0, 8, RO_REUSE_CONTAINER));
  const uint8_t *red[] = { LETTER_R, LETTER_e, LETTER_d};
  draw_letters<T>(&text_region, red, 3);
  ASSERT_EQ(NO_ERRORS, GetMinImageRegion(&text_region, &img, 1, img.height - 9, 0, 8, RO_REUSE_CONTAINER));
  const uint8_t *cyan[] = { LETTER_C, LETTER_y, LETTER_a, LETTER_n };
  draw_letters<T>(&text_region, cyan, 4);
  ASSERT_EQ(NO_ERRORS, GetMinImageRegion(&text_region, &img, img.width - 8 * 6 - 3, img.height - 8 - 3, 0, 8, RO_REUSE_CONTAINER));
  const uint8_t *yellow[] = { LETTER_Y, LETTER_e, LETTER_l, LETTER_l, LETTER_o, LETTER_w };
  draw_letters<T>(&text_region, yellow, 6);
}

template<>
STATIC_SPECIAL void create_test_image<bool>(
  MinImg &img, int const channels, int const width, int const height)
{
  DECLARE_GUARDED_MINIMG(original_img);
  ASSERT_EQ(NO_ERRORS, NewMinImagePrototype(
    &original_img, width, height, channels, TYP_UINT8));
  uint8_t bg = 0xff;
  FillMinImage(&original_img, &bg, sizeof(uint8_t));
  const uint8_t *road[] = { LETTER_R, LETTER_o, LETTER_a, LETTER_d };
  draw_letters<uint8_t>(&original_img, road, 4);
  ASSERT_EQ(NO_ERRORS, CloneRetypifiedMinImagePrototype(
    &img, &original_img, TYP_UINT1));
  ASSERT_EQ(MR_SUCCESS, minimgio::Pack(img, original_img));
}


// struct TempBinaryStream
//     : public minimgio::BinaryStreamErrorHandling {
//   TempBinaryStream() : f(nullptr) {}
//   ~TempBinaryStream() override{
//     std::fclose(this->f);
//   }
//   MinResult initialize(MISInitFlag) override {
//     if (!this->f)
//       this->f = std::tmpfile();
//     if (!this->f)
//       return MR_ENV_ERROR;
//     return MR_SUCCESS;
//   }
//   std::size_t read(void* buffer, std::size_t size) override {
//     return std::fread(buffer, sizeof(uint8_t), size, this->f);
//   }
//   std::size_t write(const void* buffer, std::size_t size) override {
//     return std::fwrite(buffer, sizeof(uint8_t), size, this->f);
//   }
//   int64_t lseek(int64_t offset, int whence) override {
//     if (std::fseek(this->f, offset, whence) != 0)
//       return -1;
//     return std::ftell(this->f);
//   };

// protected:
//   FILE *f;
// };

struct VectorBinaryStream
    : public minimgio::BinaryStreamErrorHandling {
  VectorBinaryStream(std::vector<uint8_t> &vec) : vec(vec), pos(0U) {}
  MinResult initialize(MISInitFlag) override {
    return MR_SUCCESS;
  }
  std::size_t read(void* buffer, std::size_t size) override {
    const std::size_t new_pos = std::min(this->vec.size(), this->pos + size);
    const std::size_t copy_sz = new_pos - this->pos;
    if (copy_sz)  // "this->vec[this->pos]" will throw assertation on msvc when vec.size is 0
      ::memcpy(buffer, &this->vec[this->pos], copy_sz);
    this->pos = new_pos;
    return copy_sz;
  }
  std::size_t write(const void* buffer, std::size_t size) override {
    const std::size_t new_pos = this->pos + size;
    if (new_pos > this->vec.size())
      this->vec.resize(new_pos);
    std::memcpy(&this->vec[this->pos], buffer, size);
    this->pos = new_pos;
    return size;
  }
  int64_t lseek(int64_t offset, int whence) override {
    switch(whence) {
      case SEEK_SET: this->pos = offset; break;
      case SEEK_CUR: this->pos += offset; break;
      case SEEK_END: this->pos = this->vec.size() + offset; break;
      default: return -1;
    }
    this->pos = std::max(size_t(0), this->pos);
    if (this->pos > this->vec.size())
      this->vec.resize(this->pos);
    return static_cast<int64_t>(this->pos);
  };

protected:
  std::vector<uint8_t> &vec;
  std::size_t pos;
};

struct TestBinaryStream : public VectorBinaryStream
{
  TestBinaryStream() : VectorBinaryStream(inner_vec) {}
protected:
  std::vector<uint8_t> inner_vec;
};

void minimgio_test_props(const ImgFileFormat iff) {
  DECLARE_GUARDED_MINIMG(img);
  create_test_image<uint8_t>(img);
  TestBinaryStream stream;
  MinImg prototype = {};

  const ExtImgProps save_props = {
    iff, IFC_NONE, 92.1f, 192.455f, 100
  };
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, img, iff, &save_props));
  ExtImgProps load_props;
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(prototype, stream, &load_props));
  EXPECT_EQ(load_props.iff, save_props.iff);
  EXPECT_EQ(load_props.qty, 0);
  EXPECT_EQ(load_props.comp, IFC_NONE);
  EXPECT_NEAR(load_props.xDPI, save_props.xDPI, 0.51f);
  EXPECT_NEAR(load_props.yDPI, save_props.yDPI, 0.51f);

  const ExtImgProps save_props_none = {
    IFF_UNKNOWN, IFC_NONE, .0f, .0f, 0
  };
  ASSERT_EQ(MR_SUCCESS, minimgio::Save(stream, img, iff, &save_props_none));
  ASSERT_EQ(MR_SUCCESS, minimgio::GetFileProps(prototype, stream, &load_props));
  EXPECT_EQ(load_props.iff, iff);
  EXPECT_EQ(load_props.qty, 0);
  EXPECT_EQ(load_props.comp, IFC_NONE);
  EXPECT_EQ(load_props.xDPI, save_props_none.xDPI);
  EXPECT_EQ(load_props.yDPI, save_props_none.yDPI);
}

static int create_f_image(
  MinImg *f_img, const int scale=15, const int channels=1)
{
  const unsigned char f_letter[][3] = {
    { 0x00, 0x00, 0x00 },
    { 0x00, 0xff, 0xff },
    { 0x00, 0x00, 0xff },
    { 0x00, 0xff, 0xff },
    { 0x00, 0xff, 0xff } };
  const int width = sizeof(f_letter[0]) / sizeof(f_letter[0][0]);
  const int height = sizeof(f_letter) / sizeof(f_letter[0]);
  PROPAGATE_ERROR(NewMinImagePrototype(
    f_img, width * scale, height * scale, channels, TYP_UINT8));
  unsigned char const* fill_black;
  unsigned char const* fill_white;
  int value_size = 1;
  if (channels == 4) {
    static const unsigned char data[] = { 0,0,0,0xff,0xff,0xff,0xff,0xff };
    fill_black = data;
    fill_white = data + 4;
    value_size = 4;
  }
  else {
    fill_black = &f_letter[0][0];
    fill_white = &f_letter[1][1];
  }

  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x) {
      MinImg region = {};
      PROPAGATE_ERROR(GetMinImageRegion(
        &region, f_img, x * scale, y * scale, scale, scale));
      PROPAGATE_ERROR(FillMinImage(
        &region, f_letter[y][x]?fill_white:fill_black, value_size));
    }
  return NO_ERRORS;
}
