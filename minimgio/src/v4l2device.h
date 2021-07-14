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
#ifndef MINIMGIO_SRC_V4L2DEVICE_H_INCLUDED
#define MINIMGIO_SRC_V4L2DEVICE_H_INCLUDED

#include <string>
#include <functional>

#include <linux/videodev2.h>

#include "stream.h"
#include "v4l2utils.h"


#define MIN_V4L2_DEVICE_USE_BUFFER_PROXY

static constexpr size_t MAX_V4L2_BUFFERS = 1;

class V4L2Device : public Device {
public:
  static V4L2Device *Instance(
      const std::string &device_path,
      const std::string &frame_size = "",
      const std::string &frame_rate = "");

private:
  V4L2Device(const std::string &device_path);
  V4L2Device(const V4L2Device &);
  V4L2Device &operator =(const V4L2Device &);
  virtual ~V4L2Device();

public:
  MinResult Take();
  MinResult Release();
#ifndef STRICT_DATA_CONTAINMENT
  MinResult PushImage(const MinImg *);
#endif // #ifndef STRICT_DATA_CONTAINMENT
  MinResult QueryImage(const MinImg *p_image);
  MinResult QueryImagePropsEx(MinImg *p_image, ExtImgProps *p_props);
  MinResult GetProperty(const char *p_key, char *p_value, int size);
  MinResult SetProperty(const char *p_key, const char *p_value);

private:
  MinResult NegotiateFormat();
  MinResult GetCtrls();
  MinResult GetBuffersAndStartStreaming();
  MinResult StopStreamingAndFreeBuffers();
  MinResult SetFrameSize(uint32_t width, uint32_t height);
  MinResult SetFrameRate(real64_t fps);

private:
  typedef MinResult (*ImageShipperType)(const MinImg*, const uint8_t*, size_t);
  struct Buffer {
    void  *p;
    size_t size;
  };

private:
  ImageShipperType ImageShipper_;

private:
  std::map<std::string, v4l2_queryctrl> ctrl_queries;
  std::map<std::string, v4l2_control>   cur_ctrls;

private:
  int             counter_;
  ScopedV4L2File  dev_file_;
  v4l2_format     fmt_;
  v4l2_streamparm streamparams_;
  Buffer          buffers_[MAX_V4L2_BUFFERS];
  size_t          n_buffers_;
  size_t          cur_buf_index_;
#ifdef MIN_V4L2_DEVICE_USE_BUFFER_PROXY
  void           *p_buffer_proxy_;
#endif

private:
  v4l2_pix_format &pix_;
  v4l2_fract      &timeperframe_;
//  const char      *fmt_string_;
};

#endif // #ifndef MINIMGIO_SRC_V4L2DEVICE_H_INCLUDED
