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

static void quantile_diff(uint8_t *the_diff, const MinImg &image_a, const MinImg &image_b)
{
  ASSERT_EQ(NO_ERRORS, CompareMinImagePrototypes(&image_a, &image_b));
  ASSERT_EQ(TYP_UINT8, GetMinImageType(&image_a));
  const int len = image_a.width * image_a.channels;
  *the_diff = 0;
  const uint8_t *p_line_a = image_a.p_zero_line;
  const uint8_t *p_line_b = image_b.p_zero_line;
  int hist[256] = { 0 };
  for (int y = 0; y < image_a.height; ++y)
  {
    for (int x = 0; x < len; ++x)
    {
      const uint8_t diff = p_line_a[x] > p_line_b[x] ? p_line_a[x] - p_line_b[x] : p_line_b[x] - p_line_a[x];
      ++hist[diff];
    }
    p_line_a += image_a.stride;
    p_line_b += image_b.stride;
  }
  const int threshold = image_a.height * len * 9 / 10;  // 90%
  int cnt = 0;
  int i = 0;
  for (; i < 256 && cnt < threshold; ++i) {
    cnt += hist[i];
  }
  *the_diff = i;
}
