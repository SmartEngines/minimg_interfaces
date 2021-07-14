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

#pragma once
#ifndef MINIMGIO_MINIMGIO_TYPES_H_INCLUDED
#define MINIMGIO_MINIMGIO_TYPES_H_INCLUDED

#include <minbase/crossplat.h>
#include <minbase/mintyp.h>
#include <minbase/minresult.h>

/**
 * @brief   Specifies supported file formats.
 * @details The enum specifies all supported input/output file formats.
 * @ingroup MinImgIOAPI
 */
typedef enum
{
  IFF_UNKNOWN,  ///< Unknown file format.
  IFF_TIFF,     ///< Tagged image file format.
  IFF_JPEG,     ///< JPEG file format.
  IFF_PNG,      ///< PNG file format.
  IFF_WEBP,     ///< WebP file format.
  IFF_LST       ///< Lst file format.
} ImgFileFormat;

/**
 * @brief   Specifies supported TIFF compressions.
 * @details The enum specifies all supported TIFF compressions.
 * @ingroup MinImgIOAPI
 */
typedef enum
{
  IFC_NONE,     ///< No compression.
  IFC_LZW,      ///< Lempel-Ziv & Welch algorithm.
  IFC_DEFLATE,  ///< Deflate compression.
  IFC_PACKBITS, ///< PackBits compression (Macintosh RLE algorithm).
  IFC_JPEG,     ///< JPEG DCT compression.
  IFC_RLE,      ///< CCITT modified Huffman RLE algorithm.
  IFC_GROUP3,   ///< CCITT Group 3 fax encoding.
  IFC_GROUP4    ///< CCITT Group 4 fax encoding.
} ImgFileComp;

/**
 * @brief   Specifies mode for opening a binary stream.
 * @ingroup MinImgIOAPI
 */
typedef enum {
  MIS_READONLY,  ///< 'rb'
  MIS_WRITEONLY, ///< 'wb'
  MIS_READWRITE  ///< 'ab+'
} MISInitFlag;


/**
 * @brief   Specifies additional information for an image.
 * @details The structure specifies additional information about the image such
 *          as horizontal and vertical DPI, file format and lossy quality. This
 *          is used both in input functions (to get additional information about
 *          the input image) and output functions (to specify the proper way of
 *          writing the image).
 * @ingroup MinImgIOAPI
 */
typedef struct {
  ImgFileFormat  iff;               ///< The image file format (see #ImgFileFormat).
  ImgFileComp    comp;              ///< The image file compression (see #ImgFileComp).
  float          xDPI;              ///< The horizontal resolution, in dots-per-inch.
  float          yDPI;              ///< The vertical resolution, in dots-per-inch.
  int            qty;               ///< The resultant image quality (for JPEG compression).
  uint32_t       focal_length_num;  ///< EXIF FocalLength numerator, in millimeters.
  uint32_t       focal_length_den;  ///< EXIF FocalLength denomenator.
} ExtImgProps;


typedef struct {
  MinResult (*initialize)(void *handle, MISInitFlag flag);
  size_t (*read)(void *handle, void* buffer, size_t size);
  size_t (*write)(void *handle, const void* buffer, size_t size);
  int64_t (*lseek)(void *handle, int64_t offset, int whence);
  void (*error)(void *handle, const char* message);
  const char* (*get_source)(void *handle);
  void* handle;
} MIStreamArguments;

typedef void* MIStreamHandle;

#endif // #ifndef MINIMGIO_MINIMGIO_TYPES_H_INCLUDED
