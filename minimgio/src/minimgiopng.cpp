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

#include <algorithm>  // std::max, std::min
//#include <minbase/minresult.h>
#include <minutils/smartptr.h>
#include <minimgio/minimgio.hpp>


#include <png.h>

namespace {

struct MIN_PNG {
  png_structp p_png = NULL;
  png_infop p_info = NULL;
  ~MIN_PNG() {
    png_destroy_read_struct(&this->p_png, &this->p_info, NULL);
  }
};

} // anonymous namespace

namespace minimgio {namespace internal {
static void MinimgioPngRead(png_structp png_ptr,
                            png_bytep   bytes,
                            png_size_t  count) {
  BinaryStream *stream = static_cast<BinaryStream *>(png_get_io_ptr(png_ptr));
  if (stream->read(bytes, count) != count)
    png_error(png_ptr, "`read` call failed (file corrupted?)");
}

static void MinimgioPngWrite(png_structp png_ptr,
                             png_bytep   bytes,
                             png_size_t  count) {
  BinaryStream *stream = static_cast<BinaryStream *>(png_get_io_ptr(png_ptr));
  if (stream->write(bytes, count) != count)
    png_error(png_ptr, "`write` call failed");
}

static void MinimgioPngFlush(png_structp) {
}

static void MinimgioPngError(png_structp png_ptr,
                             png_const_charp error_msg) {
  BinaryStream *stream = static_cast<BinaryStream *>(png_get_io_ptr(png_ptr));
  stream->error(error_msg);
}

static MinResult PngInitAndReadPrototype(
    png_structp  *ppPng,
    png_infop    *ppInfo,
    MinImg       &img,
    BinaryStream &stream) {
  png_byte header[8] = {};
  if (stream.read(header, sizeof(header)) != sizeof(header))
    return MR_ENV_ERROR;
  if (png_sig_cmp(header, 0, 8) != 0)
    return MR_INTERNAL_ERROR;

  png_structp pPng = *ppPng = png_create_read_struct(
    PNG_LIBPNG_VER_STRING, &stream,
    MinimgioPngError, MinimgioPngError);
  png_infop pInfo = *ppInfo = png_create_info_struct(pPng);
  if (!pInfo)
    return MR_INTERNAL_ERROR;

  png_set_read_fn(pPng, &stream, MinimgioPngRead);

  if (setjmp(png_jmpbuf(pPng)))
    return MR_ENV_ERROR;

  png_set_sig_bytes(pPng, 8);
  png_read_info(pPng, pInfo);

  // Begin
  png_uint_32 width = 0, height = 0;
  int depth = 0, color_type = 0, interlace = 0;
  png_get_IHDR(pPng, pInfo, &width, &height, &depth,
               &color_type, &interlace, 0, 0);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(pPng);

  int image_scalar_size_bytes = ByteSizeOfMinType(img.scalar_type);

  if (color_type == PNG_COLOR_TYPE_GRAY && depth < 8
      && (depth != 1 || image_scalar_size_bytes == 1))
    png_set_expand_gray_1_2_4_to_8(pPng);

  if (color_type & PNG_COLOR_MASK_ALPHA &&
      png_get_channels(pPng, pInfo) == img.channels + 1)
    png_set_strip_alpha(pPng);

  if (png_get_valid(pPng, pInfo, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(pPng);

  if (depth == 16)
  {
    if (image_scalar_size_bytes == 1)
      png_set_strip_16(pPng);
#ifndef __BIG_ENDIAN__
    else
      png_set_swap(pPng);
#endif
  }

  if (depth > 1 && depth < 8 && image_scalar_size_bytes == 1)
    png_set_packing(pPng);

  if (interlace != PNG_INTERLACE_NONE)
    png_set_interlace_handling(pPng);

  /* recheck header after setting EXPAND options */
  png_read_update_info(pPng, pInfo);
  png_get_IHDR(pPng, pInfo, &width, &height, &depth, &color_type, &interlace, 0, 0);

  if ((depth % 8 != 0 && depth != 1) || depth > 16)
    return MR_NOT_IMPLEMENTED;
  static const MinTyp png_types[] = {TYP_UINT1, TYP_UINT8, TYP_UINT16};
  img.scalar_type = png_types[depth >> 3];
  img.channels = png_get_channels(pPng, pInfo);
  img.height = height;
  img.width = width;
  return MR_SUCCESS;
}
}}


namespace minimgio {namespace internal {
MinResult GetNumPagesPng(int &num_pages, BinaryStream &) {
  num_pages = 1;
  return MR_SUCCESS;
}
MinResult GetPropsPng(
    MinImg      &img,
    ExtImgProps *p_props,
    BinaryStream &stream,
    int /*page*/
) {
  if (img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  MIN_PNG png{};
  MR_PROPAGATE_ERROR(
    PngInitAndReadPrototype(&png.p_png, &png.p_info, img, stream));

  if (p_props)
  {
    int unitType = PNG_RESOLUTION_UNKNOWN;
    png_uint_32 resX = 0, resY = 0;
    png_get_pHYs(png.p_png, png.p_info, &resX, &resY, &unitType);
    p_props->iff = IFF_PNG;
    p_props->comp = IFC_NONE;
    p_props->qty = 0;  // zlib compression quality is unknown
    if (unitType == PNG_RESOLUTION_METER)
    {
      const double dpiFactor = 100.0 / 2.54;
      p_props->xDPI = static_cast<float>(resX / dpiFactor);
      p_props->yDPI = static_cast<float>(resY / dpiFactor);
    }
    else
    {
      p_props->xDPI = static_cast<float>(resX);
      p_props->yDPI = static_cast<float>(resY);
    }
  }
  return MR_SUCCESS;
}
MinResult LoadPng(
  const MinImg &img,
  BinaryStream &stream,
  int           /*page*/
) {
  if (!img.p_zero_line || img.channels < 1)
    return MR_CONTRACT_VIOLATION;
  if (img.scalar_type != TYP_UINT1 &&
      img.scalar_type != TYP_UINT8 &&
      img.scalar_type != TYP_UINT16)
    return MR_NOT_IMPLEMENTED;
  if (img.channels > 4)
    return MR_NOT_IMPLEMENTED;

  // png_structp pPng = NULL;
  // png_infop pInfo = NULL;
  MinImg imgRead = {};
  imgRead.channels = img.channels;
  imgRead.scalar_type = img.scalar_type;
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  MIN_PNG png{};
  MR_PROPAGATE_ERROR(
    PngInitAndReadPrototype(&png.p_png, &png.p_info, imgRead, stream)
  );

  if (
    imgRead.width != img.width ||
    imgRead.height != img.height ||
    imgRead.channels != img.channels ||
    imgRead.scalar_type != img.scalar_type // ||
    //imgRead.address_space != img.address_space
    )
  {
    return MR_CONTRACT_VIOLATION;
  }

  // Read the file
  if (setjmp(png_jmpbuf(png.p_png)))
  {
    return MR_ENV_ERROR;
  }
  scoped_cpp_array<png_bytep> ppRows(new png_bytep[img.height]);
  png_bytep line = img.p_zero_line;
  for (int32_t y = 0; y < img.height; ++y) {
    ppRows[y] = line;
    line += img.stride;
  }

  png_read_image(png.p_png, ppRows);

  return MR_SUCCESS;
}

#ifdef MINIMGIO_GENERATE
MinResult SavePng(
    BinaryStream      &stream,
    const MinImg      &img,
    const ExtImgProps *p_props,
    int                page) {
  if (page != 0)
    return MR_CONTRACT_VIOLATION;
  if (!img.p_zero_line) {
    stream.error("Empty image");
    return MR_CONTRACT_VIOLATION;
  }
  if (img.channels < 1 || img.channels > 4) {
    stream.error("Invalid number of channels. png supports 1-4 channels");
    return MR_CONTRACT_VIOLATION;
  }
  if (img.scalar_type != TYP_UINT8 &&
      img.scalar_type != TYP_UINT16 &&
      img.scalar_type != TYP_UINT1) {
    stream.error("Unsupported image format");
    return MR_CONTRACT_VIOLATION;
  }

  png_structp pPng = png_create_write_struct(
    PNG_LIBPNG_VER_STRING, &stream, MinimgioPngError, MinimgioPngError);
  if (pPng == 0)
    return MR_INTERNAL_ERROR;

  png_infop pInfo = png_create_info_struct(pPng);
  if (pInfo == 0)
  {
    png_destroy_write_struct(&pPng, 0);
    return MR_INTERNAL_ERROR;
  }

  if (setjmp(png_jmpbuf(pPng)))
  {
    png_destroy_write_struct(&pPng, &pInfo);
    return MR_ENV_ERROR;
  }

  const int depth = 1 << LogBitSizeOfMinType(img.scalar_type);
  int colorType = PNG_COLOR_TYPE_GRAY;
  if (img.channels == 2)
    colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
  else if (img.channels == 3)
    colorType = PNG_COLOR_TYPE_RGB;
  else if (img.channels == 4)
    colorType = PNG_COLOR_TYPE_RGBA;
  png_set_IHDR(pPng, pInfo, img.width, img.height, depth, colorType, PNG_INTERLACE_NONE, 0, 0);

  if (p_props != 0)
  {
    if (p_props->xDPI != 0.0 || p_props->yDPI != 0.0)
    {
      const double dpiFactor = 100.0 / 2.54;
      uint32_t xDPM = static_cast<uint32_t>(p_props->xDPI * dpiFactor + 0.5);
      uint32_t yDPM = static_cast<uint32_t>(p_props->yDPI * dpiFactor + 0.5);
      png_set_pHYs(pPng, pInfo, xDPM , yDPM, PNG_RESOLUTION_METER);
    }
    if (p_props->qty)
    {
      // to comply with jpeg range 0-100, where 0 is the default value
      const int level = std::max(-1, std::min(p_props->qty / 10, 9));
      png_set_compression_level(pPng, level);
    }
  }

  png_set_write_fn(pPng, &stream, MinimgioPngWrite, MinimgioPngFlush);
  MR_PROPAGATE_ERROR(stream.initialize(MIS_WRITEONLY));
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  png_write_info(pPng, pInfo);
#ifndef __BIG_ENDIAN__
  if (depth == 16)
    png_set_swap(pPng);
#endif

  scoped_cpp_array<png_bytep> ppRows(new png_bytep[img.height]);
  png_bytep line = img.p_zero_line;
  for(int y = 0; y < img.height; ++y) {
    ppRows[y] = line;
    line += img.stride;
  }
  png_write_image(pPng, ppRows);

  png_write_end(pPng, pInfo);
  png_destroy_write_struct(&pPng, &pInfo);

  return MR_SUCCESS;
}
#endif  // #ifdef MINIMGIO_GENERATE
} // namespace internal
} // namespace minimgio
