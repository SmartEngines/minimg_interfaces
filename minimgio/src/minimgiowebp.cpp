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

#include <cstdlib>
#include <cstring>
#include <vector>

#include <webp/decode.h>
#include <webp/encode.h>

#include <minutils/smartptr.h>
#include <minimgio/minimgio.hpp>


static MinResult GetWebPPropsFromMemory(
    MinImg        &img,
    ExtImgProps   *p_props,
    const uint8_t *ptr,
    const size_t   size) {
  WebPBitstreamFeatures features;
  VP8StatusCode sc = WebPGetFeatures(ptr, size, &features);
  if (sc != VP8_STATUS_OK) {
    return MR_INTERNAL_ERROR;
  }

  img.width = features.width;
  img.height = features.height;
  img.scalar_type = TYP_UINT8;
  img.channels = features.has_alpha ? 4 : 3;

  if (p_props) {
    p_props->iff = IFF_WEBP;
  }

  return MR_SUCCESS;
}

static MinResult ReadStreamToMemory(
    std::vector<uint8_t> &file_data,
    minimgio::BinaryStream& stream) {
  int64_t file_size = stream.lseek(0, SEEK_END);
  if (file_size < 0)
    return MR_ENV_ERROR;
  stream.lseek(0);
  file_data.resize(static_cast<size_t>(file_size));
  if (stream.read(&file_data[0], file_data.size()) != file_data.size())
    return MR_ENV_ERROR;
  return MR_SUCCESS;
}

static MinResult DecodeWebPFromMemory(
    const MinImg &img,
    uint8_t      *ptr,
    size_t        size) {
  if (!ptr || size == 0) {
    return MR_CONTRACT_VIOLATION;
  }

  if (img.scalar_type != TYP_UINT8) {
    return MR_CONTRACT_VIOLATION;
  }

  const int stride = img.stride;
  const int ht = img.height;
  if (img.channels == 3) {
    if (WebPDecodeRGBInto(ptr, size, img.p_zero_line, stride * ht, stride) == NULL)
      return MR_INTERNAL_ERROR;
  } else if (img.channels == 4) {
    if (WebPDecodeRGBAInto(ptr, size, img.p_zero_line, stride * ht, stride) == NULL)
      return MR_INTERNAL_ERROR;
  } else {
    return MR_CONTRACT_VIOLATION;
  }

  return MR_SUCCESS;
}

static int MinWebPWriter(
    const uint8_t* data,
    size_t data_size,
    const WebPPicture* picture) {
  minimgio::BinaryStream* stream = (minimgio::BinaryStream*)picture->custom_ptr;
  return stream->write(data, data_size) == data_size ? 1 : 0;
}

static MinResult min_webp_error(
    minimgio::BinaryStream &stream,
    WebPEncodingError error_code) {
  const char *error_message = NULL;
  MinResult ret = MR_SUCCESS;
  switch(error_code) {
    case VP8_ENC_OK:
      return MR_SUCCESS;
    case VP8_ENC_ERROR_OUT_OF_MEMORY:
      error_message = "memory error allocating objects";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_BITSTREAM_OUT_OF_MEMORY:
      error_message = "memory error while flushing bits";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_NULL_PARAMETER:
      error_message = "a pointer parameter is NULL";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_INVALID_CONFIGURATION:
      error_message = "configuration is invalid";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_BAD_DIMENSION:
      error_message = "picture has invalid width/height";
      ret = MR_CONTRACT_VIOLATION;
      break;
    case VP8_ENC_ERROR_PARTITION0_OVERFLOW:
      error_message = "partition is bigger than 512k";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_PARTITION_OVERFLOW:
      error_message = "partition is bigger than 16M";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_BAD_WRITE:
      error_message = "error while flushing bytes";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_FILE_TOO_BIG:
      error_message = "file is bigger than 4G";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_USER_ABORT:
      error_message = "abort request by user";
      ret = MR_ENV_ERROR;
      break;
    case VP8_ENC_ERROR_LAST:
      error_message = "list terminator. always last.";
      ret = MR_ENV_ERROR;
      break;
    default:
      error_message = "unspecified webp error";
      ret = MR_INTERNAL_ERROR;
      break;
  }
  stream.error(error_message);
  return ret;
}

namespace minimgio {namespace internal {
MinResult GetPropsWebP(
    MinImg       &img,
    ExtImgProps  *p_props,
    BinaryStream &stream,
    int page) {
  if (page != 0)
    return MR_NOT_IMPLEMENTED;
  std::vector<uint8_t> file_data;
  MR_PROPAGATE_ERROR(ReadStreamToMemory(file_data, stream));
  return GetWebPPropsFromMemory(img, p_props, &file_data[0], file_data.size());
}

MinResult GetNumPagesWebP(
    int &num_pages,
    BinaryStream &/*s*/) {
  num_pages = 1;
  return MR_SUCCESS;
}

MinResult LoadWebP(
    const MinImg &img,
    BinaryStream  &stream,
    int           page) {
  if (!img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (img.scalar_type != TYP_UINT8)
    return MR_CONTRACT_VIOLATION;
  if (img.channels != 3 && img.channels != 4)
    return MR_CONTRACT_VIOLATION;
  if (page != 0)
    return MR_NOT_IMPLEMENTED;
  std::vector<uint8_t> file_data;
  MR_PROPAGATE_ERROR(ReadStreamToMemory(file_data, stream));
  return DecodeWebPFromMemory(img, &file_data[0], file_data.size());
}
#ifdef MINIMGIO_GENERATE
MinResult SaveWebP(
    BinaryStream      &stream,
    const MinImg      &img,
    const ExtImgProps */*p_props*/,
    int                /*page*/) {
  if (!img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (img.scalar_type != TYP_UINT8) {
    stream.error("not uint8");
    return MR_CONTRACT_VIOLATION;
  }
  if (img.channels != 3 && img.channels != 4) {
    stream.error("not 3 or 4 channels");
    return MR_CONTRACT_VIOLATION;
  }

  WebPPicture pic;
  WebPConfig config;

  if (!WebPConfigPreset(&config, WEBP_PRESET_DEFAULT, 70) ||
      !WebPPictureInit(&pic)) {
    return MR_INTERNAL_ERROR;
  }

  config.lossless = true;
  config.exact = true;
  pic.use_argb = true;
  pic.width = img.width;
  pic.height = img.height;
  pic.writer = MinWebPWriter;
  pic.custom_ptr = &stream;

  MR_PROPAGATE_ERROR(stream.initialize(MIS_WRITEONLY));
  int ok = 0;
  switch (img.channels) {
    case 3: ok = WebPPictureImportRGB(&pic, img.p_zero_line, img.stride); break;
    case 4: ok = WebPPictureImportRGBA(&pic, img.p_zero_line, img.stride); break;
    default: return MR_CONTRACT_VIOLATION;
  }
  if (!ok) {
    return min_webp_error(stream, pic.error_code);
  }
  ok = WebPEncode(&config, &pic);
  WebPPictureFree(&pic);
  return min_webp_error(stream, pic.error_code);
}

#endif // #ifdef MINIMGIO_GENERATE
} // namespace internal
} // namespace minimgio
