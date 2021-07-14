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

#include <string>  // std::getline
#include <istream>  // std::istream
#include <algorithm>  // std::min

#include <minutils/smartptr.h>
#include <minimgio/minimgio.hpp>
#include "utils.h"


class MemoryInputStream: public std::istream
{
public:
  MemoryInputStream(minimgio::BinaryStream &stream):
    std::istream(&stram_buffer), stram_buffer(stream) { }

private:
  class MemoryBuffer: public std::basic_streambuf<char>
  {
  public:
    MemoryBuffer(minimgio::BinaryStream& stream) : the_buffer{}, stream(stream) {
      stream.lseek(0);
    }
    int underflow() {
      const size_t read_bytes = stream.read(the_buffer, sizeof(the_buffer));
      if (read_bytes == 0)
        return traits_type::eof();
      setg(the_buffer, the_buffer, the_buffer + read_bytes);
      return the_buffer[0];
    }
  private:
    char the_buffer[4096];
    minimgio::BinaryStream &stream;
  };

  MemoryBuffer stram_buffer;
};

static std::string trimLeft(const std::string &s)
{
  static const std::string whitespace = " \n\r\t";
  size_t startpos = s.find_first_not_of(whitespace);
  return (startpos == std::string::npos) ? "" : s.substr(startpos);
}

static std::string trimRight(const std::string &s)
{
  static const std::string whitespace = " \n\r\t";
  size_t endpos = s.find_last_not_of(whitespace);
  return (endpos == std::string::npos) ? "" : s.substr(0, endpos+1);
}

static std::string trim(const std::string &s)
{
  return trimRight(trimLeft(s));
}

static std::string absoluteImagePath(const std::string &lstPath, const std::string &imageName)
{
  std::string::size_type pos = lstPath.find_last_of("/\\");
  const std::string &lstDirPath = (pos == std::string::npos) ? "." : lstPath.substr(0, pos);
  return lstDirPath + "/" + imageName;
}

MinResult GetLstPageName(
    char *pPageName,
    int pageNameSize,
    minimgio::BinaryStream &stream,
    int page) {
  if (!pPageName || pageNameSize <= 0)
    return MR_CONTRACT_VIOLATION;

  int pageIndex = -1;
  MR_PROPAGATE_ERROR(stream.initialize(MIS_READONLY));
  MemoryInputStream fileStream(stream);

  std::string imageName;
  while (std::getline(fileStream, imageName))
  {
    imageName = trim(imageName);
    if (imageName.length() == 0 || imageName[0] == ';')
      continue;
    if (imageName == "#stop" || imageName == "#end")
      break;

    if (++pageIndex == page)
      return CopyString(pPageName, pageNameSize, imageName.c_str());
  }

  return MR_CONTRACT_VIOLATION;
}

namespace minimgio { namespace internal {
MinResult GetNumPagesLst(
    int &num_pages,
    BinaryStream &stream) {
  MemoryInputStream fileStream(stream);

  int pageCount = 0;
  std::string imageName;
  while (std::getline(fileStream, imageName))
  {
    imageName = trim(imageName);
    if (imageName.length() == 0 || imageName[0] == ';')
      continue;
    if (imageName == "#stop" || imageName == "#end")
      break;
    pageCount++;
  }
  num_pages = pageCount;
  return MR_SUCCESS;
}
MinResult GetPropsLst(
    MinImg       &img,
    ExtImgProps *p_props,
    BinaryStream &stream,
    int          page) {
  char szPageName[4096] = {};
  MR_PROPAGATE_ERROR(GetLstPageName(szPageName, sizeof(szPageName), stream, page));
  const char * source = stream.get_source();
  const std::string absolute_path = absoluteImagePath(
    std::string(source ? source : "./"), szPageName);
  BinaryFileStream inner_stream(absolute_path.c_str());
  MinResult ret = ::minimgio::GetFileProps(img, inner_stream, p_props, 0);
  if (ret != MR_SUCCESS)
    stream.error(inner_stream.get_error());
  return ret;
}
MinResult LoadLst(
    const MinImg &img,
    BinaryStream  &stream,
    int           page) {
  char szPageName[4096] = {};
  MR_PROPAGATE_ERROR(GetLstPageName(szPageName, sizeof(szPageName), stream, page));
  const char * source = stream.get_source();
  const std::string absolute_path = absoluteImagePath(
    std::string(source ? source : "./"), szPageName);
  BinaryFileStream inner_stream(absolute_path.c_str());
  return ::minimgio::Load(img, inner_stream, 0);
}
} // namespace internal
} // namespace minimgio
