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

#include <algorithm>  // std::min
#include <sys/stat.h>  // fstat
#include <fcntl.h>  // open,  O_* flags

#include <cstring>  // strerror
#include <minbase/minresult.h>
#include <minimgio/minimgio.hpp>
#include <minutils/smartptr.h>

#include <vector>

#ifndef _WIN32
#  include <unistd.h>  // close, read, write
#  include <cerrno>  // errno
#  if defined(_LARGEFILE64_SOURCE) && _LFS64_LARGEFILE-0
#    define MINIMGIO_LSEEK ::lseek64
#  else
#    define MINIMGIO_LSEEK ::lseek
#  endif
#else
#  include <Windows.h>
#  include <io.h>  // _get_osfhandle
#endif


namespace minimgio {

static MinResult print_error(
    BinaryFileDescriptorStream *stream,
    char const *reason=nullptr) {
  char bufer[4096];
#ifndef _WIN32
  snprintf(bufer, sizeof(bufer), "Error opening '%s' (%s)",
            stream->get_source(), reason ? reason : strerror(errno));
#else
  char const* print_reason = reason;
  LPSTR messageBuffer = nullptr;
  if (!reason) {
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, ::GetLastError(), MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    print_reason = messageBuffer;

  }
  snprintf(bufer, sizeof(bufer), "Error opening '%s' (%s)",
    stream->get_source(), print_reason);
  if (!reason)
    ::LocalFree(messageBuffer);
#endif
  stream->error(bufer);
  return MR_ENV_ERROR;
}

//
// BinaryStreamErrorHandling
//
BinaryStreamErrorHandling::BinaryStreamErrorHandling() {
  this->error_buffer = reinterpret_cast<void*>(new std::vector<char>(1));
}
BinaryStreamErrorHandling::~BinaryStreamErrorHandling() {
  delete reinterpret_cast<std::vector<uint8_t>*>(this->error_buffer);
}
void BinaryStreamErrorHandling::error(const char* message) {
  const size_t message_length = strlen(message);
  std::vector<uint8_t>* vec = reinterpret_cast<std::vector<uint8_t>*>(
    this->error_buffer);
  if (vec->size() != 1) {
    vec->back() = '\n';
    vec->push_back('\0');
  }
  vec->insert(vec->end() - 1, message, message + message_length);
}
const char* BinaryStreamErrorHandling::get_error() const {
  return reinterpret_cast<std::vector<char>*>(this->error_buffer)->data();
}

//
// BinaryDescriptorStream
//
BinaryFileDescriptorStream::BinaryFileDescriptorStream(int fd)
{
#ifdef _WIN32
  this->handle = (HANDLE)_get_osfhandle(fd);
#else
  this->fd = fd;
#endif
}
#ifdef _WIN32
BinaryFileDescriptorStream::BinaryFileDescriptorStream(void* handle): handle(handle) {}
#endif
std::size_t BinaryFileDescriptorStream::read(void* buffer, std::size_t size) {
#ifndef _WIN32
  return ::read(this->fd, buffer, size);
#else
  DWORD o;
  uint8_t* ma = (uint8_t*)buffer;
  uint64_t mb = size;
  std::size_t p = 0;
  while (mb > 0)
  {
    DWORD n = 0x80000000UL;
    if ((uint64_t)n > mb)
      n = (DWORD)mb;
    if (!ReadFile(this->handle, (LPVOID)ma, n, &o, NULL))
      return 0;
    ma += o;
    mb -= o;
    p += o;
    if (o != n)
      break;
  }
  return p;
#endif
}
std::size_t BinaryFileDescriptorStream::write(const void* buffer, std::size_t size) {
#ifdef MINIMGIO_WRITE
#  ifndef _WIN32
  return ::write(this->fd, buffer, size);
#  else
  DWORD o;
  uint8_t* ma = (uint8_t*)buffer;
  uint64_t mb = size;
  std::size_t p = 0;
  while (mb > 0)
  {
    DWORD n = 0x80000000UL;
    if ((uint64_t)n > mb)
      n = (DWORD)mb;
    if (!WriteFile(this->handle, (LPVOID)ma, n, &o, NULL))
      return(0);
    ma += o;
    mb -= o;
    p += o;
    if (o != n)
      break;
  }
  return p;
#  endif // _WIN32
#else
  this->error("writing disabled");
  return 0;
#endif
}
int64_t BinaryFileDescriptorStream::lseek(int64_t offset, int whence) {
#ifndef _WIN32
  return MINIMGIO_LSEEK(this->fd, offset, whence);
#else
  LARGE_INTEGER offli;
  DWORD dwMoveMethod;
  offli.QuadPart = offset;
  switch (whence)
  {
  case SEEK_SET:
    dwMoveMethod = FILE_BEGIN;
    break;
  case SEEK_CUR:
    dwMoveMethod = FILE_CURRENT;
    break;
  case SEEK_END:
    dwMoveMethod = FILE_END;
    break;
  default:
    dwMoveMethod = FILE_BEGIN;
    break;
  }
  offli.LowPart = SetFilePointer(this->handle, offli.LowPart, &offli.HighPart, dwMoveMethod);
  if ((offli.LowPart == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))
    offli.QuadPart = 0;
  return offli.QuadPart;
#endif
}

//
// BinaryFileStream
//
BinaryFileStream::BinaryFileStream(const char* filename):
#ifndef _WIN32
  BinaryFileDescriptorStream(-1)
#else
  BinaryFileDescriptorStream(INVALID_HANDLE_VALUE)
#endif
  , filename(filename)
{}

BinaryFileStream::~BinaryFileStream() {
#ifndef _WIN32
  close(this->fd);
#else
  CloseHandle(this->handle);
#endif
}

MinResult BinaryFileStream::initialize(MISInitFlag flag) {
#ifndef _WIN32
  if (this->fd == -1) {
    int mode = 0;
    if (flag == MIS_READONLY)
      mode = O_RDONLY;
    else if (flag == MIS_WRITEONLY)
      mode = O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC;
    else if (flag == MIS_READWRITE)
      mode = O_RDWR | O_CREAT | O_CLOEXEC;
    /* for cygwin and mingw */
    #ifdef O_BINARY
      mode |= O_BINARY;
    #endif

    this->fd = open(this->filename, mode, 0666);
    if (this->fd < 0)
      return print_error(this);

    struct stat path_stat;
    if (fstat(this->fd, &path_stat) != 0) {
      close(this->fd);
      this->fd = -1;
      return print_error(this);
    }
    if (S_ISDIR(path_stat.st_mode)) {
      close(this->fd);
      this->fd = -1;
      return print_error(this, "is a directory");
    }
  }
#else
  if (this->handle == INVALID_HANDLE_VALUE) {
    DWORD dwDesiredAccess = 0;
    DWORD dwCreationDisposition = 0;
    if (flag == MIS_READONLY) {
      dwDesiredAccess = GENERIC_READ;
      dwCreationDisposition = OPEN_EXISTING;
    }
    else if (flag == MIS_WRITEONLY) {
      dwDesiredAccess = GENERIC_WRITE;
      dwCreationDisposition = CREATE_ALWAYS;
    }
    else if (flag == MIS_READWRITE) {
      dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
      dwCreationDisposition = OPEN_ALWAYS;
    }
    const int nLen = static_cast<int>(strlen(filename) + 1);
    const int nWideLen = MultiByteToWideChar(CP_UTF8, 0, filename, nLen, NULL, 0);
    scoped_c_array<wchar_t> wchar_name((wchar_t*)malloc(sizeof(wchar_t) * nWideLen));
    if (!wchar_name)
      return MR_ENV_ERROR;
    if (MultiByteToWideChar(CP_UTF8, 0, this->filename, nLen, wchar_name, nWideLen) == 0)
      return print_error(this);

    this->handle = CreateFileW(wchar_name,
      dwDesiredAccess,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, dwCreationDisposition,
      FILE_ATTRIBUTE_NORMAL,
      NULL);
    if (this->handle == INVALID_HANDLE_VALUE)
      return print_error(this);
  }
#endif // !_WIN32
  return MinResult::MR_SUCCESS;
}

const char *BinaryFileStream::get_source() const {
  return this->filename;
}

//
// BinaryMemoryReadonlyStream
//

BinaryMemoryReadonlyStream::BinaryMemoryReadonlyStream(
    const uint8_t* buffer,
    std::size_t size):
  buffer(buffer), size(size), pos(0) {}

MinResult BinaryMemoryReadonlyStream::initialize(
    MISInitFlag flag) {
  if (flag == MIS_READONLY)
    return MinResult::MR_SUCCESS;
  else
    return MinResult::MR_CONTRACT_VIOLATION;
}

std::size_t BinaryMemoryReadonlyStream::read(
    void        *out_buffer,
    std::size_t  read_size) {
  const std::size_t n = std::min(
    read_size,
    static_cast<std::size_t>(this->size - this->pos)
  );
  ::memcpy(out_buffer, this->buffer + this->pos, n);
  this->pos += n;
  return n;
}
std::size_t BinaryMemoryReadonlyStream::write(
    const void* /*buffer*/,
    std::size_t /*size*/) {
  return 0;
}
int64_t BinaryMemoryReadonlyStream::lseek(
    int64_t offset,
    int     whence) {
  switch (whence) {
    case SEEK_SET: {
      this->pos = offset;
      break;
    }
    case SEEK_CUR: {
      this->pos += offset;
      break;
    }
    case SEEK_END: {
      this->pos = this->size + offset;
      break;
    }
  }
  return this->pos;

}

}  // namespace minimgio
