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

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <minimgio/minimgio.hpp>
#include <cstring>  // strrchr
#include <ctype.h> // isprint
#include <algorithm> // std::max

#include <minbase/minresult.h>
#include <minutils/smartptr.h>
#include "minimgiodevice.h"
#include "utils.h"
#include "pack.h"

#ifdef _WIN32
#define stricmp _stricmp
#else
#include <strings.h>  // POSIX
#define stricmp strcasecmp
#endif


namespace minimgio { namespace internal {

#define MIN_DECLARE_INTERNAL(MODULE_NAME) \
MinResult GetNumPages ## MODULE_NAME(     \
    int &num_pages,                       \
    BinaryStream &stream);                \
MinResult GetProps ## MODULE_NAME(        \
    MinImg       &img,                    \
    ExtImgProps  *p_props,                \
    BinaryStream &stream,                 \
    int          page);                   \
MinResult Load ## MODULE_NAME(            \
    MinImg const &img,                    \
    BinaryStream &stream,                 \
    int          page);                   \
MinResult Save ## MODULE_NAME(            \
    BinaryStream      &stream,            \
    const MinImg      &img,               \
    const ExtImgProps *p_props,           \
    int                page);

#ifdef WITH_TIFF
MIN_DECLARE_INTERNAL(Tiff)
#define MIN_TIFF_CALL(call) call
#else
#define MIN_TIFF_CALL(call) MR_STRIPPED_VERSION
#endif

#ifdef WITH_JPEG
MIN_DECLARE_INTERNAL(Jpeg)
#define MIN_JPEG_CALL(call) call
#else
#define MIN_JPEG_CALL(call) MR_STRIPPED_VERSION
#endif

#ifdef WITH_PNG
MIN_DECLARE_INTERNAL(Png)
#define MIN_PNG_CALL(call) call
#else
#define MIN_PNG_CALL(call) MR_STRIPPED_VERSION
#endif

#ifdef WITH_WEBP
MIN_DECLARE_INTERNAL(WebP)
#define MIN_WEBP_CALL(call) call
#else
#define MIN_WEBP_CALL(call) MR_STRIPPED_VERSION
#endif

MIN_DECLARE_INTERNAL(Lst)

#undef MIN_DECLARE_INTERNAL
}}

MinResult GetLstPageName(
    char *pPageName,
    int pageNameSize,
    minimgio::BinaryStream &stream,
    int page);


namespace minimgio {namespace internal {
static bool IsTextualFile(
    BinaryStream &stream) {
  if (stream.lseek(0) != 0)
    return false;
  char block[1024];
  int realBlockSize = static_cast<int>(stream.read(block, sizeof(block)));

  for (int i = 0; i < realBlockSize; i++)
    if (!isprint(block[i]) && !isspace(block[i]))
      return false;
  return true;
}

static MinResult GuessImageFileFormat(
    ImgFileFormat &iff,
    BinaryStream  &stream) {
  const uint8_t TIFF_TAG_II[] = {'I', 'I'};
  const uint8_t TIFF_TAG_MM[] = {'M', 'M'};
  const uint8_t JPEG_TAG[] = {0xFF, 0xD8};
  const uint8_t PNG_TAG[]  = {0x89, 'P', 'N', 'G'};
  const uint8_t WEBP_TAG_1[] = {'R', 'I', 'F', 'F'};
  const uint8_t WEBP_TAG_2[] = {'W', 'E', 'B', 'P'};

  uint8_t tag[sizeof(WEBP_TAG_1) + 4 + sizeof(WEBP_TAG_2)] = {};
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  iff = IFF_UNKNOWN;
  const size_t read_sz = stream.read(tag, sizeof(tag));
  if (read_sz == sizeof(tag)) {
    if (!::memcmp(tag, TIFF_TAG_II, sizeof(TIFF_TAG_II)) ||
        !::memcmp(tag, TIFF_TAG_MM, sizeof(TIFF_TAG_MM)))
      iff = IFF_TIFF;
    else if (!::memcmp(tag, JPEG_TAG, sizeof(JPEG_TAG)))
      iff = IFF_JPEG;
    else if (!::memcmp(tag, PNG_TAG, sizeof(PNG_TAG)))
      iff = IFF_PNG;
    else if (!::memcmp(tag,     WEBP_TAG_1, sizeof(WEBP_TAG_1)) &&
             !::memcmp(tag + 8, WEBP_TAG_2, sizeof(WEBP_TAG_2)))
      iff = IFF_WEBP;
  }
  if (iff == IFF_UNKNOWN && read_sz > 1 && IsTextualFile(stream))
    iff = IFF_LST;
  return MR_SUCCESS;
}
} // namespace internal

MINIMGIO_API MinResult GetNumPages(
    int &pages,
    BinaryStream &stream) {
  MR_PROPAGATE_ERROR(stream.initialize(MIS_READONLY));
  ImgFileFormat iff;
  MR_PROPAGATE_ERROR(internal::GuessImageFileFormat(iff, stream));
  switch (iff) {
    case IFF_TIFF:
      return MIN_TIFF_CALL(minimgio::internal::GetNumPagesTiff(pages, stream));
    case IFF_JPEG:
      return MIN_JPEG_CALL(minimgio::internal::GetNumPagesJpeg(pages, stream));
    case IFF_PNG:
      return MIN_PNG_CALL(minimgio::internal::GetNumPagesPng(pages, stream));
    case IFF_WEBP:
      return MIN_WEBP_CALL(minimgio::internal::GetNumPagesWebP(pages, stream));
    case IFF_LST:
      return minimgio::internal::GetNumPagesLst(pages, stream);
    case IFF_UNKNOWN:
    default:
      return MR_CONTRACT_VIOLATION;
  }
  return MR_INTERNAL_ERROR;
}

MINIMGIO_API MinResult GetPageName(
    char *pPageName,
    int pageNameSize,
    BinaryStream &stream,
    int page) {
  MR_PROPAGATE_ERROR(stream.initialize(MIS_READONLY));
  ImgFileFormat iff;
  MR_PROPAGATE_ERROR(internal::GuessImageFileFormat(iff, stream));
  switch (iff) {
    case IFF_TIFF:
      return MIN_TIFF_CALL(CopyString(
        pPageName, pageNameSize, stream.get_source()));
    case IFF_JPEG:
      return MIN_JPEG_CALL(CopyString(
        pPageName, pageNameSize, stream.get_source()));
    case IFF_PNG:
      return MIN_PNG_CALL(CopyString(
        pPageName, pageNameSize, stream.get_source()));
    case IFF_WEBP:
      return MIN_WEBP_CALL(CopyString(
        pPageName, pageNameSize, stream.get_source()));
    case IFF_LST:
      return GetLstPageName(pPageName, pageNameSize, stream, page);
    default:
      return MR_CONTRACT_VIOLATION;
  }

  return MR_INTERNAL_ERROR;
}


MINIMGIO_API MinResult GetFileProps(
    MinImg       &img,
    BinaryStream &stream,
    ExtImgProps  *p_props,
    int           page) {
  MR_PROPAGATE_ERROR(stream.initialize(MIS_READONLY));
  ImgFileFormat iff;
  MR_PROPAGATE_ERROR(internal::GuessImageFileFormat(iff, stream));
  switch (iff) {
    case IFF_TIFF:
      return MIN_TIFF_CALL(internal::GetPropsTiff(img, p_props, stream, page));
    case IFF_JPEG:
      return MIN_JPEG_CALL(internal::GetPropsJpeg(img, p_props, stream, page));
    case IFF_PNG:
      return MIN_PNG_CALL(internal::GetPropsPng(img, p_props, stream, page));
    case IFF_WEBP:
      return MIN_WEBP_CALL(internal::GetPropsWebP(img, p_props, stream, page));
    case IFF_LST:
      return internal::GetPropsLst(img, p_props, stream, page);
    case IFF_UNKNOWN:
    default:
      return MR_CONTRACT_VIOLATION;
  }
  return MR_INTERNAL_ERROR;
}

MINIMGIO_API MinResult Load(
    const MinImg &img,
    BinaryStream &stream,
    int           page) {
  MR_PROPAGATE_ERROR(stream.initialize(MIS_READONLY));
  ImgFileFormat iff;
  MR_PROPAGATE_ERROR(internal::GuessImageFileFormat(iff, stream));
  switch (iff) {
    case IFF_TIFF:
      return MIN_TIFF_CALL(internal::LoadTiff(img, stream, page));
    case IFF_JPEG:
      return MIN_JPEG_CALL(internal::LoadJpeg(img, stream, page));
    case IFF_PNG:
      return MIN_PNG_CALL(internal::LoadPng(img, stream, page));
    case IFF_WEBP:
      return MIN_WEBP_CALL(internal::LoadWebP(img, stream, page));
    case IFF_LST:
      return internal::LoadLst(img, stream, page);
    case IFF_UNKNOWN:
    default:
      return MR_CONTRACT_VIOLATION;
  }
  return MR_INTERNAL_ERROR;
}

MINIMGIO_API MinResult Save(
    BinaryStream       &stream,
    const MinImg       &img,
    ImgFileFormat       img_file_format,
    const ExtImgProps  *p_props,
    int                 page) {
#ifdef MINIMGIO_GENERATE
  switch (img_file_format)
  {
  case IFF_TIFF:
    return MIN_TIFF_CALL(internal::SaveTiff(stream, img, p_props, page));
  case IFF_JPEG:
    return MIN_JPEG_CALL(internal::SaveJpeg(stream, img, p_props, page));
  case IFF_PNG:
    return MIN_PNG_CALL(internal::SavePng(stream, img, p_props, page));
  case IFF_WEBP:
    return MIN_WEBP_CALL(internal::SaveWebP(stream, img, p_props, page));
  default:
    return MR_CONTRACT_VIOLATION;
  }
  return MR_INTERNAL_ERROR;
#else
  return MR_STRIPPED_VERSION;
#endif
}

MINIMGIO_API MinResult Pack(
    const MinImg &dst_image,
    const MinImg &src_image,
    uint8_t       level) {
  if (src_image.p_zero_line == NULL || dst_image.p_zero_line == NULL)
    return MR_CONTRACT_VIOLATION;
  if (src_image.width > dst_image.width || src_image.height > dst_image.height)
    return MR_CONTRACT_VIOLATION;
  if (dst_image.scalar_type != TYP_UINT1 || src_image.scalar_type != TYP_UINT8)
    return MR_CONTRACT_VIOLATION;
  if (src_image.channels != dst_image.channels || dst_image.channels < 1)
    return MR_CONTRACT_VIOLATION;

  const size_t count = static_cast<size_t>(dst_image.width) * dst_image.channels;
  const uint8_t* p_src_line = src_image.p_zero_line;
  uint8_t* p_dst_line = dst_image.p_zero_line;

  for (int y = 0; y < dst_image.height; y++) {
    MR_PROPAGATE_ERROR(PackLine(p_dst_line, p_src_line, level, count, false));
    p_src_line += src_image.stride;
    p_dst_line += dst_image.stride;

  }
  return MR_SUCCESS;
}

MINIMGIO_API MinResult Unpack(
    const MinImg &dst_image,
    const MinImg &src_image) {
  if (src_image.p_zero_line == NULL || dst_image.p_zero_line == NULL)
    return MR_CONTRACT_VIOLATION;
  if (src_image.width > dst_image.width || src_image.height > dst_image.height)
    return MR_CONTRACT_VIOLATION;
  if (src_image.scalar_type != TYP_UINT1 || dst_image.scalar_type != TYP_UINT8)
    return MR_CONTRACT_VIOLATION;
  if (src_image.channels != dst_image.channels || dst_image.channels < 1)
    return MR_CONTRACT_VIOLATION;

  const size_t count = static_cast<size_t>(dst_image.width) * dst_image.channels;
  const uint8_t *p_src_line = src_image.p_zero_line;
  uint8_t *p_dst_line = dst_image.p_zero_line;

  for (int y = 0; y < dst_image.height; y++)   {
    MR_PROPAGATE_ERROR(UnpackLine(p_dst_line, p_src_line, count, false));
    p_src_line += src_image.stride;
    p_dst_line += dst_image.stride;

  }
  return MR_SUCCESS;
}

}  // namespace minimgio

MINIMGIO_API ImgFileFormat GuessImageFileFormatByExtension(
    const char *fileName) {
  if (!fileName)
    return IFF_UNKNOWN;

  const char *pExt = strrchr(fileName, '.');
  if (!pExt)
    return IFF_UNKNOWN;
  pExt++;
  if (!stricmp(pExt, "jpg") || !stricmp(pExt, "jpeg"))
    return IFF_JPEG;
  if (!stricmp(pExt, "tif") || !stricmp(pExt, "tiff"))
    return IFF_TIFF;
  if (!stricmp(pExt, "png"))
    return IFF_PNG;
  if (!stricmp(pExt, "webp"))
    return IFF_WEBP;
  if (!stricmp(pExt, "lst") || !stricmp(pExt, "txt"))
    return IFF_LST;

  return IFF_UNKNOWN;
}


struct BinaryStreamC : public minimgio::BinaryStream {
  MIStreamArguments callbacks;
  BinaryStreamC(MIStreamArguments &callbacks) : callbacks(callbacks) {}
  MinResult initialize(MISInitFlag flag) override {
    return callbacks.initialize(callbacks.handle, flag);
  }
  // should act like as `std::fread`
  std::size_t read(void* buffer, std::size_t size) override {
    return callbacks.read(callbacks.handle, buffer, size);
  }
  // should act like as `std::fwrite`
  std::size_t write(const void* buffer, std::size_t size) override {
    return callbacks.write(callbacks.handle, buffer, size);
  }
  // should act like posix `lseek`, using `lseek` instead of pair
  // fseek/ftell, because `lseek` produces less code
  int64_t lseek(int64_t offset, int origin) override {
    return callbacks.lseek(callbacks.handle, offset, origin);
  }
  // error message callback
  void error(const char* message) override {
    return callbacks.error(callbacks.handle, message);
  }
  const char *get_source() const override {
    return callbacks.get_source(callbacks.handle);
  }
};

MINIMGIO_API MinResult MinImgIoCreateStream(
    MIStreamHandle* handle, MIStreamArguments *callbacks) {
  if (!handle || *handle || !callbacks)
    return MR_CONTRACT_VIOLATION;
  *handle = new BinaryStreamC(*callbacks);
  return MR_SUCCESS;
}

MINIMGIO_API MinResult MinImgIoFreeStream(
    MIStreamHandle* handle) {
  if (!handle)
    return MR_CONTRACT_VIOLATION;
  delete reinterpret_cast<minimgio::BinaryStream*>(*handle);
  *handle = nullptr;
  return MR_SUCCESS;
}

MINIMGIO_API MinResult MinImgIoCreateFileStream(
    MIStreamHandle *handle, const char* path) {
  if (!handle || !path)
    return MR_CONTRACT_VIOLATION;
  *handle = new minimgio::BinaryFileStream(path);
  return MR_SUCCESS;
}

MINIMGIO_API MinResult MinImgIoCreateFileDescriptorStream(
    MIStreamHandle *handle,
    int fd) {
  if (!handle || fd < 0)
    return MR_CONTRACT_VIOLATION;
  *handle = new minimgio::BinaryFileDescriptorStream(fd);
  return MR_SUCCESS;
}

MINIMGIO_API MinResult MinImgIoGetError(
    const char**   message,
    MIStreamHandle handle) {
  if (!message || !handle)
    return MR_CONTRACT_VIOLATION;
  // user should really know what he's doing
  minimgio::BinaryStreamErrorHandling *stream =
    reinterpret_cast<minimgio::BinaryStreamErrorHandling*>(handle);
  *message = stream->get_error();
  return MR_SUCCESS;
}

MINIMGIO_API MinResult MinImgIoGetNumPages(
    int *pages,
    MIStreamHandle stream) {
  if (!stream)
    return MR_CONTRACT_VIOLATION;
  return minimgio::GetNumPages(
    *pages, *reinterpret_cast<minimgio::BinaryStream*>(stream));
}

MINIMGIO_API MinResult MinImgIoGetFileProps(
    MinImg         *img,
    ExtImgProps    *p_props,
    MIStreamHandle  stream,
    int             page) {
  if (!stream || !img)
    return MR_CONTRACT_VIOLATION;
  return minimgio::GetFileProps(
    *img, *reinterpret_cast<minimgio::BinaryStream*>(stream), p_props, page);
}

MINIMGIO_API MinResult MinImgIoLoad(
    const MinImg   *img,
    MIStreamHandle  stream,
    int             page) {
  if (!stream || !img)
    return MR_CONTRACT_VIOLATION;
  return minimgio::Load(
    *img, *reinterpret_cast<minimgio::BinaryStream*>(stream), page);
}

MINIMGIO_API MinResult MinImgIoSave(
    MIStreamHandle     stream,
    const MinImg      *img,
    ImgFileFormat      img_file_format,
    const ExtImgProps *p_props,
    int                page) {
  if (!stream || !img)
    return MR_CONTRACT_VIOLATION;
  return minimgio::Save(*reinterpret_cast<minimgio::BinaryStream*>(stream),
                        *img, img_file_format, p_props, page);
}
