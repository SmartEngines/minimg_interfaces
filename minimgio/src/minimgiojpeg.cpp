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

#include <cstdio>
#include <csetjmp>
#include <cstring>
#include <algorithm>  // std::min

#include <minutils/smartptr.h>  // DEFINE_SCOPED_OBJECT
#include <minimgapi/imgguard.hpp>
#include <minimgio/minimgio.hpp>
#include <minimgapi/minimgapi.hpp>

#include "utils.h"

#include <jpeglib.h>
#include <jerror.h>
#include <fcntl.h>

// redefine libjpeg macros that trigger warnings
#undef WARNMS
#define WARNMS(cinfo, code) \
  ((cinfo)->err->msg_code = (code), \
   (*(cinfo)->err->emit_message) (reinterpret_cast<j_common_ptr>(cinfo), -1))

#undef ERREXIT
#define ERREXIT(cinfo, code) \
  ((cinfo)->err->msg_code = (code), \
   (*(cinfo)->err->error_exit) (reinterpret_cast<j_common_ptr>(cinfo)))


#undef jpeg_create_decompress
#define jpeg_create_decompress(cinfo) \
  jpeg_CreateDecompress((cinfo), JPEG_LIB_VERSION, \
                        sizeof(struct jpeg_decompress_struct))

#undef jpeg_create_compress
#define jpeg_create_compress(cinfo) \
  jpeg_CreateCompress((cinfo), JPEG_LIB_VERSION, \
                      sizeof(struct jpeg_compress_struct))

DEFINE_SCOPED_OBJECT(_scoped_ljobj, jpeg_destroy_decompress)
typedef _scoped_ljobj<jpeg_decompress_struct> scoped_ljobj;

DEFINE_SCOPED_OBJECT(_scoped_sjobj, jpeg_destroy_compress)
typedef _scoped_sjobj<jpeg_compress_struct> scoped_sjobj;

// Error handling structures.
typedef struct
{
  jpeg_error_mgr pub;
  jmp_buf buf;
} jem;

METHODDEF(void) jee(j_common_ptr cinfo)
{
  jem *pJem = reinterpret_cast<jem*>(cinfo->err);
  longjmp(pJem->buf, 1);
}

METHODDEF(void) jom(j_common_ptr /* cinfo */)
{
}

static uint16_t read_uint16(uint8_t const* data, bool is_little_endian)
{
  if (is_little_endian)
    return static_cast<uint16_t>((data[1] << 8) + data[0]);
  else
    return static_cast<uint16_t>((data[0] << 8) + data[1]);
}

static uint32_t read_uint32(uint8_t const* data, bool is_little_endian)
{
  if (is_little_endian)
    return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
  else
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

// READING
typedef struct {
  struct jpeg_source_mgr pub;
  minimgio::BinaryStream *stream;
  JOCTET buffer[4096];
} minimgio_source_mgr;

// WRITING
typedef struct {
  jpeg_destination_mgr pub;
  minimgio::BinaryStream *stream;
  JOCTET buffer[4096];
} minimgio_destination_mgr;

static void minimgio_noop(j_decompress_ptr /*cinfo*/)
{
}

// copy from jpeg-turbo/jdatasrc.c
static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
  struct jpeg_source_mgr *src = cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > static_cast<long>(src->bytes_in_buffer)) {
      num_bytes -= static_cast<long>(src->bytes_in_buffer);
      (void)(*src->fill_input_buffer) (cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->next_input_byte += static_cast<size_t>(num_bytes);
    src->bytes_in_buffer -= static_cast<size_t>(num_bytes);
  }
}

static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
  minimgio_source_mgr *src = reinterpret_cast<minimgio_source_mgr*>(cinfo->src);
  size_t nbytes;

  nbytes = src->stream->read(src->buffer, sizeof(src->buffer));

  if (nbytes == 0) {
    // minimgio won't have zero lenght files here
    //if (src->start_of_file)     /* Treat empty input file as fatal error */
    //  ERREXIT(cinfo, JERR_INPUT_EMPTY);
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->buffer[0] = static_cast<JOCTET>(0xFF);
    src->buffer[1] = static_cast<JOCTET>(JPEG_EOI);
    nbytes = 2;
  }

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;

  return TRUE;
}

static void RecursiveDecodeEXIF(
    ExtImgProps         *p_props,
    uint16_t            &orientation,
    const uint8_t *const data,
    uint32_t             ifd_offset,
    bool                 is_little_endian,
    uint32_t             max_num_tags,
    int32_t              recursion_limit = 15) {
  static const uint16_t EXIF_TAG_ORIENTATION = 0x0112;
  static const uint16_t EXIF_TYPE_SHORT = 0x0003;
  static const uint16_t EXIF_TAG_FOCAL_LENGTH = 0x920A;
  static const uint16_t EXIF_TYPE_RATIONAL64U = 0x0005;
  static const uint16_t EXIF_TAG_IFD = 0x8769;
  static const uint16_t EXIF_TYPE_LONG = 0x0004;

  const uint8_t* cur_ptr = data + ifd_offset;
  const uint32_t num_tags = std::min<uint32_t>(
      read_uint16(cur_ptr, is_little_endian), max_num_tags);
  cur_ptr += sizeof(uint16_t);
  for (uint32_t tag_idx = 0; tag_idx < num_tags; ++tag_idx) {
    const uint16_t tag = read_uint16(cur_ptr, is_little_endian);
    const uint16_t type = read_uint16(cur_ptr + 2, is_little_endian);
    const uint32_t count = read_uint32(cur_ptr + 4, is_little_endian);
    if (tag == EXIF_TAG_ORIENTATION && type == EXIF_TYPE_SHORT && count == 1) {
      orientation = read_uint16(cur_ptr + 8, is_little_endian);
    } else if (tag == EXIF_TAG_FOCAL_LENGTH && type == EXIF_TYPE_RATIONAL64U &&
               count == 1) {
      uint32_t offset = read_uint32(cur_ptr + 8, is_little_endian);
      p_props->focal_length_num = read_uint32(data + offset, is_little_endian);
      p_props->focal_length_den = read_uint32(data + offset + 8, is_little_endian);
    } else if (tag == EXIF_TAG_IFD && type == EXIF_TYPE_LONG && count == 1) {
      uint32_t nxt_ifd_offset = read_uint32(cur_ptr + 8, is_little_endian);
      /// Check that next ifd is located after current ifd tags
      /// and not too deep recursion
      if (recursion_limit && nxt_ifd_offset >= ifd_offset + num_tags * 12)
        RecursiveDecodeEXIF(p_props, orientation, data, nxt_ifd_offset,
            is_little_endian, max_num_tags - num_tags, recursion_limit - 1);
    }
    cur_ptr += 12;
  }
}


/// returns orientation
static uint16_t decodeExtProps(
    ExtImgProps            *p_props,
    jpeg_decompress_struct *jds) {
  p_props->iff = IFF_JPEG;
  p_props->comp = IFC_NONE;
  p_props->qty = 0;
  switch (jds->density_unit)
  {
  case 1:
    p_props->xDPI = jds->X_density;
    p_props->yDPI = jds->Y_density;
    break;
  case 2:
    p_props->xDPI = jds->X_density * 2.54f;
    p_props->yDPI = jds->Y_density * 2.54f;
    break;
  default:
    p_props->xDPI = 0.f;
    p_props->yDPI = 0.f;
  }

  static const uint8_t exif[] = { 'E', 'x', 'i', 'f', '\0', '\0' };
  static const uint8_t big_endian[] = { 'M','M' };
  static const uint8_t little_endian[] = { 'I','I' };
  static const uint32_t header_sz =
    sizeof(exif) + sizeof(little_endian) + sizeof(uint16_t) + sizeof(uint32_t);

  uint16_t orientation = 0;

  for (jpeg_saved_marker_ptr marker = jds->marker_list; marker; marker = marker->next) {
    uint32_t sz = marker->data_length;
    if (sz < header_sz)
      continue;
    // shift sz, so that it's right after header
    sz -= header_sz;
    uint8_t const* cur_ptr = marker->data;
    // check chat it is exif header
    if (memcmp(cur_ptr, exif, sizeof(exif)) != 0)
      continue;
    cur_ptr += sizeof(exif);
    // read endianness
    bool is_little_endian;
    if (memcmp(cur_ptr, little_endian, sizeof(little_endian)) == 0)
      is_little_endian = true;
    else if (memcmp(cur_ptr, big_endian, sizeof(big_endian)) == 0)
      is_little_endian = false;
    else
      continue;

    const uint8_t* const data = cur_ptr;

    cur_ptr += sizeof(little_endian);
    if (read_uint16(cur_ptr, is_little_endian) != 42)
      continue;
    cur_ptr += sizeof(uint16_t);

    // read offset to the data
    uint32_t offset = read_uint32(cur_ptr, is_little_endian);
    cur_ptr += sizeof(uint32_t);
    // if offset points inside the header, continue
    if (offset < sizeof(little_endian) + sizeof(uint16_t) + sizeof(uint32_t))
      continue;
    // make offset relative to the cur_ptr position
    uint32_t offset_from_cur_ptr =
        offset - sizeof(little_endian) - sizeof(uint16_t) - sizeof(uint32_t);
    // check that we can read the number of tags
    if (offset_from_cur_ptr + sizeof(uint16_t) > sz)
      continue;
    cur_ptr += offset;
    RecursiveDecodeEXIF(p_props, orientation, data, offset,
        is_little_endian, (sz - 2 - offset_from_cur_ptr) / 12);
  }

  if (orientation > 4 && orientation <= 8)
    std::swap(p_props->xDPI, p_props->yDPI);

  return orientation;
}

static MinResult decodeImgProps(
    MinImg                 &img,
    jpeg_decompress_struct &jds,
    uint16_t                orientation) {
  if (img.p_zero_line)
    return MR_CONTRACT_VIOLATION;
  if (orientation > 4 && orientation <= 8) {
    img.width = jds.output_height;
    img.height = jds.output_width;
  }
  else {
    img.width = jds.output_width;
    img.height = jds.output_height;
  }
  img.channels = jds.output_components;
  img.scalar_type = TYP_UINT8;
  img.stride = 0;

  return MR_SUCCESS;
}

static MinResult decodeImgLoad(
    const MinImg           &img,
    jpeg_decompress_struct &jds,
    uint16_t                orientation) {
  if (img.scalar_type != TYP_UINT8 || img.channels != jds.output_components)
    return MR_CONTRACT_VIOLATION;
  int width = jds.output_width;
  int height = jds.output_height;
  if (orientation > 4 && orientation <= 8)
    std::swap(width, height);
  if (img.width < width || img.height < height)
    return MR_CONTRACT_VIOLATION;

  DECLARE_GUARDED_MINIMG(t_img);
  const MinImg *p_read_img = &img;
  if (orientation > 4 && orientation < 9) {
    MR_PROPAGATE_ERROR(
      minimg::CloneTransposedPrototype(t_img, img, AO_PREALLOCATED));
    p_read_img = &t_img;
  }

  JSAMPROW ppBuf[1] = {};
  JSAMPROW line = p_read_img->p_zero_line;
  for (int y = 0; y < static_cast<int>(jds.output_height); y++) {
    ppBuf[0] = static_cast<JSAMPROW>(line);
    jpeg_read_scanlines(&jds, ppBuf, 1);
    line += p_read_img->stride;
  }
  return static_cast<MinResult>(
    OrientImageSpecial(img, *p_read_img, orientation));
}


static MinResult GetJpegPropsEx(
    MinImg                 &img,
    ExtImgProps            *p_props,
    minimgio::BinaryStream &stream,
    int                     page,
    bool                    decode_image) {
  if (page != 0)
    return MR_CONTRACT_VIOLATION;
  // Instantiate decoding environment.
  jpeg_decompress_struct cinfo = {};
  jem jerr;
  ::memset(&jerr, 0, sizeof(jerr));
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jee;
  jerr.pub.output_message = jom;

  if (setjmp(jerr.buf)) {
    jpeg_destroy_decompress(&cinfo);
    return MR_ENV_ERROR;
  }

  jpeg_create_decompress(&cinfo);
  scoped_ljobj guard(&cinfo);

  minimgio_source_mgr src_mgr;

  cinfo.src = &src_mgr.pub;
  src_mgr.pub.init_source = minimgio_noop;
  src_mgr.pub.fill_input_buffer = fill_input_buffer;
  src_mgr.pub.skip_input_data = skip_input_data;
  src_mgr.pub.resync_to_restart = jpeg_resync_to_restart;
  src_mgr.pub.term_source = minimgio_noop;
  src_mgr.stream = &stream;
  src_mgr.pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src_mgr.pub.next_input_byte = NULL; /* until buffer loaded */

  jpeg_save_markers(&cinfo, JPEG_APP0 + 1, 0xFFFF);
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);


  ExtImgProps local_props = {};
  ExtImgProps* p_work_props = p_props ? p_props : &local_props;
  uint16_t orientation = decodeExtProps(p_work_props, &cinfo);

  if (decode_image)
  {
    MR_PROPAGATE_ERROR(decodeImgLoad(img, cinfo, orientation));
    if (jpeg_finish_decompress(&cinfo) != TRUE)  // request for suspension
      return MR_NOT_IMPLEMENTED;
  }
  else {
    MR_PROPAGATE_ERROR(decodeImgProps(img, cinfo, orientation));
    jpeg_abort_decompress(&cinfo);
  }
  return MR_SUCCESS;
}

namespace minimgio { namespace internal {
MinResult GetPropsJpeg(
    MinImg                &img,
    ExtImgProps           *p_props,
    BinaryStream &stream,
    int                    page) {
  if (page != 0)
    return MR_CONTRACT_VIOLATION;
  return GetJpegPropsEx(img, p_props, stream, page, false);
};

MinResult GetNumPagesJpeg(int &num_pages, BinaryStream &) {
  num_pages = 1;
  return MR_SUCCESS;
}
MinResult LoadJpeg(
    MinImg const &img,
    BinaryStream &stream,
    int          page) {
  if (page != 0)
    return MR_CONTRACT_VIOLATION;
  // const_cast is safe because the image isn't changed when
  // decode_image=true; removing const_cast requires more code
  return GetJpegPropsEx(const_cast<MinImg&>(img), NULL, stream, page, true);
}
}}

#ifdef MINIMGIO_GENERATE

static void init_destination(jpeg_compress_struct* /*cinfo*/) {
}

static boolean empty_output_buffer(jpeg_compress_struct* cinfo) {
  minimgio_destination_mgr* dst_mgr =
    reinterpret_cast<minimgio_destination_mgr*>(cinfo->dest);
  if (dst_mgr->stream->write(dst_mgr->buffer, sizeof(dst_mgr->buffer)
      ) !=sizeof(dst_mgr->buffer))
    ERREXIT(cinfo, JERR_FILE_WRITE);

  dst_mgr->pub.next_output_byte = dst_mgr->buffer;
  dst_mgr->pub.free_in_buffer = sizeof(dst_mgr->buffer);

  return TRUE;
}

static void term_destination(j_compress_ptr cinfo)
{
  minimgio_destination_mgr* dst_mgr =
    reinterpret_cast<minimgio_destination_mgr*>(cinfo->dest);
  size_t datacount = sizeof(dst_mgr->buffer) - dst_mgr->pub.free_in_buffer;

  /* Write any data remaining in the buffer */
  if (datacount > 0) {
    if (dst_mgr->stream->write(dst_mgr->buffer, datacount) != datacount)
      ERREXIT(cinfo, JERR_FILE_WRITE);
  }
}

namespace minimgio { namespace internal {
MinResult SaveJpeg(
    BinaryStream      &stream,
    const MinImg      &img,
    const ExtImgProps *p_props,
    int                page) {
  if (!img.p_zero_line || page != 0)
    return MR_CONTRACT_VIOLATION;
  if (img.scalar_type != TYP_UINT8)
    return MR_NOT_IMPLEMENTED;

  jpeg_compress_struct cinfo = {};
  jem jerr = {{0}};
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jee;
  jerr.pub.output_message = jom;

  // ToDo: make `error_exit` report the error to stream
  if (setjmp(jerr.buf))
    return MR_ENV_ERROR;

  jpeg_create_compress(&cinfo);
  scoped_sjobj guard(&cinfo);
  minimgio_destination_mgr dst_mgr;
  dst_mgr.pub.next_output_byte = dst_mgr.buffer;
  dst_mgr.pub.free_in_buffer = sizeof(dst_mgr.buffer);
  dst_mgr.pub.init_destination = init_destination;
  dst_mgr.pub.empty_output_buffer = empty_output_buffer;
  dst_mgr.pub.term_destination = term_destination;
  dst_mgr.stream = &stream;
  cinfo.dest = reinterpret_cast<jpeg_destination_mgr *>(&dst_mgr);


  cinfo.image_width = img.width;
  cinfo.image_height = img.height;
  cinfo.input_components = img.channels;
  switch (cinfo.input_components)
  {
  case 1:
    cinfo.in_color_space = JCS_GRAYSCALE;
    break;
  case 3:
    cinfo.in_color_space = JCS_RGB;
    break;
  default:
    cinfo.in_color_space = JCS_UNKNOWN;
  }
  jpeg_set_defaults(&cinfo);
  int quality = 90;
  if (p_props)
  {
    cinfo.density_unit = 1;
    cinfo.X_density = static_cast<UINT16>(p_props->xDPI + .5f);
    cinfo.Y_density = static_cast<UINT16>(p_props->yDPI + .5f);
    if (p_props->qty)
      quality = p_props->qty;
  }
  MR_PROPAGATE_ERROR(stream.initialize(MIS_WRITEONLY));
  if (stream.lseek(0) != 0)
    return MR_ENV_ERROR;
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  JSAMPROW ppBuf[1] = {};
  JSAMPROW line = img.p_zero_line;
  for (int y = 0; y < img.height; y++) {
    ppBuf[0] = static_cast<JSAMPROW>(line);
    jpeg_write_scanlines(&cinfo, ppBuf, 1);
    line += img.stride;
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  return MR_SUCCESS;
}
}} // namespace minimgio::internal
#endif // #ifdef MINIMGIO_GENERATE
