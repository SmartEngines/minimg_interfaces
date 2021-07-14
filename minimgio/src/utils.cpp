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

#include <cstring>
#include <cstdio>

#include <minbase/minresult.h>
#include <minimgapi/minimgapi.h>  // DO_HORIZONTAL
#include <algorithm>  // std::min

#include "utils.h"


MinResult ExtractMemoryLocation(const char *fileName, uint8_t **ptr, size_t *size)
{
  void *_ptr = NULL;
  unsigned long _size = 0;

  int ret = sscanf(fileName, "mem://%p.%lu.", &_ptr, &_size);
  if (ret != 2)
    return MR_CONTRACT_VIOLATION;

  *ptr = static_cast<uint8_t*>(_ptr);
  *size = _size;

  return MR_SUCCESS;
}

bool atob(
    const char *pValue){
  if (pValue == NULL)
    return false;

  if (strcmp(pValue, "true") == 0)
    return true;

  if (atoi(pValue) != 0)
    return true;

  return false;
}

int OrientImageSpecial(  // ToDo: use new minimgapi functions
    const MinImg &dst_image,
    const MinImg &src_image,
    uint16_t      orientation) {
  switch (orientation) {
  case 2:
    return FlipMinImage(&dst_image, &src_image, DO_HORIZONTAL);
  case 3:
    return FlipMinImage(&dst_image, &src_image, DO_BOTH);
  case 4:
    return FlipMinImage(&dst_image, &src_image, DO_VERTICAL);
  case 5:
    return TransposeMinImage(&dst_image, &src_image);
  case 6: {
    MinImg tmp_img = {};
    PROPAGATE_ERROR(FlipMinImageVertically(&tmp_img, &src_image));
    return TransposeMinImage(&dst_image, &tmp_img);
  }
  case 7:
    PROPAGATE_ERROR(FlipMinImage(&src_image, &src_image, DO_BOTH));
    return TransposeMinImage(&dst_image, &src_image);
  case 8:
    PROPAGATE_ERROR(FlipMinImage(&src_image, &src_image, DO_HORIZONTAL));
    return TransposeMinImage(&dst_image, &src_image);
  default:
    return MR_SUCCESS;
  }
}

MinResult CopyString(
    char *p_dst_str,
    size_t dst_size,
    const char *p_src_str) {
  if (!p_dst_str || !p_src_str || dst_size <= 1)
    return MR_CONTRACT_VIOLATION;
  const size_t source_len = strlen(p_src_str);
  const size_t copy_size = std::min(source_len, dst_size - 1);

  ::memcpy(p_dst_str, p_src_str, copy_size);
  p_dst_str[copy_size] = '\0';
  return MR_SUCCESS;
}
