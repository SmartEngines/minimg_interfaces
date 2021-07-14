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
#ifndef MINIMGAPI_MINIMGAPI_TYPES_H_INCLUDED
#define MINIMGAPI_MINIMGAPI_TYPES_H_INCLUDED


/**
 * @brief   Specifies allocation options.
 * @details The enum specifies whether the new object should be allocated. This
 *          is used in various create- and clone-functions.
 */
typedef enum CheckType {
  CT_PRECONDITION,
  CT_INFO,
  CT_POSTCONDITION
} CheckType;


/**
 * @brief   Specifies allocation options.
 * @details The enum specifies whether the new object should be allocated. This
 *          is used in various create- and clone-functions.
 */
typedef enum AllocationOption {
  AO_EMPTY,         ///< The object should stay empty (without allocation).
  AO_PREALLOCATED   ///< The object should be allocated.
} AllocationOption;


/**
 * @brief   Specifies border acceptable border conditions.
 * @details The enum specifies acceptable options for border condition. If a
 *          function needs pixels outside of an image, then they are
 *          reconstructed according to one the following modes (that is, fill
 *          the "image border").
 */
typedef enum BorderOption {
  BO_IGNORE = 0,  ///< Ignores the image size and allows out of memory reading.
  BO_REPEAT,      ///< The value of pixel out of the image is assumed to be
                  ///  equal to the nearest one in the image.
  BO_CYCLIC,      ///< Assumes that coordinate plane is periodical with an image
                  ///  as a period.
  BO_SYMMETRIC,   ///< Assumes that coordinate plane is periodical with
                  ///  an image as a half-period.
  BO_CONSTANT,    ///< Assumes that pixels out of image have fixed value.
  BO_VOID         ///< Assumes that pixels out of image do not exist.
} BorderOption;


/**
 * @brief   Specifies acceptable directions.
 * @details The enum specifies directions which can be used in image
 *          transformation, image filtration, calculation orientation and other
 *          functions.
 */
typedef enum DirectionOption {
  DO_VERTICAL,     ///< Vertical transformation.
  DO_HORIZONTAL,   ///< Horizontal transformation.
  DO_BOTH          ///< Transformation in both directions.
} DirectionOption;


/**
* @brief   Specifies the part of Hough space to calculate
* @details The enum specifies the part of Hough space to calculate. Each
* member specifies primarily direction of lines (horizontal or vertical)
* and the direction of angle changes.
* Direction of angle changes is from multiples of 90 to odd multiples of 45.
* The image considered to be written top-down and left-to-right.
* Angles are started from vertical line and go clockwise.
* Separate quarters and halves are written in orientation they should be in
* full Hough space.
*/
typedef enum AngleRangeOption {
  ARO_0_45,    ///< Vertical primarily direction and clockwise angle changes
  ARO_45_90,   ///< Horizontal primarily direction and counterclockwise angle changes
  ARO_90_135,  ///< Horizontal primarily direction and clockwise angle changes
  ARO_315_0,   ///< Vertical primarily direction and counterclockwise angle changes
  ARO_315_45,  ///< Vertical primarily direction
  ARO_45_135,  ///< Horizontal primarily direction
  ARO_315_135, ///< Full set of directions
  ARO_CTR_HOR, ///< 90 +/- atan(0.5), interval approximately from 63.5 to 116.5 degrees (~53 degrees)
               ///< It is used for calculating Hough transform for images skewed by atan(0.5).
  ARO_CTR_VER, ///< +/- atan(0.5), interval approximately from 333.5(-26.5) to 26.5 degrees (~53 degrees)
               ///< It is used for calculating Hough transform for images skewed by atan(0.5).
} AngleRangeOption;


#endif // #ifndef MINIMGAPI_MINIMGAPI_TYPES_H_INCLUDED
