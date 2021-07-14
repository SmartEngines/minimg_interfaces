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

/**
 * @file   imgguard.hpp
 * @brief  Definition of utility classes.
 */

#pragma once
#ifndef MINIMGAPI_IMGGUARD_HPP_INCLUDED
#define MINIMGAPI_IMGGUARD_HPP_INCLUDED

#include <minimgapi/minimgapi.h>

/**
 * @brief   RAII class for automated MinImg free-ing.
 * @ingroup MinImgAPI_Utility
 */
class MinImgGuard {
public:
  /// Constructor. Sets the image ref.
  MinImgGuard(MinImg &image)
    : image(image)
  {}
  /// Destructor. Frees the image.
  virtual ~MinImgGuard() {
    FreeMinImage(&image);
  }
private:
  MinImg &image; ///< The reference to the image to be freed.
};

/**
 * @brief   Declares a new @MinImg called <name> and the @c MinImgGuard
 *          called <name>_MinImgGuard.
 * @ingroup MinImgAPI_Utility
 */
#define DECLARE_GUARDED_MINIMG(name) \
  MinImg name = {}; MinImgGuard name##_MinImgGuard(name)

#endif // #ifndef MINIMGAPI_IMGGUARD_HPP_INCLUDED
