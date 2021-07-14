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

#include <minbase/mintyp.h>


MINBASE_API MinFmt MinFormatOfMinType(MinTyp typ) {
  static const MinFmt formats[MINTYP_COUNT] = {
    FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT, FMT_UINT,
    FMT_INT,  FMT_INT,  FMT_INT,  FMT_INT,
    FMT_REAL, FMT_REAL, FMT_REAL
  };
  return typ < 0 || MINTYP_COUNT <= typ ? FMT_INVALID : formats[typ];
}


MINBASE_API int32_t LogBitSizeOfMinType(MinTyp typ) {
  static const int32_t sizes_logbits[MINTYP_COUNT] = {
    0,  3,  4,  5,  6,
    3,  4,  5,  6,
    4,  5,  6
  };
  return typ < 0 || MINTYP_COUNT == typ ? -1 : sizes_logbits[typ];
}


MINBASE_API int32_t ByteSizeOfMinType(MinTyp typ) {
  static const int32_t sizes_bytes[MINTYP_COUNT] = {
    -1,
    sizeof(uint8_t),  sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t),
    sizeof(int8_t),   sizeof(int16_t),  sizeof(int32_t),  sizeof(int64_t),
    sizeof(real16_t), sizeof(real32_t), sizeof(real64_t)
  };
  return typ < 0 || MINTYP_COUNT == typ ? -1 : sizes_bytes[typ];
}


MINBASE_API MinTyp MinTypeByMinFormatAndLogSize(
    MinFmt  format,
    int32_t size_logbits) {
  enum { MINLOGBITSIZE_COUNT = 7 };
  static const MinTyp typs[MINLOGBITSIZE_COUNT][MINFMT_COUNT] = {
    { TYP_UINT1,    TYP_INVALID,  TYP_INVALID },
    { TYP_INVALID,  TYP_INVALID,  TYP_INVALID },
    { TYP_INVALID,  TYP_INVALID,  TYP_INVALID },
    { TYP_UINT8,    TYP_INT8,     TYP_INVALID },
    { TYP_UINT16,   TYP_INT16,    TYP_REAL16  },
    { TYP_UINT32,   TYP_INT32,    TYP_REAL32  },
    { TYP_UINT64,   TYP_INT64,    TYP_REAL64  }
  };
  return size_logbits < 0 || MINLOGBITSIZE_COUNT <= size_logbits
      ? TYP_INVALID
      : format < 0 || MINFMT_COUNT <= format
        ? TYP_INVALID
        : typs[size_logbits][format];
}
