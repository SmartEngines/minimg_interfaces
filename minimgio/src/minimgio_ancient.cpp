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

#include <minimgio/minimgio.h>
#include <minimgio/minimgio.hpp>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>  // std::min
#include <ctype.h>

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


typedef enum _FileLocation {
  inFileSystem,
  inMemory,
  inDevice
} FileLocation;


static FileLocation DeduceFileLocation(
    const char *fileName) {
  const char kMemoryLocationPrefix[] = "mem://";
  const char kDeviceLocationPrefix[] = "dev://";

  if (!strncmp(fileName, kMemoryLocationPrefix, strlen(kMemoryLocationPrefix)))
    return inMemory;
  else if (!strncmp(fileName, kDeviceLocationPrefix, strlen(kDeviceLocationPrefix)))
    return inDevice;
  else
    return inFileSystem;
}


struct UniversalBinaryStream {
  UniversalBinaryStream() : stream(NULL) {}
  MinResult initialize(const char *pFileName, const FileLocation fileLocation ) {
    if (fileLocation == inFileSystem)
      this->stream = new minimgio::BinaryFileStream(pFileName);
    else if (fileLocation == inMemory) {
      uint8_t *ptr = 0;
      size_t size = 0;
      MR_PROPAGATE_ERROR(ExtractMemoryLocation(pFileName, &ptr, &size));
      this->stream = new minimgio::BinaryMemoryReadonlyStream(ptr, size);
    } else {
      return MR_INTERNAL_ERROR;
    }
    return MR_SUCCESS;
  }
  operator minimgio::BinaryStream &() const {
    return *this->stream;
  }
  ~UniversalBinaryStream() { delete stream; }
private:
  minimgio::BinaryStream *stream;
};


namespace minimgio { namespace internal {

#define MIN_DECLARE_INTERNAL(MODULE_NAME) \
MinResult GetNumPages ## MODULE_NAME(     \
    int &pages,                           \
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
#endif

#ifdef WITH_JPEG
MIN_DECLARE_INTERNAL(Jpeg)
#endif

#ifdef WITH_PNG
MIN_DECLARE_INTERNAL(Png)
#endif

#ifdef WITH_WEBP
MIN_DECLARE_INTERNAL(WebP)
#endif

MIN_DECLARE_INTERNAL(Lst)

#undef MIN_DECLARE_INTERNAL
}}


MINIMGIO_API int GetMinImageFilePages(
    const char *pFileName) {
  if (!pFileName)
    return MR_CONTRACT_VIOLATION;
  const FileLocation fileLocation = DeduceFileLocation(pFileName);
  int pages = -1;
  if (fileLocation == inDevice) {
    MR_PROPAGATE_ERROR(GetDevicePages(&pages, pFileName));
  }
  else {
    UniversalBinaryStream stream;
    MR_PROPAGATE_ERROR(stream.initialize(pFileName, fileLocation));
    MR_PROPAGATE_ERROR(static_cast<minimgio::BinaryStream&>(stream).
                    initialize(MIS_READONLY));
    MR_PROPAGATE_ERROR(minimgio::GetNumPages(pages, stream));
  }
  return pages;
}

MINIMGIO_API int GetMinImagePageName(
    char *pPageName,
    int pageNameSize,
    const char *pFileName,
    int page) {
  const FileLocation fileLocation = DeduceFileLocation(pFileName);
  if (fileLocation == inDevice)
    return GetDevicePageName(pPageName, pageNameSize, pFileName, page);
  minimgio::BinaryFileStream stream(pFileName);
  return minimgio::GetPageName(pPageName, pageNameSize, stream, page);
}

MINIMGIO_API int GetMinImageFileProps(
    MinImg     *pImg,
    const char *pFileName,
    int         page) {
  return GetMinImageFilePropsEx(pImg, NULL, pFileName, page);
}

MINIMGIO_API int GetMinImageFilePropsEx(
    MinImg      *pImg,
    ExtImgProps *pProps,
    const char  *pFileName,
    int          page) {
  if (!pFileName || (!pProps && !pImg))
    return MR_CONTRACT_VIOLATION;
  const FileLocation fileLocation = DeduceFileLocation(pFileName);
  if (fileLocation == inDevice)
    return GetDevicePropsEx(pImg, pProps, pFileName);
  UniversalBinaryStream uni_strem;
  MR_PROPAGATE_ERROR(uni_strem.initialize(pFileName, fileLocation));
  return minimgio::GetFileProps(*pImg, uni_strem, pProps, page);
}

MINIMGIO_API int LoadMinImage(
    const MinImg *pImg,
    const char   *pFileName,
    int           page) {
  if (!pFileName || !pImg)
    return MR_CONTRACT_VIOLATION;
  const FileLocation fileLocation = DeduceFileLocation(pFileName);
  if (fileLocation == inDevice)
    return LoadDevice(pImg, pFileName);

  UniversalBinaryStream uni_strem;
  MR_PROPAGATE_ERROR(uni_strem.initialize(pFileName, fileLocation));
  return minimgio::Load(*pImg, uni_strem, page);
}

#ifndef STRICT_DATA_CONTAINMENT
MINIMGIO_API int SaveMinImage(
    const char   *pFileName,
    const MinImg *pImg,
    int           page) {
  return SaveMinImageEx(pFileName, pImg, NULL, page);
}

MINIMGIO_API int SaveMinImageEx(
    const char        *pFileName,
    const MinImg      *pImg,
    const ExtImgProps *pProps,
    int                page) {
  const FileLocation fileLocation = DeduceFileLocation(pFileName);
  if (fileLocation == inDevice)
    return SaveDeviceEx(pFileName, pImg, pProps);

  ImgFileFormat iff = pProps ? pProps->iff : IFF_UNKNOWN;

  if (iff == IFF_UNKNOWN)
    iff = GuessImageFileFormatByExtension(pFileName);
  if (iff == IFF_UNKNOWN)
    return MR_CONTRACT_VIOLATION;
  minimgio::BinaryFileStream stream(pFileName);
  return minimgio::Save(stream, *pImg, iff, pProps, page);
}
#endif // #ifndef STRICT_DATA_CONTAINMENT

MINIMGIO_API int PackMinImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    uint8_t       level) {
  if (p_src_image == NULL || p_dst_image == NULL)
    return MR_CONTRACT_VIOLATION;
  return minimgio::Pack(*p_dst_image, *p_src_image, level);
}

MINIMGIO_API int UnpackMinImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image) {
  if (p_src_image == NULL || p_dst_image == NULL)
    return MR_CONTRACT_VIOLATION;
  return minimgio::Unpack(*p_dst_image, *p_src_image);
}
