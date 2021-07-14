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
#ifndef MINIMGIO_SRC_V4L2UTILS_H_INCLUDED
#define MINIMGIO_SRC_V4L2UTILS_H_INCLUDED

#include <string>
#include <map>

#include <minbase/minresult.h>

class ScopedV4L2File {
public:
  ScopedV4L2File(const std::string& file_path);
  ~ScopedV4L2File();

  bool IsFileProper() const;
  int Open();
  void Close();

  operator const int&() const;

private:
  ScopedV4L2File(const ScopedV4L2File&);
  ScopedV4L2File& operator =(const ScopedV4L2File&);

  const std::string file_path_;
  bool              file_is_proper_;
  int               fd_;
};

int InsistentIoctl(int dev_file, int request, const void* p_arg);

MinResult GetStreamingCaptureDeviceNameAddrMap(
    std::map<std::string, std::string>& name_addr_map);

#endif // #ifndef MINIMGIO_SRC_V4L2UTILS_H_INCLUDED
