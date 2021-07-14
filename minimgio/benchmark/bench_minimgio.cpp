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

#include <minimgio/minimgio.hpp>
#include <minimgapi/imgguard.hpp>
#include <benchmark/benchmark.h>
#include <minimgapi/test/minrandom.hpp>


struct VectorBinaryStream
    : public minimgio::BinaryStreamErrorHandling {
  VectorBinaryStream(std::vector<uint8_t> &vec) : vec(vec), pos(0U) {}
  MinResult initialize(MISInitFlag) override {
    return MR_SUCCESS;
  }
  std::size_t read(void* buffer, std::size_t size) override {
    const std::size_t new_pos = std::min(this->vec.size(), this->pos + size);
    const std::size_t copy_sz = new_pos - this->pos;
    if (copy_sz)  // "this->vec[this->pos]" will throw assertation on msvc when vec.size is 0
      ::memcpy(buffer, &this->vec[this->pos], copy_sz);
    this->pos = new_pos;
    return copy_sz;
  }
  std::size_t write(const void* buffer, std::size_t size) override {
    const std::size_t new_pos = this->pos + size;
    if (new_pos > this->vec.size())
      this->vec.resize(new_pos);
    std::memcpy(&this->vec[this->pos], buffer, size);
    this->pos = new_pos;
    return size;
  }
  int64_t lseek(int64_t offset, int whence) override {
    switch(whence) {
      case SEEK_SET: this->pos = offset; break;
      case SEEK_CUR: this->pos += offset; break;
      case SEEK_END: this->pos = this->vec.size() + offset; break;
      default: return -1;
    }
    this->pos = std::max(size_t(0), this->pos);
    if (this->pos > this->vec.size())
      this->vec.resize(this->pos);
    return static_cast<int64_t>(this->pos);
  };

protected:
  std::vector<uint8_t> &vec;
  std::size_t pos;
};

struct TestBinaryStream : public VectorBinaryStream
{
  TestBinaryStream() : VectorBinaryStream(inner_vec) {}
protected:
  std::vector<uint8_t> inner_vec;
};

#define FAIL_ON_ERROR(call) do {if ((call) != 0) \
  state.SkipWithError("min call failed");} while (0)


static void BM_Decoding(
    benchmark::State& state,
    ImgFileFormat const img_format,
    int const width,
    int const height) {
  DECLARE_GUARDED_MINIMG(src);
  DECLARE_GUARDED_MINIMG(dst);
  FAIL_ON_ERROR(NewMinImagePrototype(&src, width, height, 3, TYP_UINT8));
  ApplyLineFunctor(&src, Random255());
  TestBinaryStream stream;
  FAIL_ON_ERROR(minimgio::Save(stream, src, img_format));
  FAIL_ON_ERROR(CloneMinImagePrototype(&dst, &src));
  for (auto _ : state) {
    FAIL_ON_ERROR(minimgio::Load(dst, stream, 0));
  }
  state.SetBytesProcessed(
    int64_t(state.iterations()) * stream.lseek(0, SEEK_END));
  state.SetItemsProcessed(
    int64_t(state.iterations()) * width * height * src.channels);
}

static void BM_Encoding(
    benchmark::State& state,
    ImgFileFormat const img_format,
    int const width,
    int const height) {
  DECLARE_GUARDED_MINIMG(src);
  DECLARE_GUARDED_MINIMG(dst);
  FAIL_ON_ERROR(NewMinImagePrototype(&src, width, height, 3, TYP_UINT8));
  ApplyLineFunctor(&src, Random255());
  TestBinaryStream stream;
  for (auto _ : state) {
    stream.lseek(0, SEEK_SET);
    FAIL_ON_ERROR(minimgio::Save(stream, src, img_format));
  }
  state.SetBytesProcessed(
    int64_t(state.iterations()) * stream.lseek(0, SEEK_END));
  state.SetItemsProcessed(
    int64_t(state.iterations()) * width * height * src.channels);
}

#define BENCHMARK_FORMAT(Func, name, NAME) \
  /*BENCHMARK_CAPTURE(Func, name##_vga, IFF_##NAME, 640, 480);*/ \
  BENCHMARK_CAPTURE(Func, name##_svga, IFF_##NAME, 800, 600); \
  /*BENCHMARK_CAPTURE(Func, name##_hdtv, IFF_##NAME, 1920, 1080);*/ \
  /*BENCHMARK_CAPTURE(Func, name##_wquxga, IFF_##NAME, 3840, 2400);*/

BENCHMARK_FORMAT(BM_Decoding, jpeg, JPEG);
BENCHMARK_FORMAT(BM_Decoding, png, PNG);
BENCHMARK_FORMAT(BM_Decoding, tiff, TIFF);
BENCHMARK_FORMAT(BM_Encoding, jpeg, JPEG);
BENCHMARK_FORMAT(BM_Encoding, png, PNG);
BENCHMARK_FORMAT(BM_Encoding, tiff, TIFF);

BENCHMARK_MAIN();
