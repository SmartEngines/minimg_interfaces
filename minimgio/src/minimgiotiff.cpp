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

#include <cstring>
#include <algorithm>

#include <minutils/smartptr.h>
#include <minutils/minhelpers.h>
#include <minbase/minresult.h>
#include <minimgapi/imgguard.hpp>
#include <minimgio/minimgio.hpp>
#include <minimgapi/minimgapi.hpp>
#include <minbase/minresult.h>

#include <tiffio.h>
#include <errno.h>
#include <limits>
#include "utils.h"
#include "pack.h"

static void _TIFFClose(TIFF *pTIF) {
  if (pTIF)
    TIFFClose(pTIF);
}

DEFINE_SCOPED_OBJECT(_scoped_tiff_handle, _TIFFClose)
typedef _scoped_tiff_handle<TIFF> scoped_tiff_handle;

DEFINE_SCOPED_OBJECT(_tiff_memory, _TIFFfree)
typedef _tiff_memory<uint8_t> tiff_memory;

template <class T> static void _TIFFGetField(
    TIFF *pTIF,
    ttag_t tag,
    T *pVal,
    const T &defVal) {
  if (!TIFFGetField(pTIF, tag, pVal)) {
    *pVal = defVal;
  }
}

static void MinimgioTIFFError(
    thandle_t handle,
    const char*,
    const char* fmt_str,
    va_list args) {
  char message[4096];
  ::snprintf(message, sizeof(message), fmt_str, args);
  reinterpret_cast<minimgio::BinaryStream*>(handle)->error(message);
}

static tsize_t MinimgioTIFFRead(
    thandle_t handle,
    tdata_t buf,
    tsize_t size) {
  return reinterpret_cast<minimgio::BinaryStream*>(handle)->read(buf, size);
}

static toff_t MinimgioTiffSeek(
    thandle_t handle,
    toff_t off,
    int whence) {
  return reinterpret_cast<minimgio::BinaryStream*>(handle)->lseek(off, whence);
}

static int MinimgioTIFFClose(
    thandle_t /*handle*/){
  return 0;
}

static toff_t MinimgioTIFFSize(
    thandle_t /*handle*/) {
  return 0;
}

static tsize_t MinimgioTIFFWrite(
    thandle_t handle,
    tdata_t buf,
    tsize_t size) {
  return reinterpret_cast<minimgio::BinaryStream*>(handle)->write(buf, size);
}

static inline bool read_nth_bit(uint8_t *src, size_t i) {
  return (src[i / 8] >> (7 - i % 8)) & 1;
}

static int bpc_to_channel_depth(int num_bits) {
  return (num_bits <= 8 ? 1 :
          num_bits <= 16 ? 2 :
          num_bits <= 32 ? 4 : 8);
}

template<typename T>
void _bit_unpack(uint8_t *dst, size_t x, uint64_t val) {
  reinterpret_cast<T*>(dst)[x] = static_cast<T>(val);
}

static void bit_unpack(uint8_t *dst, uint8_t *src, size_t width, int src_bits) {
  int const channel_depth = bpc_to_channel_depth(src_bits);
  ::memset(dst, 0, width * channel_depth);
  void (*f)(uint8_t *dst, size_t x, uint64_t val);
  if (src_bits <= 8)
    f = _bit_unpack<uint8_t>;
  else if (src_bits <= 16)
    f = _bit_unpack<uint16_t>;
  else if (src_bits <= 32)
    f = _bit_unpack<uint32_t>;
  else if (src_bits <= 64)
    f = _bit_unpack<uint64_t>;
  else
    return; // Make compiler happy about uninitialized f. Bits always <= 64.
  for (size_t x = 0; x < width; x++) {
    uint32_t result = 0;
    for (int i = 0; i < src_bits; i++) {
      bool bit = read_nth_bit(src, x * src_bits + i);
      result |= bit << (src_bits - 1 - i);
    }
    // system endianness dependent write to dst
    f(dst, x, result);
  }
}

/// A fast way to find the position of the lowest set bit in a 32-bit interger
/// See Jorg Arndt "Matters Computational" (https://www.jjj.de/fxt/fxtbook.pdf) pp. 13-14
/// Used for computing the log2 of depth
static int LowestBitIndexDeBruijn32(uint32_t x) {
  static const int de_bruijn_table[32] = {
    0, 1, 2, 6, 3, 11, 7, 16, 4, 14, 12, 21, 8, 23, 17, 26,
    31, 5, 10, 15, 13, 20, 22, 25, 30, 9, 19, 24, 29, 18, 28, 27
  };
  return de_bruijn_table[((x & -x) * 0x4653ADFU) >> 27];
}

static inline bool go_rgba(int metr) {
   // uncommon formats
  return metr >= PHOTOMETRIC_YCBCR && metr != PHOTOMETRIC_CFA;
}

namespace minimgio { namespace internal {
MinResult GetPropsTiff(
    MinImg       &img,
    ExtImgProps  *p_props,
    BinaryStream &stream,
    int           page) {
  if (img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (page > std::numeric_limits<uint16_t>::max())
    return MR_CONTRACT_VIOLATION;
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;

  TIFFSetErrorHandler(NULL);
  TIFFSetWarningHandler(NULL);
  TIFFSetErrorHandlerExt(MinimgioTIFFError);
  scoped_tiff_handle pTIF(TIFFClientOpen(
      "MINTIFF", "rmh", static_cast<thandle_t>(&stream), MinimgioTIFFRead,
      MinimgioTIFFWrite, MinimgioTiffSeek, MinimgioTIFFClose,
      MinimgioTIFFSize, NULL, NULL
    ));
  if (!pTIF)
    return MR_ENV_ERROR;

  const int num_pages = TIFFNumberOfDirectories(pTIF);
  if (page >= num_pages)
    return MR_CONTRACT_VIOLATION;
  TIFFSetDirectory(pTIF, static_cast<uint16_t>(page));

  {
    int nc = 0, typ = PLANARCONFIG_CONTIG, metr = 0;
    _TIFFGetField(pTIF, TIFFTAG_SAMPLESPERPIXEL, &nc, 1);
    _TIFFGetField(pTIF, TIFFTAG_PHOTOMETRIC, &metr, 0);
    if (go_rgba(metr))
      nc = 4;  // Because we will be using `TIFFReadRGBAImage`
    if (nc > 1)
      _TIFFGetField(pTIF, TIFFTAG_PLANARCONFIG, &typ, PLANARCONFIG_CONTIG);
    if (typ != PLANARCONFIG_CONTIG)
      return MR_NOT_IMPLEMENTED;

    int wd = 0, ht = 0, bpc = 0;
    _TIFFGetField(pTIF, TIFFTAG_IMAGEWIDTH, &wd, 0);
    _TIFFGetField(pTIF, TIFFTAG_IMAGELENGTH, &ht, 0);
    _TIFFGetField(pTIF, TIFFTAG_BITSPERSAMPLE, &bpc, 0);
    if (bpc < 1 || bpc > 64)
      return MR_NOT_IMPLEMENTED;

    int dtyp = 0;
    _TIFFGetField(pTIF, TIFFTAG_SAMPLEFORMAT, &dtyp, 0);
    switch (dtyp)
    {
    case 0:
    case SAMPLEFORMAT_UINT:
      dtyp = FMT_UINT;
      break;
    case SAMPLEFORMAT_INT:
      dtyp = FMT_INT;
      break;
    case SAMPLEFORMAT_IEEEFP:
      dtyp = FMT_REAL;
      break;
    default:
      return MR_NOT_IMPLEMENTED;
    }
    uint16_t orientation = 0;
    TIFFGetFieldDefaulted(pTIF, TIFFTAG_ORIENTATION, &orientation);
    const bool image_is_transposed = orientation > 4 && orientation < 9;
    if (image_is_transposed) {
      img.width = ht;
      img.height = wd;
    }
    else {
      img.width = wd;
      img.height = ht;
    }
    img.channels = nc;
    /// LowestBitIndexDeBruijn32(pbc) gives log2(pbc)
    img.scalar_type = MinTypeByMinFormatAndLogSize(
        static_cast<MinFmt>(dtyp),
        LowestBitIndexDeBruijn32(bpc));
//    img.channelDepth = bpc == 1 ? 0 : bpc_to_channel_depth(bpc);
//    img.format = (MinFmt)dtyp;
    img.stride = 0;
  }

  if (p_props)
  {
    p_props->iff = IFF_TIFF;
    p_props->comp = IFC_NONE;
    p_props->qty = 0;

    int compression = 0;
    _TIFFGetField(pTIF, TIFFTAG_COMPRESSION, &compression, COMPRESSION_NONE);
    switch(compression)
    {
      case COMPRESSION_LZW: p_props->comp = IFC_LZW; break;
      case COMPRESSION_DEFLATE: p_props->comp = IFC_DEFLATE; break;
      case COMPRESSION_PACKBITS: p_props->comp = IFC_PACKBITS; break;
      case COMPRESSION_JPEG: p_props->comp = IFC_JPEG; break;
      case COMPRESSION_CCITTRLE: p_props->comp = IFC_RLE; break;
      case COMPRESSION_CCITTFAX3: p_props->comp = IFC_GROUP3; break;
      case COMPRESSION_CCITTFAX4: p_props->comp = IFC_GROUP4; break;
      default: p_props->comp = IFC_NONE;
    }

    int unit = 0;
    _TIFFGetField(pTIF, TIFFTAG_XRESOLUTION, &p_props->xDPI, 0.f);
    _TIFFGetField(pTIF, TIFFTAG_YRESOLUTION, &p_props->yDPI, 0.f);
    _TIFFGetField(pTIF, TIFFTAG_RESOLUTIONUNIT, &unit, 0);
    switch (unit)
    {
    case RESUNIT_INCH:
      break;
    case RESUNIT_CENTIMETER:
      p_props->xDPI *= 2.54f;
      p_props->yDPI *= 2.54f;
      break;
    default:
      p_props->xDPI = 0.f;
      p_props->yDPI = 0.f;
    }
  }

  return MR_SUCCESS;
}

MinResult GetNumPagesTiff(
    int          &pages,
    BinaryStream &stream) {
  stream.initialize(MIS_READONLY);
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  TIFFSetErrorHandler(NULL);
  TIFFSetWarningHandler(NULL);
  TIFFSetErrorHandlerExt(NULL);  // don't show read error
  scoped_tiff_handle pTIF(TIFFClientOpen(
      "MINTIFF", "rm", static_cast<thandle_t>(&stream),
      MinimgioTIFFRead, MinimgioTIFFWrite, MinimgioTiffSeek,
      MinimgioTIFFClose, MinimgioTIFFSize, NULL, NULL
    ));
  if (!pTIF)
    return MR_ENV_ERROR;
  const int tiff_pages = TIFFNumberOfDirectories(pTIF);
  if (tiff_pages < 0)
    return MR_INTERNAL_ERROR;
  pages = tiff_pages;
  return MR_SUCCESS;
}

MinResult LoadTiff(
    MinImg const &img,
    BinaryStream &stream,
    int           page) {
  if (page < 0)
    return MR_CONTRACT_VIOLATION;
  if (!img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (page > std::numeric_limits<uint16_t>::max())
    return MR_CONTRACT_VIOLATION;

  TIFFSetErrorHandler(NULL);
  TIFFSetWarningHandler(NULL);
  TIFFSetErrorHandlerExt(MinimgioTIFFError);
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  scoped_tiff_handle pTIF(TIFFClientOpen(
      "MINTIFF", "rm", static_cast<thandle_t>(&stream),
      MinimgioTIFFRead, MinimgioTIFFWrite, MinimgioTiffSeek,
      MinimgioTIFFClose, MinimgioTIFFSize, NULL, NULL
    ));
  if (!pTIF)
    return MR_ENV_ERROR;
  const int num_pages = TIFFNumberOfDirectories(pTIF);
  if (page >= num_pages)
    return MR_CONTRACT_VIOLATION;
  TIFFSetDirectory(pTIF, static_cast<uint16_t>(page));

  int nc = 0, typ = PLANARCONFIG_CONTIG, metr = PHOTOMETRIC_MINISWHITE;
  _TIFFGetField(pTIF, TIFFTAG_SAMPLESPERPIXEL, &nc, 1);
  _TIFFGetField(pTIF, TIFFTAG_PHOTOMETRIC, &metr, PHOTOMETRIC_MINISWHITE);
  if (go_rgba(metr))
    nc = 4;  // Because we will be using `TIFFReadRGBAImage`
  if (nc > 1)
    _TIFFGetField(pTIF, TIFFTAG_PLANARCONFIG, &typ, PLANARCONFIG_CONTIG);
  if (typ != PLANARCONFIG_CONTIG)
    return MR_NOT_IMPLEMENTED;

  int wd = 0, ht = 0, bpc = 0;
  _TIFFGetField(pTIF, TIFFTAG_IMAGEWIDTH, &wd, 0);
  _TIFFGetField(pTIF, TIFFTAG_IMAGELENGTH, &ht, 0);
  _TIFFGetField(pTIF, TIFFTAG_BITSPERSAMPLE, &bpc, 0);
  if (bpc < 1 || bpc > 64)
    return MR_NOT_IMPLEMENTED;

  uint16_t orientation = 0;
  TIFFGetFieldDefaulted(pTIF, TIFFTAG_ORIENTATION, &orientation);
  bool const image_is_transposed = orientation > 4 && orientation < 9;
  if (image_is_transposed)
    std::swap(wd, ht);
  if (img.channels != nc)
    return MR_CONTRACT_VIOLATION;
  if (img.height < ht || img.width < wd)
    return MR_CONTRACT_VIOLATION;
  // uint8_t image can be represented as TYP_UINT1 or TYP_UINT8:
  if (bpc == 1 && img.scalar_type != TYP_UINT1 && img.scalar_type != TYP_UINT8)
    return MR_CONTRACT_VIOLATION;
  // also, we can convert 4 bit images to 8 bit, so loosely check bit depth:
  if (bpc > 1 && ByteSizeOfMinType(img.scalar_type) != bpc_to_channel_depth(bpc))
    return MR_CONTRACT_VIOLATION;

  if (metr == PHOTOMETRIC_MINISWHITE && bpc != 1)
    return MR_NOT_IMPLEMENTED;  // must be bad format, really

  int dtyp = SAMPLEFORMAT_UINT;
  MinFmt const image_format = MinFormatOfMinType(img.scalar_type);
  _TIFFGetField(pTIF, TIFFTAG_SAMPLEFORMAT, &dtyp, SAMPLEFORMAT_UINT);
  switch (dtyp)
  {
  case SAMPLEFORMAT_UINT:
    if (image_format != FMT_UINT)
      return MR_CONTRACT_VIOLATION;
    break;
  case SAMPLEFORMAT_INT:
    if (image_format != FMT_INT)
      return MR_CONTRACT_VIOLATION;
    break;
  case SAMPLEFORMAT_IEEEFP:
    if (image_format != FMT_REAL)
      return MR_CONTRACT_VIOLATION;
    break;
  default:
    return MR_NOT_IMPLEMENTED;
  }
  MinImg const* p_read_img = &img;
  DECLARE_GUARDED_MINIMG(transposed_img);
  if (image_is_transposed) {
    MR_PROPAGATE_ERROR(minimg::CloneTransposedPrototype(transposed_img, img, AO_EMPTY));
    MR_PROPAGATE_ERROR(minimg::AllocImage(transposed_img, sizeof(uint32_t))); // for RGBA
    p_read_img = &transposed_img;
  }
  if (go_rgba(metr)) {
    const bool is_not_solid = minimg::AssureImageIsSolid<CT_INFO>(*p_read_img) != MR_YES;
    MinImg const* p_solid_img = p_read_img;
    if (is_not_solid) {
      if (image_is_transposed)
        return MR_INTERNAL_ERROR;
      MR_PROPAGATE_ERROR(minimg::ClonePrototype(transposed_img, img, AO_EMPTY));
      MR_PROPAGATE_ERROR(minimg::AllocImage(transposed_img, sizeof(uint32_t)));
      p_solid_img = &transposed_img;
    }
    if (!image_is_transposed) {// libtiff only does flips and supports solid images
      if (!TIFFReadRGBAImageOriented(pTIF, p_solid_img->width, p_solid_img->height,
                                     reinterpret_cast<uint32_t*>(p_solid_img->p_zero_line),
                                     ORIENTATION_LEFTTOP, 1))
        return MR_ENV_ERROR;  // most likely error;
      if (is_not_solid)
        CopyMinImage(p_read_img, p_solid_img);
      return MR_SUCCESS;
    }
    // read image as is
    if (!TIFFReadRGBAImageOriented(pTIF, p_solid_img->width, p_solid_img->height,
                                   reinterpret_cast<uint32_t*>(p_solid_img->p_zero_line),
                                   orientation, 1))
      return MR_ENV_ERROR;  // most likely error;
  }
  else {
    if (!TIFFIsTiled(pTIF))
    {
      const tsize_t scan_size = TIFFScanlineSize(pTIF);
      tiff_memory p_scan_line(reinterpret_cast<uint8_t*>(_TIFFmalloc(scan_size)));
      if (!p_scan_line)
        return MR_ENV_ERROR;
      const int byte_width = GetMinImageBytesPerLine(p_read_img);
      if (byte_width < scan_size)
        return MR_INTERNAL_ERROR;

      const bool invert = (metr == PHOTOMETRIC_MINISWHITE);
      const size_t len = static_cast<size_t>(wd) * nc;
      uint8_t *p_dst_line = p_read_img->p_zero_line;
      bool const unpack_bits =
        bpc > 1 && bpc != 8 && bpc != 16 && bpc != 32 && bpc != 64;

      for (int y = 0; y < p_read_img->height; y++)
      {
        if (TIFFReadScanline(pTIF, p_scan_line, y) != 1)
          return MR_ENV_ERROR;
        if (bpc == 1 && p_read_img->scalar_type == TYP_UINT1)
          CopyBits(p_dst_line, p_scan_line, len, invert);
        else if (bpc == 1 && p_read_img->scalar_type == TYP_UINT8)
          UnpackLine(p_dst_line, p_scan_line, len, invert);
        else if (unpack_bits)
          bit_unpack(p_dst_line, p_scan_line, len, bpc);
        else
          ::memcpy(p_dst_line, p_scan_line, scan_size);
        p_dst_line += p_read_img->stride;
      }
    }
    else
    {
      // code below is tested on one image and isn't universal
      uint32_t tile_width, tile_height;
      if (!TIFFGetField(pTIF, TIFFTAG_TILEWIDTH, &tile_width) ||
          !TIFFGetField(pTIF, TIFFTAG_TILELENGTH, &tile_height)) {
        return MR_ENV_ERROR;
      }
      tiff_memory buf(reinterpret_cast<uint8_t*>(_TIFFmalloc(TIFFTileSize(pTIF))));
      if (!buf)
        return MR_INTERNAL_ERROR;
      const int pix_size = (bpc >> 3) * nc;
      for (int tile_y = 0; tile_y < ht; tile_y += tile_height)
      {
        const int end_y = std::min(static_cast<int>(tile_y + tile_height), ht);
        for (int tile_x = 0; tile_x < wd; tile_x += tile_width)
        {
          if (TIFFReadTile(pTIF, buf, tile_x, tile_y, 0, 0) == -1)
            return MR_ENV_ERROR;
          const size_t tile_line_size = tile_width * pix_size;
          for (int y = tile_y; y < end_y; ++y)
          {
            uint8_t const* p_src_line = buf + (y - tile_y) * tile_line_size;
            uint8_t *dst_line = (
              p_read_img->p_zero_line +
              p_read_img->stride * y +
              tile_x * pix_size
            );
            ::memcpy(dst_line, p_src_line, tile_line_size);
          }
        }
      }
    }
  }
  return static_cast<MinResult>(
    OrientImageSpecial(img, *p_read_img, orientation));
}
} // namespace internal
} // namespace minimgio

#ifdef MINIMGIO_GENERATE
struct TiffData
{
  int samples_per_pixel;
  int width;
  int height;
  int bytes_per_sample;
  int photometric;
  int sample_format;
  uint8_t *scan_lines;
  float xresolution;
  float yresolution;
  int resolution_unit;
  int compression_type;
  int jpeg_quality;
  tmsize_t scanLen;

  TiffData():
    samples_per_pixel(0),
    width(-1),
    height(-1),
    bytes_per_sample(0),
    photometric(0),
    sample_format(SAMPLEFORMAT_UINT),
    scan_lines(nullptr),
    xresolution(0.0f),
    yresolution(0.0f),
    resolution_unit(RESUNIT_INCH),
    compression_type(COMPRESSION_NONE),
    jpeg_quality(90),
    scanLen(0) { }

  ~TiffData()
  {
    if (scan_lines)
      delete [] scan_lines;
    scan_lines = 0;
  }
};

static MinResult AddPageToTiffExImpl(
    TIFF              *pTIF,
    const MinImg      &img,
    const ExtImgProps *pProps) {
  int image_scalar_size_logbits = LogBitSizeOfMinType(img.scalar_type);
  if (image_scalar_size_logbits < 0)
    return MR_CONTRACT_VIOLATION;
  int bpp = 1 << image_scalar_size_logbits;
//  int bpp = std::max(1, (int)img.channelDepth * 8);
  if (pProps != NULL)
  {
    switch (pProps->comp)
    {
      case IFC_RLE:
      case IFC_GROUP3:
      case IFC_GROUP4:
        bpp = 1;
        break;
      case IFC_JPEG:
        bpp = std::max(8, bpp);
        break;
      default:
        break;
    }
  }

  TIFFSetField(pTIF, TIFFTAG_IMAGELENGTH, img.height);
  TIFFSetField(pTIF, TIFFTAG_IMAGEWIDTH, img.width);
  TIFFSetField(pTIF, TIFFTAG_BITSPERSAMPLE, bpp);
  TIFFSetField(pTIF, TIFFTAG_SAMPLESPERPIXEL, img.channels);
  TIFFSetField(pTIF, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  // FIXME: should be clarified for non-standard as soon as possible!
  if (img.channels == 3)
    TIFFSetField(pTIF, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  else
    TIFFSetField(pTIF, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

  const MinFmt image_format = MinFormatOfMinType(img.scalar_type);
  switch (image_format)
//  switch (img.format)
  {
    case FMT_UINT:
      TIFFSetField(pTIF, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
      break;
    case FMT_INT:
      TIFFSetField(pTIF, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
      break;
    case FMT_REAL:
      TIFFSetField(pTIF, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
      break;
    default:
      return MR_CONTRACT_VIOLATION;
  }

  if (pProps)
  {
    TIFFSetField(pTIF, TIFFTAG_XRESOLUTION, pProps->xDPI);
    TIFFSetField(pTIF, TIFFTAG_YRESOLUTION, pProps->yDPI);
    TIFFSetField(pTIF, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

    switch(pProps->comp)
    {
      case IFC_LZW: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_LZW); break;
      case IFC_DEFLATE: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE); break;
      case IFC_PACKBITS: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS); break;
      case IFC_JPEG: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_JPEG); break;
      case IFC_RLE: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_CCITTRLE); break;
      case IFC_GROUP3: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX3); break;
      case IFC_GROUP4: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4); break;
      default: TIFFSetField(pTIF, TIFFTAG_COMPRESSION, COMPRESSION_NONE); break;
    }

    if (pProps->comp == IFC_JPEG && pProps->qty >= 0 && pProps->qty <= 100)
      TIFFSetField(pTIF, TIFFTAG_JPEGQUALITY, pProps->qty);
  }
  uint8_t *line = img.p_zero_line;

  if (bpp == 1 && image_scalar_size_logbits > 0)
//  if (bpp == 1 && img.channelDepth > 0)
  {
    if (image_scalar_size_logbits > 3 || img.channels > 1 || image_format != FMT_UINT)
      return MR_NOT_IMPLEMENTED;
//    if (img.channelDepth > 1 || img.channels > 1 || img.format != FMT_UINT)
//      return MR_NOT_IMPLEMENTED;

    const uint8_t level = 128;
    size_t size = (img.width + 7) / 8;
    scoped_cpp_array<uint8_t> pBuf(new uint8_t[size]);

    for (int y = 0; y < img.height; y++) {
      PackLine(pBuf, line, level, img.width, false);
      if (TIFFWriteScanline(pTIF, pBuf, y, 0) < 0)
        return MR_ENV_ERROR;
      line += img.stride;
    }
  }
  else if (bpp == 8 && image_scalar_size_logbits == 0)
//  else if (bpp == 8 && img.channelDepth == 0)
  {
    if (img.channels > 1 || image_format != FMT_UINT)
      return MR_NOT_IMPLEMENTED;
//    if (img.channels > 1 || img.format != FMT_UINT)
//      return MR_NOT_IMPLEMENTED;

    const int size = img.width;
    scoped_cpp_array<uint8_t> pBuf(new uint8_t[size]);

    for (int y = 0; y < img.height; ++y) {
      UnpackLine(pBuf, line, img.width, false);
      if (TIFFWriteScanline(pTIF, pBuf, y, 0) < 0)
        return MR_ENV_ERROR;
      line += img.stride;
    }
  }
  else
  {
    for (int y = 0; y < img.height; ++y) {
      if (TIFFWriteScanline(pTIF, line, y, 0) < 0)
        return MR_ENV_ERROR;
      line += img.stride;
    }
  }

  return MR_SUCCESS;
}


namespace minimgio {
namespace internal {
MinResult SaveTiff(
    BinaryStream      &stream,
    const MinImg      &img,
    const ExtImgProps *p_props,
    int                page) {
  if (page < 0)
    return MR_CONTRACT_VIOLATION;
  if (!img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (page > std::numeric_limits<uint16_t>::max())
    return MR_CONTRACT_VIOLATION;

  TIFFSetErrorHandler(NULL);
  TIFFSetWarningHandler(NULL);
  TIFFSetErrorHandlerExt(MinimgioTIFFError);
  MR_PROPAGATE_ERROR(stream.initialize(MIS_READWRITE));
  int num_pages = 0;
  GetNumPagesTiff(num_pages, stream);  // just try, ignore errors;
  if (page > num_pages || page < 0)
    return MR_CONTRACT_VIOLATION;
  scoped_cpp_array<TiffData> tiff_pages(new TiffData[num_pages]);
  if (page < num_pages)
  {
    if (stream.lseek(0) != 0)
      return MR_ENV_ERROR;

    scoped_tiff_handle pTIF(TIFFClientOpen(
        "MINTIFF", "rm", static_cast<thandle_t>(&stream),
        MinimgioTIFFRead, MinimgioTIFFWrite, MinimgioTiffSeek,
        MinimgioTIFFClose, MinimgioTIFFSize, NULL, NULL
      ));
    if (!pTIF)
      return MR_ENV_ERROR;

    for (int k = 0; k < num_pages; k++)
    {
      TiffData &data = tiff_pages[k];
      TIFFSetDirectory(pTIF, static_cast<uint16_t>(k));
      int &nc = data.samples_per_pixel;
      int typ = PLANARCONFIG_CONTIG;
      _TIFFGetField(pTIF, TIFFTAG_SAMPLESPERPIXEL, &nc, 1);
      if (nc > 1)
        _TIFFGetField(pTIF, TIFFTAG_PLANARCONFIG, &typ, PLANARCONFIG_CONTIG);
      if (typ != PLANARCONFIG_CONTIG)
        return MR_NOT_IMPLEMENTED;

      int &bpc = data.bytes_per_sample;
      _TIFFGetField(pTIF, TIFFTAG_IMAGEWIDTH, &data.width, 0);
      _TIFFGetField(pTIF, TIFFTAG_IMAGELENGTH, &data.height, 0);
      _TIFFGetField(pTIF, TIFFTAG_BITSPERSAMPLE, &bpc, 0);

      if (bpc != 1 && bpc & 7)
        return MR_NOT_IMPLEMENTED;
      bpc = bpc >> 3;

      int &metr = data.photometric;
      _TIFFGetField(pTIF, TIFFTAG_PHOTOMETRIC, &metr, PHOTOMETRIC_MINISWHITE);
      if (metr == PHOTOMETRIC_MINISWHITE && bpc > 0)
        return MR_NOT_IMPLEMENTED;

      _TIFFGetField(pTIF, TIFFTAG_SAMPLEFORMAT, &data.sample_format, SAMPLEFORMAT_UINT);
      _TIFFGetField(pTIF, TIFFTAG_XRESOLUTION, &data.xresolution, 0.0f);
      _TIFFGetField(pTIF, TIFFTAG_YRESOLUTION, &data.yresolution, 0.0f);
      _TIFFGetField(pTIF, TIFFTAG_RESOLUTIONUNIT, &data.resolution_unit, RESUNIT_INCH);
      _TIFFGetField(pTIF, TIFFTAG_COMPRESSION, &data.compression_type, COMPRESSION_NONE);
      _TIFFGetField(pTIF, TIFFTAG_JPEGQUALITY, &data.jpeg_quality, 100);
      const tmsize_t scanLen = TIFFScanlineSize(pTIF);
      data.scan_lines = new uint8_t[data.height * scanLen];
      data.scanLen = scanLen;
      for (int y = 0; y < data.height; y++)
      {
        void * pScanLine = data.scan_lines + y * scanLen;
        if (TIFFReadScanline(pTIF, pScanLine, y) != 1)
          return MR_ENV_ERROR;
      }
    }
   }

  char mode[] = "amh";
  if (page < num_pages)
    mode[0] = 'w';
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  scoped_tiff_handle pTIF(TIFFClientOpen(
      "MINTIFF", mode, static_cast<thandle_t>(&stream),
      MinimgioTIFFRead, MinimgioTIFFWrite, MinimgioTiffSeek,
      MinimgioTIFFClose, MinimgioTIFFSize, NULL, NULL
    ));
  if (!pTIF)
    return MR_ENV_ERROR;

  if (page == num_pages)
  {
    TIFFSetField(pTIF, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
    TIFFSetField(pTIF, TIFFTAG_PAGENUMBER, page, num_pages + 1 );
    MR_PROPAGATE_ERROR(AddPageToTiffExImpl(pTIF, img, p_props) );
    if (!TIFFWriteDirectory (pTIF))
      return MR_ENV_ERROR;
    return MR_SUCCESS;
  }

  for (int k = 0; k < num_pages; k++)
  {
    TIFFSetField(pTIF, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
    if (k == page)
    {
      TIFFSetField(pTIF, TIFFTAG_PAGENUMBER, page, num_pages );
      MR_PROPAGATE_ERROR(AddPageToTiffExImpl(pTIF, img, p_props) );
      if (!TIFFWriteDirectory(pTIF))
        return MR_ENV_ERROR;
      continue;
    }

    TIFFSetField(pTIF, TIFFTAG_PAGENUMBER, k, num_pages );
    TiffData const& data = tiff_pages[k];
    int bpp = std::max(1, data.bytes_per_sample * 8);
    switch (data.compression_type)
    {
      case COMPRESSION_CCITTRLE:
      case COMPRESSION_CCITTFAX3:
      case COMPRESSION_CCITTFAX4:
        bpp = 1;
        break;
      case COMPRESSION_JPEG:
        bpp = std::max(8, bpp);
        break;
      default:
        break;
    }

    TIFFSetField(pTIF, TIFFTAG_IMAGELENGTH, data.height);
    TIFFSetField(pTIF, TIFFTAG_IMAGEWIDTH, data.width);
    TIFFSetField(pTIF, TIFFTAG_BITSPERSAMPLE, bpp);
    TIFFSetField(pTIF, TIFFTAG_SAMPLESPERPIXEL, data.samples_per_pixel );
    TIFFSetField(pTIF, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(pTIF, TIFFTAG_PHOTOMETRIC, data.photometric);
    TIFFSetField(pTIF, TIFFTAG_SAMPLEFORMAT, data.sample_format);
    TIFFSetField(pTIF, TIFFTAG_XRESOLUTION, data.xresolution);
    TIFFSetField(pTIF, TIFFTAG_YRESOLUTION, data.yresolution);
    TIFFSetField(pTIF, TIFFTAG_RESOLUTIONUNIT, data.resolution_unit);
    TIFFSetField(pTIF, TIFFTAG_COMPRESSION, data.compression_type);

    if (data.compression_type == COMPRESSION_JPEG && data.jpeg_quality >= 0 && data.jpeg_quality <= 100)
      TIFFSetField(pTIF, TIFFTAG_JPEGQUALITY, data.jpeg_quality);

    for (int y = 0; y < data.height; ++y)
    {
      uint8_t * pBuf = data.scan_lines + y * data.scanLen;
      if (TIFFWriteScanline(pTIF, pBuf, y, 0) < 0)
        return MR_ENV_ERROR;
    }
    if (!TIFFWriteDirectory(pTIF))
      return MR_ENV_ERROR;
  }
  return MR_SUCCESS;
}
}  // namespace internal
}  // namespace minimgio
#endif  // #ifdef MINIMGIO_GENERATE
