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

/**
 * @file   minimgio.h
 * @brief  MinImgIO library application programming interface.
 */

/**
 * @mainpage Overview
 * <b>MinImgIO</b> is an open-source platform-independent library for reading and
 * writing image files. The library does not contain any implementations of
 * encode/decode algorithms. Rather than do this, it uses third party open-source
 * cross-platform libraries. The following table provides a summary of the
 * supported image formats and respective libraries:
 *
 * <table>
 *   <tr>
 *     <th>Format</th>
 *     <th>Description</th>
 *     <th>Library</th>
 *   </tr>
 *   <tr>
 *     <td>TIFF</td>
 *     <td>Tagged Image File Format</td>
 *     <td>libtiff</td>
 *   </tr>
 *   <tr>
 *     <td>JPEG</td>
 *     <td>Joint Photographic Experts Group</td>
 *     <td>libjpeg</td>
 * </table>
 *
 * For the internal representation of images is used cross-platform open-source
 * container - MinImg (see @ref MinUtils_MinImg section for more information).
 * The advantages of this container are the using a minimal number of fields
 * needed to represent the bitmap image and the easy way to cast it to other
 * standard and popular containers (for instance, Windows DIB, GDI+ BitmapData,
 * Intel/OpenCV IplImage).
 *
 * <b>MinImgIO</b> library allows write/read images to/from both file system and
 * memory block (see @ref MinImgIOTutorial_Memory for more information).
 *
 * The library is written in C++ and can be compiled under Linux (GCC) and
 * Windows (MSVC 8 and later). Though the library has been written in C++, it
 * has C interface, so it can be embedded in different systems.
 */

/**
 * @page MinImgIOTutorial Quick Tutorial
 * This tutorial is intended to get you start using <b>MinImgIO</b> library to
 * simply read and write images, therefore the tutorial is not a complete or
 * detailed documentation of the library. Note also, that some secondary
 * operations will be purposely omitted for brevity.
 *
 * @section MinImgIOTutorial_Reading Reading and Writing Images
 * Let @c szImagePath is a null-terminated string that contains the physical
 * path of the image. At the first step we should get image properties (size,
 * channel number, depth and other). The following code shows how to open the
 * image and retrieve the properties of the first page:
 *
 * @code
 * MinImg image = {0};
 * PROPAGATE_ERROR(GetMinImageFileProps(&image, szImagePath, 0));
 * @endcode
 *
 * Then we will allocate the memory for the image data. To do that we use @c
 * AllocMinImage() function from <b>MinImgAPI</b> library:
 *
 * @code
 * PROPAGATE_ERROR(AllocMinImage(&image, 16));
 * @endcode
 *
 * Now we are ready to read image data. The following code demonstrates loading
 * the first page of the image:
 *
 * @code
 * PROPAGATE_ERROR(LoadMinImage(&image, szImagePath, 0));
 * @endcode
 *
 * Now move to writing images. Let we have already some @c image object and want
 * to save it to the file @c szImagePath. The following code shows how to do that:
 *
 * @code
 * PROPAGATE_ERROR(SaveMinImage(szImagePath, &image, 0));
 * @endcode
 *
 * Note that @c SaveMinImage() function determines the proper file format based
 * on the filename extension. If you want to specify it manually, you should use
 * more comprehensive function @c SaveMinImageEx().
 *
 * @section MinImgIOTutorial_Memory Loading Images from Memory
 * Sometimes it is not practical or even possible to load an image from disk. For
 * such situations LoadMinImage() allows to read an image from memory block which
 * contains valid image format. To do this, you should use a special format of
 * filename:
 *
 * @code
 *   mem://<pointer-to-memory-block>.<size-of-memory-block>
 * @endcode
 *
 * Let @c pImageData is a pointer to the image in memory and @c imageSize is the
 * size the image. The following example demonstrates a way to generate filename
 * to the image in memory:
 *
 * @code
 *   char szImageMemPath[250] = {0};
 *   sprintf(szImageMemPath, "mem://%p.%lu", pImageData, imageSize);
 * @endcode
 */

/**
 * @page MinImgIOLicese MinImgIO License Agreements
 * @section MinImgIOLicese_Library Library License Agreement
 * <b>MinImgIO</b> is released under FreeBSD License. It is free for both
 * academic and commercial use.
 * @include license.txt
 *
 * @section MinImgIOLicese_Documentation Documentation License Agreement
 * This documentation is released under FreeBSD Documentation License. It is
 * free for both academic and commercial use.
 * @include doc/license.txt
 */

#pragma once
#ifndef MINIMGIO_MINIMGIO_HPP_INCLUDED
#define MINIMGIO_MINIMGIO_HPP_INCLUDED

#include <minbase/crossplat.h>
#include <minimgio/define.h>
#include <minbase/minimg.h>
#include <minbase/minresult.h>
#include "minimgio_types.h"

#ifdef __cplusplus
#  include <cstddef>  // std::size_t
#  include <cstdio>  // SEEK_SET
  extern "C" {
#endif

MINIMGIO_API ImgFileFormat GuessImageFileFormatByExtension(
  const char *fileName
);

MINIMGIO_API MinResult MinImgIoCreateStream(
    MIStreamHandle *handle, MIStreamArguments *);

MINIMGIO_API MinResult MinImgIoFreeStream(
    MIStreamHandle *handle);

MINIMGIO_API MinResult MinImgIoCreateFileStream(
    MIStreamHandle *handle, const char*);

MINIMGIO_API MinResult MinImgIoCreateFileDescriptorStream(
    MIStreamHandle *handle, int fd);

MINIMGIO_API MinResult MinImgIoGetError(
    const char** message, MIStreamHandle handle);

MINIMGIO_API MinResult MinImgIoGetNumPages(
    int *pages, MIStreamHandle  stream);

MINIMGIO_API MinResult MinImgIoGetFileProps(
    MinImg         *img,
    ExtImgProps    *p_props,
    MIStreamHandle  stream,
    int             page);

MINIMGIO_API MinResult MinImgIoLoad(
    const MinImg   *img,
    MIStreamHandle  stream,
    int             page);

MINIMGIO_API MinResult MinImgIoSave(
    MIStreamHandle     stream,
    const MinImg      *img,
    ImgFileFormat      img_file_format,
    const ExtImgProps *p_props,
    int                page);

#ifdef __cplusplus
  } // extern "C"
#endif

#ifdef __cplusplus
namespace minimgio {

struct MINIMGIO_API BinaryStream {
  // initialize the stream, this is where `fopen` should happen.
  // for example, reading png the flag is READONLY, writing png WRITEONLY,
  // and writing tiff is READWRITE
  // this method is expected to be called multiple times
  virtual MinResult initialize(MISInitFlag flag) = 0;
  // should act like as `std::fread`
  virtual std::size_t read(void* buffer, std::size_t size) = 0;
  // should act like as `std::fwrite`
  virtual std::size_t write(const void* buffer, std::size_t size) = 0;
  // should act like posix `lseek`, using `lseek` instead of pair
  // fseek/ftell, because `lseek` produces less code
  virtual int64_t lseek(int64_t offset, int whence=SEEK_SET) = 0;
  // error message callback
  virtual void error(const char* message) = 0;
  virtual const char *get_source() const { return nullptr; }

  BinaryStream() = default;
  virtual ~BinaryStream() = default;
  BinaryStream(BinaryStream const&) = delete;
  BinaryStream& operator=(BinaryStream const&) = delete;
};

struct MINIMGIO_API BinaryStreamErrorHandling : public BinaryStream {
  BinaryStreamErrorHandling();
  ~BinaryStreamErrorHandling() override;
  void error(const char* message) override;
  const char* get_error() const;

  BinaryStreamErrorHandling(const BinaryStreamErrorHandling&) = delete;
protected:
  void* error_buffer;
};

struct MINIMGIO_API BinaryFileDescriptorStream
    : public BinaryStreamErrorHandling {
  BinaryFileDescriptorStream(int fd);
#ifdef _WIN32
  BinaryFileDescriptorStream(void* handle);
#endif
  MinResult initialize(MISInitFlag) override { return MR_SUCCESS; }
  std::size_t read(void* buffer, std::size_t size) override;
  std::size_t write(const void* buffer, std::size_t size) override;
  int64_t lseek(int64_t offset, int whence) override;

protected:
#ifdef _WIN32
  void* handle;
#else
  int fd;
#endif // _WIN32
};

struct MINIMGIO_API BinaryFileStream : public BinaryFileDescriptorStream {
  BinaryFileStream(const char* filename);
  MinResult initialize(MISInitFlag flag) override;
  virtual const char *get_source() const override;
  ~BinaryFileStream() override;

private:
  const char* filename;
};

struct MINIMGIO_API BinaryMemoryReadonlyStream
    : public BinaryStreamErrorHandling {
  // small caveat: buffer MUST live till the stream is destroyed
  BinaryMemoryReadonlyStream(const uint8_t* buffer, std::size_t size);
  MinResult initialize(MISInitFlag flag) override;
  std::size_t read(void* buffer, std::size_t size) override;
  std::size_t write(const void* buffer, std::size_t size) override;
  int64_t lseek(int64_t offset, int whence) override;

  BinaryMemoryReadonlyStream(const BinaryMemoryReadonlyStream&) = delete;

private:
  const uint8_t* buffer;
  const std::size_t size;
  std::size_t pos;
};

MINIMGIO_API MinResult GetNumPages(
    int          &num_pages,
    BinaryStream &stream);

MINIMGIO_API MinResult GetPageName(
    char *p_page_name,
    int page_name_size,
    BinaryStream &stream,
    int page);

MINIMGIO_API MinResult GetFileProps(
    MinImg       &img,
    BinaryStream &stream,
    ExtImgProps  *p_props = nullptr,
    int           page = 0);

MINIMGIO_API MinResult Load(
    const MinImg &img,
    BinaryStream &stream,
    int           page = 0);

MINIMGIO_API MinResult Save(
    BinaryStream      &stream,
    const MinImg      &img,
    ImgFileFormat      img_file_format,
    const ExtImgProps *p_props = nullptr,
    int                page = 0);

MINIMGIO_API MinResult Pack(
    const MinImg &dst_image,
    const MinImg &src_image,
    uint8_t       level = 128);

MINIMGIO_API MinResult Unpack(
    const MinImg &dst_image,
    const MinImg &src_image);

} // namespace minimgio

#endif // __cplusplus

#endif // #ifndef MINIMGIO_MINIMGIO_HPP_INCLUDED
