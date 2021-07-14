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
#ifndef MINIMGIO_CONTRIB_H_INCLUDED
#define MINIMGIO_CONTRIB_H_INCLUDED

#include <cstdio>
#include <vector>

#include <minbase/minimg.h>
#include <minimgio/define.h>

namespace se { namespace image_io {

enum ImageFormat {
  FORMAT_JPEG,
  FORMAT_TIFF,
  FORMAT_PNG,
  FORMAT_WEBP,
  FORMAT_UNKNOWN
};

struct MINIMGIO_API OutputStreamInterface {
  virtual ~OutputStreamInterface() { }
  virtual int WriteBytes(const uint8_t *bytes, int count) = 0;
};

#ifndef STRICT_DATA_CONTAINMENT
class MINIMGIO_API FileOutputStream : public OutputStreamInterface {
 public:
  FileOutputStream();
  ~FileOutputStream();

  virtual int WriteBytes(const uint8_t *bytes, int count);

  int Open(const char *file_name);
  int Close();
 private:
  FILE *file_;
};
#endif // #ifndef STRICT_DATA_CONTAINMENT

class MINIMGIO_API FixedBufferOutputStream : public OutputStreamInterface {
 public:
  FixedBufferOutputStream();
  ~FixedBufferOutputStream();

  virtual int WriteBytes(const uint8_t *bytes, int count);

  int Bind(uint8_t *buffer, int length);
  int WrittenBytes() const;
 private:
  uint8_t *buffer_;
  int length_;
  int written_;
};

class MINIMGIO_API ExtensibleBufferOutputStream : public OutputStreamInterface {
 public:
  ExtensibleBufferOutputStream();
  ~ExtensibleBufferOutputStream();

  virtual int WriteBytes(const uint8_t *bytes, int count);

  const uint8_t *GetBuffer() const;
  int WrittenBytes() const;
 private:
  std::vector<uint8_t> buffer_;
};

struct MINIMGIO_API InputStreamInterface {
  virtual ~InputStreamInterface() { }
  virtual int ReadBytes(uint8_t *bytes, int count) = 0;
};



MINIMGIO_API int EncodeImage(const MinImg            &image,
                             ImageFormat             format,
                             OutputStreamInterface   &output);

} // image_io

} // namespace se

#endif // #ifndef MINIMGIO_CONTRIB_H_INCLUDED
