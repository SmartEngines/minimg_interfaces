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

#include <gtest/gtest.h>
#include <minimgapi/minimgapi.h>
#include <minimgapi/minimgapi-inl.h>
#include <minimgapi/imgguard.hpp>
#include "../src/vector/transpose-inl.h"

TEST(TransposeTest, Transpose16x16) {
  uint8_t pool0[16 * 17] = {0};
  uint8_t pool1[16 * 19] = {0};

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      pool0[y * 17 + x] = y * 16 + x;
    }
  }

  Transpose16x16(pool1, 19, pool0, 17);

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      ASSERT_EQ(pool1[y * 19 + x], x * 16 + y);
    }
  }
}

TEST(TransposeTest, Transpose8x8) {
  uint16_t pool0[8 * 9] = {0};
  uint16_t pool1[8 * 11] = {0};

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      pool0[y * 9 + x] = (y * 8 + x) * 0x0101 + 0x8040;
    }
  }

  Transpose8x8(pool1, 22, pool0, 18);

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      ASSERT_EQ(pool1[y * 11 + x], (x * 8 + y) * 0x0101 + 0x8040);
    }
  }
}

TEST(TransposeTest, Transpose4x4) {
  uint32_t pool0[4 * 5] = {0};
  uint32_t pool1[4 * 7] = {0};

  for (unsigned int y = 0; y < 4; y++) {
    for (unsigned int x = 0; x < 4; x++) {
      pool0[y * 5 + x] = (y * 4U + x) * 0x10101010U + 0x0C0D0E0FU;
    }
  }

  Transpose4x4(pool1, 28, pool0, 20);

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      ASSERT_EQ(pool1[y * 7 + x],
                (uint32_t)((x * 4 + y) * 0x10101010U + 0x0C0D0E0FU));
    }
  }
}

TEST(TransposeTest, Transpose32x32Bits) {
  uint8_t pool0[10][32 * 5] = {{0}};
  uint8_t pool1[10][32 * 7] = {{0}};

  for (int k = 0; k < 10; ++k) {
    for (int y = 0; y < 32; ++y) {
      for (int x = 0; x < 32; ++x) {
        if ((y * 32 + x) & 1 << k) {
          SET_IMAGE_LINE_BIT(pool0[k] + y * 5, x);
        }
      }
    }
    Transpose32x32Bits(pool1[k], 7, pool0[k], 5);
  }
  for (int k = 0; k < 10; ++k) {
    for (int y = 0; y < 32; ++y) {
      for (int x = 0; x < 32; ++x) {
        ASSERT_EQ(!(GET_IMAGE_LINE_BIT(pool1[k] + y * 7, x)),
                  !((x * 32 + y) & 1 << k));
      }
    }
  }
}

TEST(TransposeTest, Transpose8x8Bits) {
  uint8_t pool0[6][8 * 3] = {{0}};
  uint8_t pool1[6][8 * 5] = {{0}};

  for (int k = 0; k < 6; ++k) {
    for (int y = 0; y < 8; ++y) {
      for (int x = 0; x < 8; ++x) {
        pool0[k][y * 3] |= !!((y * 8 + x) & 1 << k) << (7 - x);
      }
    }
    Transpose8x8Bits(pool1[k], 5, pool0[k], 3);
  }
  for (int k = 0; k < 6; ++k) {
    for (int y = 0; y < 8; ++y) {
      for (int x = 0; x < 8; ++x) {
        ASSERT_EQ(!(pool1[k][y * 5] & 128 >> x),
                  !((x * 8 + y) & 1 << k));
      }
    }
  }
}

TEST(TransposeTest, Transpose8x8BitsInternal) {
  uint8_t pool0[6][8] = {{0}};
  uint8_t pool1[6][8] = {{0}};

  for (int k = 0; k < 6; ++k) {
    for (int y = 0; y < 8; ++y) {
      for (int x = 0; x < 8; ++x) {
        pool0[k][y] |= !!((y * 8 + x) & 1 << k) << (7 - x);
      }
    }
    Transpose8x8BitsInternal(reinterpret_cast<uint64_t *>(pool1[k]),
                             reinterpret_cast<uint64_t *>(pool0[k]));
  }

  for (uint8_t k = 0; k < 6; ++k) {
    for (uint8_t y = 0; y < 8; ++y) {
      for (uint8_t x = 0; x < 8; ++x) {
        ASSERT_EQ(!(pool1[k][y] & 128 >> x),
                  !((x * 8 + y) & 1 << k));
      }
    }
  }
}

int main(int argc, char **argv) {
  // This will force Visual Studio to link against minimgapi library.
  MinImg dummy = {};
  GetMinImageType(&dummy);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
