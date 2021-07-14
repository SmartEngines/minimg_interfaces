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
 * @file   minimgprc.h
 * @brief  MinImgPrc library API
 */

#pragma once
#ifndef MINIMGPRC_H_INCLUDED
#define MINIMGPRC_H_INCLUDED

#include <minimgapi/minimgapi.h>
#include <minutils/mathoper.h>
#ifdef __cplusplus
  #include <limits> // std::numeric_limits<conn_comps_height_t>::max()
#endif

/**
 * @def     MINIMGPRC_API
 * @brief   Specifies storage-class information (only for MSC).
 */

#if defined _MSC_VER && MINIMGPRC_EXPORTS
# define MINIMGPRC_API __declspec(dllexport)
#elif defined(__clang__) || defined(__GNUC__)
# define MINIMGPRC_API  \
  __attribute__ ((visibility ("default")))  \
  __attribute__ ((noinline))
#else
# define MINIMGPRC_API
#endif


#ifdef MINIMGPRC_STOPWATCH_OLD_INTERFACE
# define MINIMGPRC_DECLARE_STOPWATCH_CTL_OLD_INTERFACE(sw) \
    DECLARE_MINSTOPWATCH_CTL(sw)
#else
# define MINIMGPRC_DECLARE_STOPWATCH_CTL_OLD_INTERFACE(sw)
#endif


#ifdef __cplusplus
  extern "C" {
#endif

/**
 * @mainpage Overview
 *
 * <b>MinImgPrc</b> is a platform-independent toolkit for developers that will
 * allow you to process bitmap images.
 *
 * One of the MinImgPrc's goals is to provide computational efficiency image
 * processing functions therefore a set of special algorithms is implemented in
 * the library: the van Herk/Gil-Werman (HGW) algorithm for fast computing the
 * dilatation and erosion (the basic mathematical morphology operations), the
 * Deriche-van Vliet algorithm for computing recursive Gaussian and Gaussian
 * derivative filters, the algorithm of fast calculation of the 2-dimensional
 * Hough Transform and others.
 *
 * For the internal representation of the image is used cross-platform
 * open-source container - MinImg. The advantages of this container are the using
 * a minimal number of fields needed to represent the bitmap image and the easy
 * way to cast it to other standard and popular containers (for instance,
 * Windows DIB, GDI+ BitmapData, Intel/OpenCV IplImage).
 *
 * The library is written in C++ and can be compiled under Linux (GCC) and
 * Windows (MSVC 8 and later). Though the library has been written in C++, it
 * has C interface, so it can be embedded in different systems.
 */

/**
 * @page MinImgPrcLicese_Documentation Documentation License Agreement
 * This documentation is released under the following license.
 * @include doc/license.txt
 */

/**
 * @defgroup MinImgPrc_Convert Image Conversion
 * @brief    Sometimes it is necessary to convert an image from one type to
 *           another. This module includes function @c ConvertImage() that can
 *           do it. The conversion is done with rounding and saturation, that is
 *           if the result of conversion can not be represented exactly by a
 *           value of the destination array element type, it is set to the
 *           nearest representable value of the real axis.
 */

/**
 * @defgroup MinImgPrc_Spatial Spatial Image Transformation
 * @brief    A spatial transformation (also known as a geometric operation)
 *           modifies the spatial relationship between pixels in an image,
 *           mapping pixel locations in an input image to new locations in an
 *           output image. The module includes functions that perform certain
 *           specialized spatial transformations, such as resizing and rotating
 *           an image.
 */

/**
 * @defgroup MinImgPrc_Enhancement Image Analysis & Enhancement
 * @brief    This module includes the function for image enhancement such as
 *           gamma correction, contrast enhancement and others.
 */

/**
 * @defgroup MinImgPrc_Filter Image Filtering
 * @brief    The module includes a set of 2D filters: averaging, Gaussian
 *           smoothing and others. All filters are implemented for different
 *           border conditions (see @c BorderOptions).
 */

/**
 * @defgroup MinImgPrc_Binarization Image Binarization
 * @brief    Image binarization (or thresholding) is the simplest method of
 *           image segmentation. From a grayscale image, thresholding can be
 *           used to create binary images. There are three binarization methods
 *           are implemented in the MinImgPrc library: simple threshold
 *           binarization, global binarization using Otsu's method and local
 *           adaptive binarization using Niblack's method.
 */

/**
 * @defgroup MinImgPrc_Merge Binary Image Operations
 * @brief    This module includes the function MergeImage() that allows a set of
 *           binary operation under images.
 */

/**
 * @defgroup MinImgPrc_Morphology Mathematical Morphology
 * @brief    Mathematical morphology is a theory and technique for the analysis
 *           and processing of geometrical structures, based on set theory,
 *           geometry and topology. The library contains implementation for basic
 *           morphological operations (erosion, dilation, opening, closing) with
 *           rectangular structuring element. To increase the performance the
 *           van Herk/Gil-Werman (HGW) algorithm is used.
 */

/**
 * @defgroup MinImgPrc_Tensor Structure tensor
 * @brief    In mathematics, the structure tensor, also referred to as the
 *           second-moment matrix, is a matrix derived from the gradient of a
 *           function. It summarizes the predominant directions of the gradient
 *           in a specified neighborhood of a point, and the degree to which
 *           those directions are coherent.
 */

/**
 * @defgroup MinImgPrc_Hough Fast Hough Transform
 * @brief    The Hough transform is a feature extraction technique used in image
 *           analysis, computer vision, and digital image processing. The purpose
 *           of the technique is to find imperfect instances of objects within a
 *           certain class of shapes by a voting procedure. The library contains
 *           a fast implementation of 2D Hough transform.
 */

/**
 * @defgroup MinImgPrc_Integral Integral Image
 * @brief    An integral image (also known as a summed area table)  is an
 *           algorithm for quickly and efficiently generating the sum of values
 *           in a rectangular subset of a grid.
 */

/**
 * @defgroup MinImgPrc_Blend Image Blending
 * @brief    This module includes two functions for blending two images.
 */

/**
 * @defgroup MinImgPrc_Edge Edge Detection
 * @brief    Edge detection is a fundamental tool in image processing and
 *           computer vision. This module includes the implementation of Canny
 *           algorithm for edge detection.
 */

/**
 * @defgroup MinImgPrc_ConnComps Connected Components Search
 */

/**
 * @defgroup MinImgPrc_ScalePyramid Scale pyramid management
 */

#ifndef MINGEO_MINGEO_H_INCLUDED

typedef struct MinPoint {
    int32_t x;
    int32_t y;
} MinPoint;

typedef struct MinPoint2d {
    real64_t x;
    real64_t y;
} MinPoint2d;

/**
* @brief   Line segment represented as two ends points.
* @details The struct @c MinLineSegment represents a line segment as a tuple of
* two MinPoint's.
* @ingroup MinUtils_Geometry
*/
typedef struct
{
    MinPoint u;     ///< First point.
    MinPoint v;     ///< Second point.
} MinLineSegment;

#endif

/**
 * @brief   Specifies interpolation methods.
 * @details The enum specifies acceptable interpolation methods. This can be used
 *          in the the case when pixel values at fractional coordinates needs to
 *          be retrieved (for example, affine or projective transformation functions).
 */
typedef enum QualityOption {
  QO_PIXEL,       ///< Nearest-neighbor interpolation.
  QO_SUBPIXEL     ///< Bilinear interpolation.
} QualityOption;

/**
 * @brief   Specifies calculation precision required from methods.
 * @details The enum specifies acceptable precision. This can be used
 *          to choose between faster but less precise and slower but more precise algorithms
 *          implementing specified image transformation.
 */
typedef enum PrecisionControl {
  PC_PRECISE,     ///< Calculation with maximum precision is required
  PC_MEDIUM,      ///< Calculation with medium precision is allowed
  PC_LOW          ///< Calculation with low precision is allowed
} PrecisionControl;

/**
* @brief   Specifies to do or not to do skewing of Hough transform image
* @details The enum specifies to do or not to do skewing of Hough transform image
* so it would be no cycling in Hough transform image through borders of image.
*/
typedef enum HoughDeskewOption {
  HDO_RAW,           ///< Use raw cyclic image
  HDO_DESKEW,        ///< Prepare deskewed image
} HoughDeskewOption;

/**
 * @brief   Specifies predicates supported by CombineImage function.
 * @details The enum contains predicates that specify the rule of blending images
 * by CombineImage.
 */
typedef enum PredicateOption {
  PO_EQUAL                = 0x01,
  PO_LESS                 = 0x02,
  PO_GREATER              = 0x04,

  PO_ABS_MASK             = 0x08,
  PO_NOT_MASK             = PO_EQUAL | PO_LESS | PO_GREATER,
  PO_PERMUTE_MASK         = PO_LESS | PO_GREATER,

  PO_FALSE                = 0x00,
  PO_TRUE                 = PO_NOT_MASK ^ PO_FALSE,
  PO_LESS_OR_EQUAL        = PO_LESS | PO_EQUAL,
  PO_NOT_EQUAL            = PO_NOT_MASK ^ PO_EQUAL,
  PO_GREATER_OR_EQUAL     = PO_GREATER | PO_EQUAL,

  PO_ABS_EQUAL            = PO_ABS_MASK | PO_EQUAL,
  PO_ABS_LESS             = PO_ABS_MASK | PO_LESS,
  PO_ABS_GREATER          = PO_ABS_MASK | PO_GREATER,
  PO_ABS_LESS_OR_EQUAL    = PO_ABS_MASK | PO_LESS_OR_EQUAL,
  PO_ABS_NOT_EQUAL        = PO_ABS_MASK | PO_NOT_EQUAL,
  PO_ABS_GREATER_OR_EQUAL = PO_ABS_MASK | PO_GREATER_OR_EQUAL
} PredicateOption;

/**
 * @brief Specifies the masks to calculate image gradient.
 * @detail The enum contains masks supported by GetImageGradient that specify gradient
 * calculation pattern.
 */
typedef enum GradientMaskOption {
  GMO_STRAIGHT_CROSS,
  GMO_STRAIGHT_CROSS_DRAFT, // without normalization by 2
  GMO_ROBERTS_CROSS,
  GMO_ROBERTS_CROSS_DRAFT, // without normalization by sqrt(2)
  GMO_TRIANGLE_TOP_LEFT,
  GMO_TRIANGLE_TOP_RIGHT,
  GMO_TRIANGLE_BOTTOM_LEFT,
  GMO_TRIANGLE_BOTTOM_RIGHT,
  GMO_SCHARR_32,
  GMO_SCHARR_32_DRAFT // without normalization by 32
} GradientMaskOption;

/**
 * @brief   Converts an image to another format.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
            @c #TYP_REAL16 can only be converted to @c #TYP_REAL32 and backward.
 * @remarks Both source and destination images must have the same size and
 *          the same number of channels.
 * @ingroup MinImgPrc_Convert
 *
 * The function converts the source pixel values to the target datatype
 * (specified by the destination image @c p_dst_image) and copies them to
 * @c p_dst_image image. The conversion is done with rounding and saturation,
 * that is if the result of conversion can not be represented exactly by a value
 * of the destination array element type, it is set to the nearest representable
 * value of the real axis. If the destination image has the same format (that is
 * the same type and the same channel depth) as the source image, then this
 * function just copies all elements from @c p_src_image to @c p_dst_image
 * without any conversions. All the channels of multi-channel images are
 * processed independently.
 */
MINIMGPRC_API int ConvertImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image);

/**
 * @brief   Converts an image to another format.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size.
 * @ingroup MinImgPrc_Convert
 *
 * The function converts the source pixel values to the target datatype
 * (specified by the destination image @c p_dst_image) and copies them to
 * @c p_dst_image image with channel number change. It supports convert from
 * 1-channel image to multi-channel image (value of pixel from @c p_src_image is
 * copied to all channels of @c p_dst_image pixel), convert from multi-channel
 * image to 1-channel image (value of @c p_dst_image pixel is the average of
 * @c p_src_image channels).
 */
MINIMGPRC_API int ConvertImageDepth(
    const MinImg *p_dst_image,
    const MinImg *p_src_image);

/**
 * @brief   Scales an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   quality     Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for nearest-neighbor interpolation.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @ingroup MinImgPrc_Spatial
 *
 * The function resizes the source image @c p_src_image down to or up to so that
 * it fits exactly into the destination image @c p_dst_image. Additionally it
 * allows to downscale the image along the one axis and upscale along another
 * axis. The @c quality argument specifies the interpolation method: @c QO_PIXEL
 * for nearest-neighbor interpolation and @c QO_SUBPIXEL for bilinear
 * interpolation.
 * All the channels of multi-channel images are processed independently.
 */
MINIMGPRC_API int ScaleImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    QualityOption quality);

/**
 * @brief   Rotates an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   angle       The rotation angle in radians.
 * @param   p_bg_pixel  The value used to fill outliers.
 * @param   quality     Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for nearest-neighbor interpolation.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @ingroup MinImgPrc_Spatial
 *
 * The function rotates the source image in the counter-clockwise direction.
 * The center of the image maps to itself. Pixels in the destination image that
 * corresponds to the "outliers" in the source image are filled by
 * @c p_bg_pixel. The @c quality argument specifies the interpolation method:
 * @c QO_PIXEL for nearest-neighbor interpolation and @c QO_SUBPIXEL for
 * bilinear interpolation. All the channels of multi-channel images are
 * processed independently.
 */
MINIMGPRC_API int RotateImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        angle,
    const void   *p_bg_pixel,
    QualityOption quality,
    BorderOption  bo IS_BY_DEFAULT(BO_CONSTANT));

/**
 * @brief   Skews an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   shift       The tangent of the skew angle.
 * @param   is_vertical Specifies whether the vertical skew is used.
 * @param   p_bg_pixel  The value used to fill outliers.
 * @param   quality     Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for nearest-neighbor interpolation.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @ingroup MinImgPrc_Spatial
 *
 * The function skews the source image both vertical and horizontal directions.
 * The center of the image maps to itself. Pixels in the destination image that
 * corresponds to the "outliers" in the source image are filled by
 * @c p_bg_pixel. The @c quality argument specifies the interpolation method:
 * @c QO_PIXEL for nearest-neighbor interpolation and @c QO_SUBPIXEL for
 * bilinear interpolation. All the channels of multi-channel images are
 * processed independently.
 */
MINIMGPRC_API int SkewImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        shift,
    bool          is_vertical,
    const void   *p_bg_pixel,
    QualityOption quality,
    BorderOption  bo IS_BY_DEFAULT(BO_CONSTANT));

/**
 * @brief   Applies an affine transformation to an image.
 * @param   p_dst_image    The destination image.
 * @param   p_src_image    The source image.
 * @param   inverse_matrix The 2x3 inverse transformation matrix.
 * @param   p_bg_pixel     The value used to fill outliers.
 * @param   quality        Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for nearest-neighbor interpolation.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @ingroup MinImgPrc_Spatial
 *
 * An affine transformation is specified by the following matrix:
 * @f[
 *   inverse{\_}matrix = \begin{pmatrix}
 *     a_1 & b_1 & c_1 \cr a_2 & b_2 & c_2
 *   \end{pmatrix}
 * @f]
 * The function transforms the source image by the following way:
 * @f[ p{\_}dst{\_}image(x', y') = p{\_}src{\_}image(x, y) @f]
 * @f[ x = a_1 \cdot x' + b_1 \cdot y' + c_1 @f]
 * @f[ y = a_2 \cdot x' + b_2 \cdot y' + c_2 @f]
 * All the channels of multi-channel images are processed independently.
 */
MINIMGPRC_API int AffineTransformImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    const double  inverse_matrix[2][3],
    const void   *p_bg_pixel,
    QualityOption quality,
    BorderOption  bo IS_BY_DEFAULT(BO_CONSTANT));

/**
 * @brief   Applies a projective transformation to an image.
 * @param   p_dst_image    The destination image.
 * @param   p_src_image    The source image.
 * @param   inverse_matrix The 3x3 inverse transformation matrix.
 * @param   p_bg_pixel     The value used to fill outliers and area close to
 *                         the line at infinity.
 * @param   quality        Interpolation method.
 * @param   pc             The required calculation precision.
 * @param   bo             The required border option. Supports BO_SYMMETRIC,
 *                         BO_CYCLIC, BO_REPEAT, BO_CONSTANT and BO_VOID
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @remarks If p_bg_pixel is set to NULL value when using border option
 *          BO_SYMMETRIC or BO_CYCLIC,the value of pixels close
 *          to the line at infinity on destination image won't be changed.
 * @ingroup MinImgPrc_Spatial
 *
 * A projective transformation is specified by the following matrix:
 * @f[
 *   inverse{\_}matrix = \begin{pmatrix}
 *     a_1 & b_1 & c_1 \cr a_2 & b_2 & c_2 \cr a_3 & b_3 & c_3
 *   \end{pmatrix}
 * @f]
 * The function transforms the source image by the following way:
 * @f[ p{\_}dst{\_}image(x', y') = p{\_}src{\_}image(x, y) @f]
 * @f[ x = \frac{a_1 \cdot x' + b_1 \cdot y' + c_1}
 *              {a_3 \cdot x + b_3 \cdot y + c_3} @f]
 * @f[ y = \frac{a_2 \cdot x' + b_2 \cdot y' + c_2}
 *              {a_3 \cdot x + b_3 \cdot y + c_3} @f]
 * All the channels of multi-channel images are processed independently.
 */
MINIMGPRC_API int ProjectiveTransformImage(
    const MinImg    *p_dst_image,
    const MinImg    *p_src_image,
    const double     inverse_matrix[3][3],
    const void      *p_bg_pixel,
    QualityOption    quality,
    PrecisionControl pc IS_BY_DEFAULT(PC_PRECISE),
    BorderOption bo IS_BY_DEFAULT(BO_VOID));

/**
 * @brief   Returns parameters for decomposed projective transformation of an
 * image.
 * @param   task_num           The number of tasks in decomposed problem.
 * @param   p_dst_images       The pointer to array of destination images.
 * @param   decomposedMatrices The pointer to array of inverse matrices.
 * @param   inverseMatrices    The 3x3 inverse transformation matrix.
 * @param   p_src_image        The image to decompose.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The arrays for destination images and decomposed matrices must be
 * already allocated.
 * @ingroup MinImgPrc_Spatial
 * The function performs decomposition of projective transformation of an image.
 * It returns array of task_num destination images and array of corresponding
 * matrices of inverse projective transform, that can be processed independently.
 */
MINIMGPRC_API int GetDecomposedProjectiveTransformImage(
    int           task_num,
    MinImg       *p_dst_images,
    void         *decomposedMatrices,
    const double  inverseMatrix[3][3],
    const MinImg *p_src_image);

/**
 * @brief   Returns parameters for decomposed Gauss filtering of an
 * image.
 * @param   task_num           The number of tasks in decomposed problem.
 * @param   p_dec_dst          The pointer to array of decomposed destination images.
 * @param   p_dec_src          The pointer to array of decomposed source images.
 * @param   p_dst_image        The pointer to destination image.
 * @param   p_src_image        The pointer to source image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The arrays for destination images must be
 * already allocated.
 * @ingroup MinImgPrc_Spatial
 */
MINIMGPRC_API int GetDecomposedGaussFilterImage(
    int           task_num,
    MinImg       *p_dec_dst,
    MinImg       *p_dec_src ,
    const MinImg *p_dst_image,
    const MinImg *p_src_image);

/**
 * @brief   Corrects radial distortion of an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   alpha       The first radial distortion coefficient.
 * @param   beta        The second radial distortion coefficient.
 * @param   gamma       The third radial distortion coefficient.
 * @param   f           f = d * focus_ratio, where d is @c p_src_image diameter.
 * @param   x0s         X-coordinate of @c p_src_image center.
 * @param   y0s         Y-coordinate of @c p_src_image center.
 * @param   x0d         X-coordinate of @c p_dst_image center.
 * @param   y0d         Y-coordinate of @c p_dst_image center.
 * @param   scale       Additional affine scale.
 * @param   p_bg_pixel  The value used to fill outliers.
 * @param   quality     Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @ingroup MinImgPrc_Spatial
 *
 * The function transforms the @c p_src_image image as follows:
 * @f[ p{\_}dst{\_}image(x', y') = p{\_}src{\_}image(x, y) @f]
 * @f[ x = x'(alpha \cdot r^2 + beta \cdot r^4 + gamma \cdot r^6) @f]
 * @f[ y = y'(alpha \cdot r^2 + beta \cdot r^4 + gamma \cdot r^6) @f]
 * where @f$ r = \sqrt{(x')^2 + (y')^2} / f @f$ - the distance to the point
 * @f$ (x', y') @f$. Here we assume that coordinates start at @f$ (x0s, y0s) @f$
 * for @c p_src_image and at @f$ (x0d, y0d) @f$ for @c p_dst_image. Pixels in
 * the destination image that corresponds to the "outliers" in the source image
 * are filled by @c p_bg_pixel.
 */
MINIMGPRC_API int RadialCorrectAndScaleImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        alpha,
    double        beta,
    double        gamma,
    double        f,
    double        x0s,
    double        y0s,
    double        x0d,
    double        y0d,
    double        scale,
    const void   *p_bg_pixel,
    QualityOption quality);

/**
 * @brief   Performs a transformation of every image pixel.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   p_trn       The transformation engine.
 * @param   p_bg_pixel  The value used to fill outliers.
 * @param   quality     Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for nearest-neighbor interpolation.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination image must have the same format and number of
 *          channels as the source one.
 * @ingroup MinImgPrc_Spatial
 *
 * The function performs transformation of every element of the input image and
 * stores the results in @c p_dst_image. The transformation engine just contains
 * shifts relative to @c p_src_image pixels. Special value of 0x80000000 is used
 * to indicate out of bounds value.
 */
MINIMGPRC_API int GeneralTransformImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    const MinImg *p_trn,
    const void   *p_bg_pixel,
    QualityOption quality);

/**
 * @brief   Performs a look-up table transform of an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   p_lut       The look-up table.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for @c FMT_UINT images.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function fills the destination image with values from the look-up table.
 * Indices of the entries are taken from the source image. That is, the function
 * processes each element of @c p_src_image as follows:
 * @f[ p{\_}dst{\_}image(x, y) = p{\_}lut(p{\_}src{\_}image(x, y)) @f]
 * In the case of multi-channel source image, the table should have the same
 * number of channels as in the source image.
 * Single channel to multi-channel transform is supported.
 */
MINIMGPRC_API int ApplyLUT(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    const MinImg *p_lut_image);

/**
 * @brief   Performs a linear transform of an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   mul         The multiplier of transform.
 * @param   add         The absolute term of transform.
 * @param   pc          The required calculation precision.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @remarks Unlike most parts of minimgprc, the function saturates values for
 *          all integer types. Other types are still not saturated.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function processes each element of @c p_src_image as follows:
 * @f[ p{\_}dst{\_}image(x, y) = p{\_}src{\_}image(x, y) \cdot mul + add  @f]
 * All the channels of multi-channel arrays are processed independently. The
 * type of conversion is done with rounding and saturation, that is if the
 * result can not be represented exactly by a value of the destination array
 * element type, it is set to the nearest representable value on the real axis.
 * The function supports the in-place mode.
 */
MINIMGPRC_API int LinTransformImage(
    const MinImg    *p_dst_image,
    const MinImg    *p_src_image,
    double           mul,
    double           add,
    PrecisionControl pc IS_BY_DEFAULT(PC_PRECISE));

/**
 * @brief   Performs a generalized linear transform of an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   pp_mul      The multiplier of transformation, matrix by rows.
 * @param   p_add       The absolute term of transform, vector.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Unlike most parts of minimgprc, the function saturates values for
 *          all integer types. Other types are still not saturated.
 * @ingroup MinImgPrc_Enhancement
 *
 * This function is a multi-channel generalization of LinTransformImage. It
 * treats each pixes of the source and destination images as a vector and
 * process each element of @c p_src_image as follows:
 * @f[ p{\_}dst{\_}image(x, y) =
 *     pp{\_}mul \cdot p{\_}src{\_}image(x, y) + p{\_}add @f]
 */
MINIMGPRC_API int LinTransformImageEx(
    const MinImg        *p_dst_image,
    const MinImg        *p_src_image,
    const double *const *pp_mul,
    const double        *p_add);

/**
 * @brief   Increases the contrast of an image.
 * @param   p_dst_image       The destination image.
 * @param   p_src_image       The source image.
 * @param   saturate_black    The quantile of black pixels.
 *                            Valid range is [0, 1].
 * @param   saturate_white    The quantile of white pixels.
 *                            Valid range is [0, 1].
 * @param   binding_point     Brightness control for images with contrast
 *                            insufficient to maximize with specified
 *                            @c max_amplification.
 *                            Valid range is [0, 1] if @c max_amplification is
 *                            specified.
 * @param   max_amplification Restriction of maximal amplification of the image.
 *                            Non-positive value means no restriction.
 * @param   precision         The required calculation precision.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for @c TYP_UINT8 images with @c channels == 1.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function performs image value scale stretching. Quantiles
 * @c saturate_black and @c saturate_white are used to compute values on the
 * source scale which shall be mapped to minimum and maximum values on the
 * destination scale, respectively. This interval is stretched linearly either
 * to full range or by max_amplification (if specified) factor, whether is
 * weaker. If max_amplification applies, values below minimum or above maximum
 * will not be saturated except due to restrictions of the data range. Also,
 * place of specified source interval on the destination scale is controlled by
 * @c binding_point, which specifies relative position on the source interval
 * that will have same relative coordinate on the destination range. Higher
 * @c binding_point means brighter destination image.
 */
MINIMGPRC_API int AutoContrastImage(
    const MinImg    *p_dst_image,
    const MinImg    *p_src_image,
    double           saturate_black,
    double           saturate_white,
    double           binding_point     IS_BY_DEFAULT(-1.),
    double           max_amplification IS_BY_DEFAULT(-1.),
    PrecisionControl precision         IS_BY_DEFAULT(PC_PRECISE));

/**
 * @brief   Adjusts the contrast of an image and it's brightness histogram.
 * @param   p_dst_image       The destination image.
 * @param   p_src_image       The source image.
 * @param   p_dst_hist        The brightness histogram of the destination image.
 *                            Can be NULL if no resulting histogram is needed.
 * @param   p_src_hist        The brightness histogram of the source image.
 * @param   saturate_black    The quantile of black pixels.
 *                            Valid range is [0, 1].
 * @param   saturate_white    The quantile of white pixels.
 *                            Valid range is [0, 1].
 * @param   binding_point     Brightness control for images with contrast
 *                            insufficient to maximize with specified
 *                            @c max_amplification.
 *                            Valid range is [0, 1] if @c max_amplification is
 *                            specified.
 * @param   max_amplification Restriction of maximal amplification of the image.
 *                            Non-positive value means no restriction.
 * @param   precision         The required calculation precision.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for @c TYP_UINT8 images.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Enhancement
 *
 * This function is almost equivalent to @c #AutoContrastImage with exception
 * that the precomputed brightness histogram of the source image must be
 * provided and the brightness histogram of the destination image is
 * (optionally) constructed.
 */
MINIMGPRC_API int AutoContrastImageEx(
    const MinImg    *p_dst_image,
    const MinImg    *p_src_image,
    uint32_t        *p_dst_hist,
    const uint32_t  *p_src_hist,
    double           saturate_black,
    double           saturate_white,
    double           binding_point     IS_BY_DEFAULT(-1.),
    double           max_amplification IS_BY_DEFAULT(-1.),
    PrecisionControl precision         IS_BY_DEFAULT(PC_PRECISE));

/**
 * @brief   Apply gamma correction to an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   gamma       The gamma correction rate.
 * @param   p_fixpoint  Fixed point.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for @c TYP_UINT8, @c TYP_UINT16, @c TYP_REAL32 and
 *          @c TYP_REAL64 images.
 * @remarks When fixed point is NULL, default values of 256 for @c TYP_UINT8,
 *          65535.0 for @c TYP_UINT16 and 1.0
 *          for @c TYP_REAL32 or @c TYP_REAL64 are used.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function applies gamma correction to the source image as follows:
 * @f[ p{\_}dst{\_}image(x, y) =
 *    (\frac{p{\_}src{\_}image(x, y)}{p{\_}fixpoint})^{gamma} \cdot
 *    p{\_}fixpoint @f]
 * The function supports the in-place mode. In the case of multi-channel images
 * each channel is processed independently.
 */
MINIMGPRC_API int GammaCorrectionImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        gamma,
    void         *p_fixpoint IS_BY_DEFAULT(NULL));

/**
 * @brief   Smooths image using normalized box filter.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   wx          The horizontal wing of the smoothing kernel.
 * @param   wy          The vertical wing of the smoothing kernel.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Filter
 *
 * The function smooths the image using normalized box filter. The function
 * supports the in-place mode. In the case of multi-channel images each channel
 * is processed independently.
 */
MINIMGPRC_API int AverFilterImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Smooths an images with a Gaussian filter.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   sigma_x     The Gaussian sigma in the horizontal direction.
 * @param   sigma_y     The Gaussian sigma in the vertical direction.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Filter
 *
 * The function convolves the source image with the specified Gaussian kernel.
 * The function supports the in-place mode. In the case of multi-channel images
 * each channel is processed independently.
 */
MINIMGPRC_API int GaussFilterImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        sigma_x,
    double        sigma_y,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Calculates a Pseudo-Laplacian of an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   sigma_1     The sigma of the first Gaussian filter.
 * @param   sigma_2     The sigma of the second Gaussian filter.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.s
 * @ingroup MinImgPrc_Filter
 *
 * The function calculates a Pseudo-Laplacian of the image (that is, the
 * difference of two Gaussian filters bounded below by zero). The function
 * supports the in-place mode. In the case of multi-channel images each channel
 * is processed independently.
 */
MINIMGPRC_API int PseudoLaplacianFilterImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        sigma_1,
    double        sigma_2,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Computes a structural tensor of an image.
 * @param   p_dst_tensor The destination tensor.
 * @param   p_src_image  The source image.
 * @param   sigma_x      The sigma of Gaussian smoothing in the horizontal direction.
 * @param   sigma_y      The sigma of Gaussian smoothing in the vertical direction.
 * @param   central      Specifies whether compute centered structure.
 * @param   bc           The border condition (see @c #BorderOption).
 * @param   p_bg_pixel   The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination images must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Tensor
 *
 * The function computes a structural tensor as follows:
 * @f[ p{\_}im{\_}dx(x, y) = \frac{p{\_}src{\_}image(x, y)}{dx},
 *     p{\_}im{\_}dy(x, y) = \frac{p{\_}src{\_}image(x, y)}{dy} @f]
 * @f[ p{\_}dst{\_}image[0](x, y) =
 *                    GAver(p{\_}im{\_}dx(x, y) \cdot p{\_}im{\_}dx(x, y)) @f]
 * @f[ p{\_}dst{\_}image[1](x, y) =
 *                    GAver(p{\_}im{\_}dx(x, y) \cdot p{\_}im{\_}dy(x, y)) @f]
 * @f[ p{\_}dst{\_}image[2](x, y) =
 *                    GAver(p{\_}im{\_}dy(x, y) \cdot p{\_}im{\_}dy(x, y)) @f]
 * where @c GAver() is gaussian averaging of the image. If the @c central is set,
 * then the destination will be centered:
 * @f[ p{\_}dst{\_}image[0](x, y) = p{\_}dst{\_}image[0](x, y) -
 *     GAver(p{\_}im{\_}dx(x, y)) \cdot GAver(p{\_}im{\_}dx(x, y)) @f]
 * @f[ p{\_}dst{\_}image[1](x, y) = p{\_}dst{\_}image[1](x, y) -
 *     GAver(p{\_}im{\_}dx(x, y)) \cdot GAver(p{\_}im{\_}dy(x, y)) @f]
 * @f[ p{\_}dst{\_}image[2](x, y) = p{\_}dst{\_}image[2](x, y) -
 *     GAver(p{\_}im{\_}dy(x, y)) \cdot GAver(p{\_}im{\_}dy(x, y)) @f]
 */
MINIMGPRC_API int StTensor(
    const MinImg *p_dst_tensor[3],
    const MinImg *p_src_image,
    double        sigma_x,
    double        sigma_y,
    bool          central,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Binarizes an image using a fixed threshold value.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   p_threshold The threshold value.
 * @param   is_invert   Specifies whether the image should be inverted.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks The destination image must be unsigned 8-bit integer,
 *          single-channel.
 * @remarks The source image must be single-channel.
 * @remarks Both source and destination images must have the same size.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @ingroup MinImgPrc_Binarization
 *
 * The function applies fixed-level thresholding to a single-channel array:
 * @f[ p{\_}dst{\_}image(x, y) = \left\{ \begin{array} {rl}
 *       0 & p{\_}src{\_}image(x, y) < p{\_}threshold \cr
 *     255 & otherwise \end{array} \right. @f]
 * If the flag @c is_invert is set, then the destination image will be inverted.
 */
MINIMGPRC_API int BinarizeSimpleImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    const void   *p_threshold,
    bool          is_invert);

/**
 * @brief Structure to hold the result of @ref ComputeOtsuLevel function.
 */
typedef struct OtsuLevel {
  /**
   *  @f[ \sigma_b^2 = \omega_{left} \omega_{right} \cdot (\mu_{left} - \mu_{right}))^2 @f] is the measurement of the distance between left and right clusters.
   *  @f[ \omega_{left}, \omega_{right} @f] -- are the weights of the left and right clusters.
   *  @f[ \mu_{left}, \mu_{right} @f] -- are the means of left and right clusters.
   */
  double between_variance;
  /**
   *  @f[ \sigma_w^2 = \omega_{left} \cdot \sigma_{left}^2 +
   *                   \omega_{right} \cdot \sigma_{right}^2 @f] -- is the weighted dispersion of left and right classes
   *  @f[ \sigma_{left}^2, \sigma_{right}^2  @f] -- are the dispertions of the left and right clusters.
   */
  double within_variance;
  /**
   *  Histogram index that represents optimal binarization threshold. In case
   *  there is a set of optimal thresholds the level is the nearest value
   *  to then histogram mean (@f$mean_{total}@f$) inside [level_left, level_right].
   *  @f$mean_{total} = \frac{1}{weight_{total}} \sum\limits_{i = 0}^{hist_{size}} i \cdot hist[i]]@f$,
   *  where @f$hist_{size}@f$ is the number of histograms bins and @f$weight_{total} = \sum\limits_{i=0}^{hist_{size}} hist[i]@f$
   */
  int    level;
  /**
   *  Histogram index that represents left boundary on possible value of the level.
   */
  int    level_left;
  /**
   *  Histogram index that represents right boundary on possible value of the level.
   */
  int    level_right;

#ifdef __cplusplus
  /**
   * @brief Computes achieved performance.
   *
   * This method assumes that the value of structure it operates on was computed
   * with @ComputeOtsuLevel function.
   *
   * The performance it returns is a value in range [0.0, 1.0]. The higher
   * the performance, the better result was achieved.
   *
   * Please read "t -tests, F -tests and Otsu's Methods for Image Thresholding"
   * by Jing-Hao Xue and D. Michael Titterington, IEEE Trans. on Image
   * Processing, 2010 to understand the nature of this performance measure.
   */
  double GetPerformance() const {
    return between_variance / (within_variance + between_variance);
  }
#endif // __cplusplus
} OtsuLevel;

/**
 * @brief Computes the thresholding level by Otsu's algorithm.
 * @param otsu_level     pointer to @OtsuLevel structure which will be filled by
 *                       algorithm
 * @param histogram      pointer to MinImg representing the histogram Otsu's
 *                       algorithm to be applied to.
 * @param least_level    the least level to be returned
 * @param greatest_level the greatest level to be returned
 * @param unbalanced_classes enables modified Otsu's criterion
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented only for @histogram type TYP_UINT32.
 *
 * The function takes as input @histogram of some binary image and range
 * [@ref least_level, @ref greatest_level] and performs exhaustive search for
 * optimal thresholding level by Otsu's algorithm[1].
 *
 * @histogram must be an MinImage representing a horizontal array of
 * scalars. That is it's width equals the length of the array and it's height
 * and number of channels equal 1.
 *
 * Refer for details to Nobuyuki Otsu (1979). "A threshold selection method
 * from gray-level histograms". IEEE Trans. Sys., Man., Cyber. 9 (1):62–66.
 *
 * Kurita, T., Otsu, N., & Abdelmalek, N. (1992). Maximum likelihood
 * thresholding based on population mixture models.
 * Pattern recognition, 25(10), 1231-1240.
 */
MINIMGPRC_API int ComputeOtsuLevel(
    OtsuLevel    *otsu_level,
    const MinImg *histogram,
    int           least_level,
    int           greatest_level,
    bool          unbalanced_classes IS_BY_DEFAULT(false));

/**
 * @brief   Binarizes an image using Otsu's algorithm
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   is_invert   Specifies whether the image should be inverted.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Both source and destination images must have the same size.
 * @remarks Both source and destination images must be unsigned 8-bit integer
 *          single-channel.
 * @ingroup MinImgPrc_Binarization
 *
 * The function determines the optimal threshold value using Otsu’s algorithm
 * and uses it instead of the specified @c p_threshold in
 * @c BinarizeSimpleImage().
 * If the flag @c is_invert is set, then the destination image will be inverted.
 */
MINIMGPRC_API int BinarizeOtsuImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    bool          is_invert,
    bool          unbalanced_classes IS_BY_DEFAULT(false));

/**
 * @brief   Binarizes an image using Niblack's algorithm.
 * @param   p_dst_image   The destination image.
 * @param   p_src_image   The source image.
 * @param   wx            The horizontal wing of the local window.
 * @param   wy            The vertical wing of the local window.
 * @param   ratio         The relative contrast threshold.
 * @param   add_deviation The priori noise level.
 * @param   border_option Border condition (see @c #BorderOption).
 * @param   is_invert     Specifies whether the image should be inverted.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Both source and destination images must have the same size.
 * @remarks Implemented for TYP_INT8 and TYP_UINT8 (see @c #MinTyp).
 * @ingroup MinImgPrc_Binarization
 *
 * The function binarizes a grayscale image according to the formula:
 * @f[ p{\_}dst{\_}image(x, y) =
 *    \left\{ \begin{array} {rl}  255 & p{\_}src{\_}image(x, y) > T(x, y) \cr
 *            0                       & otherwise  \end{array} \right. @f]
 * Here @f$ T(x, y) @f$ is a threshold calculated individually for each pixel
 * according to Niblack's algorithm.
 * If the flag @c is_invert is set, then the destination image will be inverted.
 */
MINIMGPRC_API int BinarizeNiblackImage(
    const MinImg      *p_dst_image,
    const MinImg      *p_src_image,
    const int          wx,
    const int          wy,
    const double       ratio,
    const double       add_deviation,
    const BorderOption border_option,
    const bool         is_invert);


/**
 * @brief Function is under construction, use BinarizeNiblackImage instead.
 */
MINIMGPRC_API int BinarizeGaussImage(
    const MinImg      *p_dst_image,
    const MinImg      *p_src_image,
    const double       sigma_0,
    const double       sigma_1,
    const double       a,
    const double       b,
    const BorderOption border_option,
    const bool         is_invert);

/**
 * @brief   Applies binary operations to images.
 * @param   p_dst_image   The destination image.
 * @param   p_src_image_1 The first operand (image).
 * @param   p_src_image_2 The second operand (image).
 * @param   op            The binary operation (see @c #BiOp).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks All images must have the same format.
 * @remarks There are four possible cases:
 *           1) first, second and destination images have the same size,
 *              and the same number of channels;
 *           2) any of the operands is scalar image;
 *           3) any of the operands is pixel image;
 *           4) first and second images have the same number of channels,
 *              and the same width but different height.
 * @remarks The destination image 3d size must equal to maximal 3d size
 *          of operand images. The image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @ingroup MinImgPrc_Merge
 *
 * The function applies the specified binary operation @c op to two images
 * @c p_src_image_1 and @c p_src_image_2 and holds the result to @c p_dst_image.
 * @c p_dst_image is defined as follows:
 * In case 1: @f[ p{\_}dst{\_}image(x, y, c) =
 *                op(p{\_}src{\_}image{\_}1(x, y, c),
 *                p{\_}src{\_}image{\_}2(x, y, c)) @f]
 * In case 2: @f[ p{\_}dst{\_}image(x, y, c) =
 *                op(p{\_}src{\_}image{\_}1(x, y, c),
 *                p{\_}src{\_}image{\_}2(0, 0, 0)) @f]
 * In case 3: @f[ p{\_}dst{\_}image(x, y, c) =
 *                op(p{\_}src{\_}image{\_}1(x, y, c),
 *                p{\_}src{\_}image{\_}2(0, 0, c)) @f]
 * In case 4: @f[ p{\_}dst{\_}image(x, y, c) =
 *                op(p{\_}src{\_}image{\_}1(x, y, c),
 *                   p{\_}src{\_}image{\_}2(
 *                            x, y \mod{p{\_}src{\_}image{\_}1.height}, c)) @f]
 * At the present time the following operations are implemented:
 *   @li @c BIOP_MIN - binary minimum;
 *   @li @c BIOP_MAX - binary maximum;
 *   @li @c BIOP_ADD - binary addition;
 *   @li @c BIOP_DIF - binary difference;
 *   @li @c BIOP_ADF - absolute difference;
 *   @li @c BIOP_MUL - binary multiplication;
 *   @li @c BIOP_EUC - binary Euclidean norm.
 *   @li @c BIOP_POW - binary power operation.
 *
 * The function supports the in-place mode.
 */
MINIMGPRC_API int MergeImage(
    const MinImg  *p_dst_image,
    const MinImg  *p_src_image_1,
    const MinImg  *p_src_image_2,
    BiOp           op);

/**
 * @brief   Erodes an image by using a rectangular structuring element.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   wx          The horizontal wing of the structuring element.
 * @param   wy          The vertical wing of the structuring element.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @remarks Implemented only for @c #BO_CONSTANT and @c #BO_VOID border
 *          conditions.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @ingroup MinImgPrc_Morphology
 *
 * The function erodes the source image using the rectangular structuring
 * element: @f[ p{\_}dst{\_}image(x, y) =
 * \min_{|x'| \le wx, |y'| \le wy}{p{\_}src{\_}image(x + x', y + y')} @f]
 * The function supports the in-place mode. In the case of multi-channel images
 * each channel is processed independently.
 */
MINIMGPRC_API int ErodeImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Dilates an image by using a rectangular structuring element.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   wx          The horizontal wing of the structuring element.
 * @param   wy          The vertical wing of the structuring element.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for @c #BO_CONSTANT and @c #BO_VOID border
 *          conditions.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Morphology
 *
 * The function dilates the source image using the rectangular structuring
 * element: @f[ p{\_}dst{\_}image(x, y) =
 * \max_{|x'| \le wx, |y'| \le wy}{p{\_}src{\_}image(x + x', y + y')} @f]
 * The function supports the in-place mode. In the case of multi-channel images
 * each channel is processed independently.
 */
MINIMGPRC_API int DilateImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Opens an image by using a rectangular structuring element.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   wx          The horizontal wing of the structuring element.
 * @param   wy          The vertical wing of the structuring element.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for @c #BO_CONSTANT and @c #BO_VOID border
 *          conditions.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Morphology
 *
 * The function opens the source image using the rectangular structuring
 * element: @f[ p{\_}dst{\_}image =
 *                        dilate(erode(p{\_}src{\_}image, wx, wy), wx, wy) @f]
 * The function supports the in-place mode. In the case of multi-channel images
 * each channel is processed independently.
 */
MINIMGPRC_API int OpenImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Closes an image by using a rectangular structuring element.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   wx          The horizontal wing of the structuring element.
 * @param   wy          The vertical wing of the structuring element.
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented only for @c #BO_CONSTANT and @c #BO_VOID border
 *          conditions.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_Morphology
 *
 * The function closes the source image using the rectangular structuring element:
 * @f[ p{\_}dst{\_}image = erode(dilate(p{\_}src{\_}image, wx, wy), wx, wy) @f]
 * The function supports the in-place mode. In the case of multi-channel images
 * each channel is processed independently.
 */
MINIMGPRC_API int CloseImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @defgroup MinImgPrc_Hough Fast Hough Transform
 *
 * The Hough transform is a feature extraction technique used in image analysis,
 * computer vision, and digital image processing. The purpose of the technique
 * is to find imperfect instances of objects within a certain class of shapes by
 * a voting procedure. The library contains a fast implementation of 2D Hough
 * transform.
 */

/**
 * @brief   Calculates 2D Hough transform of an image.
 * @param   p_dst_image  The destination image, result of transformation.
 * @param   p_src_image  The source (input) image.
 * @param   operation    The operation to be applied.
 * @param   is_vertical  Primarily direction of lines (horizontal or vertical).
 * @param   is_clockwise The direction of angle changes.
 * @param   aspl         Shift per line for simulation of Hough transform for
 *                       skewed image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @ingroup MinImgPrc_Hough
 *
 * The function calculates the Hough transform for a quarter of a range of
 * angles.
 */
MINIMGPRC_API int FHT2(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    CoOp          operation,
    bool          is_vertical,
    bool          is_clockwise,
    double        aspl IS_BY_DEFAULT(0.0));

/**
* @brief   Creates image header for Fast Hough transform of an image.
* @param   p_dst_image The destination image, result of transformation.
* @param   p_src_image The source (input) image.
* @param   angle_range The part of Hough space to calculate.
* @param   allocation  Specifies whether the destination image should be
*                      allocated.
* @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
* @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
* @ingroup MinImgPrc_Hough
*
* The function creates image header for the result of fast Hough transform for
* full, half or quarter range of angles. If @c allocation is set to
* @c AO_PREALLOCATED (the default) then the image will also be allocated.
*/
MINIMGPRC_API int CloneFHTMinImagePrototype(
    MinImg          *p_dst_image,
    const MinImg    *p_src_image,
    MinTyp           type,
    AngleRangeOption angle_range IS_BY_DEFAULT(ARO_315_135),
    AllocationOption allocation  IS_BY_DEFAULT(AO_PREALLOCATED));

/**
* @brief   Calculates 2D Fast Hough transform of an image.
* @param   p_dst_image The destination image, result of transformation.
* @param   p_src_image The source (input) image.
* @param   angle_range The part of Hough space to calculate.
* @param   operation   The operation to be applied.
* @param   make_skew   Specifies to do or not to do image skewing.
* @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
* @remarks The destination image must be already allocated.
* @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
* @ingroup MinImgPrc_Hough
*
* The function calculates the fast Hough transform for full, half or quarter
* range of angles.
*/
MINIMGPRC_API int FastHoughTransform(
    const MinImg     *p_dst_image,
    const MinImg     *p_src_image,
    AngleRangeOption  angle_range IS_BY_DEFAULT(ARO_315_135),
    CoOp              operation   IS_BY_DEFAULT(COOP_ADD),
    HoughDeskewOption make_skew   IS_BY_DEFAULT(HDO_DESKEW));

/**
* @brief   Calculates coordinates of line segment corresponded by point in
*          Hough space.
* @param   p_line           Coordinates of line segment corresponded by point in
*                           Hough space.
* @param   hough_point      Point in Hough space.
* @param   p_src_image_info The source (input) image of Hough transform.
* @param   angle_range      The part of Hough space where point is situated.
* @param   make_skew        Specifies to do or not to do image skewing.
* @param   rules            Specifies strictness of line segment calculating
*                           (see @c #RulesOption).
* @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
* @remarks The destination line segment must be already allocated.
* @remarks If @c rules parameter has @c RO_IGNORE_BORDERS bit clear
           then returned line cuts along the border of source image.
* @remarks If @c rules parameter has @c RO_IGNORE_BORDERS bit set then in case
*          of point, which belongs to the incorrect part of Hough image,
*          returned line will not intersect source image.
* @remarks The function returns INTERNAL_ERROR if the point belongs to the
*          incorrect part of Hough image and @c #RO_IGNORE_BORDERS bit is clear.
* @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
* @ingroup MinImgPrc_Hough
*
* The function calculates coordinates of line segment corresponded by point in
* Hough space.
*/
MINIMGPRC_API int HoughPoint2Line(
    MinLineSegment   *p_line,
    const MinPoint    hough_point,
    const MinImg     *p_src_image_info,
    AngleRangeOption  angle_range       IS_BY_DEFAULT(ARO_315_135),
    HoughDeskewOption make_skew         IS_BY_DEFAULT(HDO_DESKEW),
    RulesOption       rules             IS_BY_DEFAULT(RO_IGNORE_BORDERS));

/**
 * TODO: Radon documentation
 */
MINIMGPRC_API int CloneRadonMinImagePrototype(
    MinImg*          p_radon_image,
    const MinImg*    p_src_image,
    int32_t          angle_range,
    AllocationOption allocation IS_BY_DEFAULT(AO_PREALLOCATED));

MINIMGPRC_API int RadonBackProjection(
    const MinImg* p_dst_image,
    const MinImg* p_src_radon_image,
    real64_t      delta_x,
    real64_t      delta_y);

MINIMGPRC_API int RadonBackProjectionEx(
    const MinImg*   p_dst_image,
    const MinImg*   p_src_radon_image,
    real64_t        delta_x,
    real64_t        delta_y,
    const real64_t* angles);

MINIMGPRC_API int RadonTransform(
    const MinImg* p_dst_radon_image,
    const MinImg* p_src_image,
    real64_t      delta_x,
    real64_t      delta_y);

MINIMGPRC_API int RadonTransformEx(
    const MinImg*   p_dst_radon_image,
    const MinImg*   p_src_image,
    real64_t        delta_x,
    real64_t        delta_y,
    const real64_t* angles);

/**
 * @defgroup MinImgPrc_Integral Integral Image
 *
 * An integral image (also known as a summed area table)  is an algorithm for
 * quickly and efficiently generating the sum of values in a rectangular subset
 * of a grid.
 */

/**
 * @brief   Calculates the integral of an image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   direction   Direction, in which to integrate image
 *                      (see @c #DirectionOption).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size and the
 *          same number of channels.
 * @ingroup MinImgPrc_Integral
 *
 * The function calculates the integral images for the source image as
 * following: \f[ p{\_}dst{\_}image(x, y) =
 *                \sum_{x' \leq x, y' \leq y}{p{\_}src{\_}image(x', y')} \f]
 * Using these integral images, one may calculate sum over a specific up-right
 * rectangular region of the image in a constant time. In the case of
 * multi-channel images, sums for each channel are accumulated independently.
 */
MINIMGPRC_API int IntegrateImage(
    const MinImg   *p_dst_image,
    const MinImg   *p_src_image,
    DirectionOption direction    IS_BY_DEFAULT(DO_BOTH));

/**
 * @brief   Calculates weighted average of two images.
 * @param   p_dst_image   The destination image.
 * @param   p_src_image_1 The source image.
 * @param   p_src_image_2 The background image.
 * @param   p_wgt_1       The weight map image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Both source and destination images must have the same size.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination, source, and background images must have the same
 *          format and the same number of channels.
 * @ingroup MinImgPrc_Blend
 *
 * The function blends the source image with background as follows:
 * @f[ p{\_}dst{\_}image(x, y) =
 *          p{\_}wgt{\_}1(x, y) \cdot p{\_}src{\_}image{\_}1(x, y) +
 *          (1 - p{\_}wgt{\_}1(x, y)) \cdot p{\_}src{\_}image{\_}2(x, y) @f]
 * The constant background or weight map can be specified as scalar (that is, as
 * single-pixel image). In the case of multi-channel images each channel is
 * processed independently.
 */
MINIMGPRC_API int BlendImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image1,
    const MinImg *p_src_image2,
    const MinImg *p_wgt_1);

/**
 * @brief   Applies two images.
 * @param   p_dst_image   The destination image.
 * @param   p_src_image_1 The source image.
 * @param   p_src_image_2 The background image.
 * @param   p_wgt_1       The weight map image.
 * @param   p_threshold   The threshold value.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Both source and destination images must have the same size.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination, source, and background images must have the same
 *          format and the same number of channels.
 * @ingroup MinImgPrc_Blend
 *
 * The function blends the source image with background as follows:
 * @f[ p{\_}dst{\_}image(x, y) = \left\{
 *     \begin{array} {rl} p{\_}src{\_}image{\_}1(x, y) &
 *     p{\_}wgt{\_}1(x, y) > p{\_}threshold \cr
 *     p{\_}src{\_}image{\_}2(x, y) & otherwise\end{array} \right. @f]
 * The constant background or weight map can be specified as scalar (that is, as
 * single-pixel image). In the case of multi-channel images each channel is
 * processed independently.
 */
MINIMGPRC_API int ApplyImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image1,
    const MinImg *p_src_image2,
    const MinImg *p_wgt_1,
    const void   *p_threshold);

/**
 * @brief   Combines two images.
 * @param   p_combined     The destination image.
 * @param   p_true_filler  The true filler image.
 * @param   p_false_filler The false filler image.
 * @param   p_argument_a   The first argument image.
 * @param   p_argument_b   The second argument image.
 * @param   predicate      The predicate which defines filling of the
 *                         destination image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Both source and destination images must have the same size.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks The destination, true filler and false filler images must have the
 *          same format and the same number of channels.
 * @ingroup MinImgPrc_Blend
 *
 * The function blends the p_true_filler and p_false_filler as follows:
 * @f[ p{\_}combined(x, y) = \left\{ \begin{array}
 *     {rl} p{\_}true{\_}filler(x, y) &
 *     if\ predicate(p{\_}argument{\_}a(x, y), p{\_}argument{\_}b(x, y)) == true
 *     \cr p{\_}false{\_}filler(x, y) &
 *     if\ predicate(p{\_}argument{\_}a(x, y), p{\_}argument{\_}b(x, y)) == false
 *     \end{array} \right. @f]
 * In the case of multi-channel images each channel is processed independently.
 */
MINIMGPRC_API int CombineImage(
    const MinImg   *p_combined,
    const MinImg   *p_true_filler,
    const MinImg   *p_false_filler,
    const MinImg   *p_argument_a,
    const MinImg   *p_argument_b,
    PredicateOption predicate);

/**
 * @brief   Computes a gradient of an image.
 * @param   p_dx_image  The X component of the gradient.
 * @param   p_dy_image  The Y component of the gradient.
 * @param   p_src_image The source image.
 * @param   mask        The mask of gradient (see @c #GradientMaskOption).
 * @param   bc          The border condition (see @c #BorderOption).
 * @param   p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_Enhancement
 *
 * The function computes image gradient (@c p_dx_image corresponds to x
 * derivative, @c p_dy_image corresponds to y derivative) calculated with the
 * @c mask, that defines which approximation is used.
 */
MINIMGPRC_API int GetImageGradient(
    const MinImg      *p_dx_image,
    const MinImg      *p_dy_image,
    const MinImg      *p_src_image,
    GradientMaskOption mask,
    BorderOption       bc,
    const void        *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief   Applies unary operation to image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   operation   The unary operation (see @c #UnOp).
 * @param   pc          The required calculation precision.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be already allocated.
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both source and destination images must have the same size, the same
 *          format, and the same number of channels.
 * @ingroup MinImgPrc_API
 *
 * The function applies the specified unary operation @c operation to source
 * image @c p_src_image and holds the result to @c p_dst_image:
 * @f[ p{\_}dst{\_}image(x, y) = op(p{\_}src{\_}image(x, y)) @f]
 * At the present time the following operations are implemented:
 *   @li @c UNOP_NEG - unary minus operation;
 *   @li @c UNOP_ABS - unary absolute value;
 *   @li @c UNOP_RCPR - unary reciprocal value;
 *   @li @c UNOP_SQRT - unary square root;
 *   @li @c UNOP_NOT - unary bitwise NOT.
 *   @li @c UNOP_INV - unary inversion - deprecated.
 *   @li @c UNOP_LOG - unary logarithm.
 *          When pc is PC_PRECISE returns exact log value;
 *          When pc is PC_MEDIUM and image type is @c #TYP_REAL32 returns
 *          @f[ \log(x) = \left\{ \begin{array}
 *          {rl} -inf & if x == 0
 *          \cr log{\_}approx{\_}(x), |log{\_}approx{\_}(x) - log(x)| < 1e-5, &
 *          if x is normalized rea32 value
 *          \cr v < log(FLT_MIN), &
 *          if 0 < x < FLT_MIN, where FLT_MIN is minimal normalized real32 value
 *          \cr -nan, & otherwise
 *     \end{array} \right. @f]
 * @remarks The UNOP_LOG operation is implemented for @c #TYP_REAL32 and
 *          @c #TYP_REAL64 images only.
 * In the case of multi-channel images each channel is processed independently.
 * The function supports the in-place mode.
 *   @li @c UNOP_EXP - unary exponent.
 *          When pc is PC_PRECISE returns exact exp value;
 *          When pc is PC_LOW and image type is @c #TYP_REAL32 returns
 *          @f[ \exp(x) = \left\{ \begin{array}
 *          {rl} 0, & if x < MIN\_EXP\_FLT,
 *          \cr a y + (b - c), &
 *          if MIN\_EXP\_FLT \leq x \leq MAX\_EXP\_FLT, where a = 2^{20}/\log(2),
 *          b = 1023 \cdot 2^{20} and c = 486411,
 *          \cr +inf, & otherwise,
 *     \end{array} \right. @f]
 *          where MIN_EXP_FLT and MAX_EXP_FLT is minimal and maximal real32 values
 *          respectively, that exponent might be computed in valid real32 values
 * @remarks The UNOP_EXP operation is implemented for @c #TYP_REAL32 and
 *          @c #TYP_REAL64 images only.
 */
MINIMGPRC_API int OperateImageUnary(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    UnOp          operation,
    PrecisionControl pc IS_BY_DEFAULT(PC_PRECISE));

/**
 * @brief   Finds edges in an image using Canny algorithm.
 * @param   p_dst_image The destination images with edges.
 * @param   p_src_image The source image.
 * @param   sigma       The sigma of the first Gaussian filter.
 * @param   min_thresh  The first threshold for the hysteresis procedure.
 * @param   max_thresh  The second threshold for the hysteresis procedure.
 * @param   max_factor  The coefficient of maximum edge gain.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The destination image must be @c TYP_UINT8.
 * @remarks Both destination and source images must be single-channel.
 * @remarks Both destination and source images must have the same size.
 * @ingroup MinImgPrc_Edge
 *
 * The function finds edges in the input image image and marks them in the
 * output map edges using the Canny algorithm. The smallest value between
 * minThresh and maxThresh is used for edge linking, the largest value is used
 * to find the initial segments of strong edges.
 */
MINIMGPRC_API int GetImageEdgesCanny(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    double        sigma,
    double        min_thresh,
    double        max_thresh,
    double        max_factor);

typedef struct MinCannyPoint {
  struct MinCannyPoint *previous;
  struct MinCannyPoint *next;
  int x;
  int y;
} MinCannyPoint;

typedef struct MinCannyComp {
  struct MinCannyPoint *head;
  int x;
} MinCannyComp;

typedef struct MinCannyOption {
  int size;  // [out] number of components
  MinCannyComp **comps;  // [out] list of connected components
  real32_t individual_threshold_k;
  real32_t individual_threshold_b;
  real32_t group_threshold_k;
  real32_t group_threshold_b;
  int min_size;
  void *internal;
} MinCannyOption;

/**
 * @brief   Create single argument for GetImageCannyDelineation
 * @param   p_p_option             Pointer to destination option.
 * @param   individual_threshold_k Individual threshold k.
 * @param   individual_threshold_b Individual threshold b.
 * @param   group_threshold_k      Group threshold k.
 * @param   group_threshold_b      Group threshold b.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 */
MINIMGPRC_API int CreateMinCannyOption(
  MinCannyOption** p_p_option,
  real32_t individual_threshold_k,
  real32_t individual_threshold_b,
  real32_t group_threshold_k,
  real32_t group_threshold_b,
  int min_size IS_BY_DEFAULT(0));

/**
 * @brief   Free option created by CreateMinCannyOption
 * @param   option The option.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 */
MINIMGPRC_API int FreeMinCannyOption(
  MinCannyOption *option);

/**
 * @brief   Function to find edges, ridges and valleys on an image
 * @param   p_edges     Option created by CreateMinCannyOption (optional).
 * @param   p_ridges    Option created by CreateMinCannyOption (optional).
 * @param   p_valleys   Option created by CreateMinCannyOption (optional).
 * @param   p_src_image The source image.
 * @param   sigma       The sigma of the first Gaussian filter.
 * @param   p_threshold_image Background image to use (optional).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The source and threshold image image must be sinlge
 *          channel TYP_UINT8.
 */
MINIMGPRC_API int GetImageCannyDelineation(
  MinCannyOption *p_edges,
  MinCannyOption *p_ridges,
  MinCannyOption *p_valleys,
  const MinImg   *p_src_image,
  double          sigma,
  const MinImg   *p_threshold_image);

/**
 * @brief   Draw connected components obtained by GetImageCannyDelineation function
 * @param   p_dst_image The destination image.
 * @param   p_options   The options.
 * @param   min_size    Don't draw componets, shorter than min_size.
 * @param   fill_value  Fill value for p_dst_image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Destination image must be sinlge channel TYP_UINT8 image.
 */
MINIMGPRC_API int PlaceCannyComponents(
  MinImg const *p_dst_image,
  MinCannyOption const *p_options,
  uint8_t fill_value IS_BY_DEFAULT(255));

/**
 * @brief   Finds maximum and minimum values of the image.
 * @param   p_min_value The minimum value.
 * @param   p_max_value The maximum value.
 * @param   p_src_image The source image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function finds minimum and maximum of the source image and places them in
 * @c p_min_value and @c p_max_value respectively.
 */
MINIMGPRC_API int GetImageValueBounds(
    void         *p_min_value,
    void         *p_max_value,
    const MinImg *p_src_image);

/**
 * @brief   Reduce channels of source image to 1.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   operation   The operation of reduction.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Both destination and source images must have the same size.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function reduces each multi-channel pixel in @c p_src_image to 1-channel
 * pixel that is placed in @c p_dst_image. @c operation specifies the operation
 * which is performed on the channels of @c p_src_image to get 1-channel
 * @c p_dst_image.
 * At the present time the following operations are implemented:
 *   @li @c OP_AVE - average of channels;
 *   @li @c OP_MIN - minimum of channels;
 *   @li @c OP_MAX - maximum of channels;
 *   @li @c OP_ADF - absolute difference of channels.
 */
MINIMGPRC_API int ReduceImageChannels(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    CoOp          operation);

/**
 * @brief   Reduce size of the source image.
 * @param   p_dst_image The destination image.
 * @param   p_src_image The source image.
 * @param   operation   The operation of reduction.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented for all acceptable image formats except @c #TYP_REAL16.
 * @remarks Destination and source images must have the same number of channels.
 * @ingroup MinImgPrc_Enhancement
 *
 * The function reduces size of the source image @c p_src_image so that it fits
 * exactly into the destination image @c p_dst_image. The @c operation argument
 * specifies the @c #CoOp operation which is performed on the image areas to
 * resize it. In the case of multi-channel images each channel is processed
 * independently.
 */
MINIMGPRC_API int ReduceImageTiles(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    const CoOp    operation);

/**
 * @brief   Convert image from YUV format to RGB.
 * @param   p_dst_image  The destination image.
 * @param   p_y_image    The source Y-image.
 * @param   p_vu_image    The source VU-image.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented for @c #TYP_UINT8 images.
 * @remarks Destination image must have exactly 3 channels and have the same size
 *          as @c p_y_image.
 * @remarks @c p_y_image must have exactly 1 channel and have the same size
 *          as @c p_dst_image.
 * @remarks @c p_uv_image must have exactly 2 channels, have the same width as
 *          @c p_y_image and half of @c p_y_image's height.
 * @ingroup MinImgPrc_Convert
 *
 * The function converts the source image represented by @c p_y_image and
 * c p_uv_image from YUV format to RGB. The result is placed in @c p_dst_image.
 */
MINIMGPRC_API int YuvToRgbTransformImage(
    const MinImg *p_dst_image,
    const MinImg *p_y_image,
    const MinImg *p_vu_image);

MINIMGPRC_API int Yuv420ToRgb(
    const MinImg *p_dst_image,
    const uint8_t* p_y_data,
    const uint8_t* p_u_data,
    const uint8_t* p_v_data,
    int32_t y_pixel_stride,
    int32_t y_row_stride,
    int32_t u_pixel_stride,
    int32_t u_row_stride,
    int32_t v_pixel_stride,
    int32_t v_row_stride);


/**
 * @brief Computes a histogram of 1-channel image within given bounds.
 * @param histogram_ptr pointer to the MinImg representing the array of scalars
 *                      in which histogram will be collected.
 * @param bounds        pointer to the array of elements of the same type as
 *                      pixels in @c image_ptr. Array must contain exactly two
 *                      elements: bounds[0] is inclusive lower bound and
 *                      bounds[1] is inclusive upper bound.
 * @param image_ptr     pointer to 1-channel image histogram of which will be
 *                      computed.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks Implemented only for @c image_ptr of type @c TYP_UINT8 and for
 * @histogram_ptr of type @c TYP_UINT32 (see @c #MinTyp).
 */
MINIMGPRC_API int Compute1ChImageHistogram(
    MinImg       *histogram_ptr,
    void         *bounds,
    const MinImg *image_ptr);

/**
 * @brief Median filter image
 *        Enhanced Huang's algorithm (with integrated histogram speed-up)
 *        NB: works only for TYP_UINT8 images with [1 ... 16] channels
 *        otherwise returns NOT_IMPLEMENTED
 * @param p_dst_image The destination image.
 * @param p_src_image The source image.
 * @param wx          The horizontal wing of the window.
 * @param wy          The vertical wing of the window.
 * @param bc          The border condition (see @c #BorderOption).
 * @param p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 */
MINIMGPRC_API int MedianFilterImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief Bilateral filter image with rectangular spatial and intensity windows
 *        NB: works only for TYP_UINT8 images with [1 ... 16] channels
 *        otherwise returns NOT_IMPLEMENTED
 * @param p_dst_image The destination image.
 * @param p_src_image The source image.
 * @param wx          The horizontal wing of the window.
 * @param wy          The vertical wing of the window.
 * @param i_wing      The intensity wing
 * @param bc          The border condition (see @c #BorderOption).
 * @param p_bg_pixel  The value used to fill outliers (for @c #BO_CONSTANT).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 */
MINIMGPRC_API int BilateralAverAverFilterImage(
    const MinImg *p_dst_image,
    const MinImg *p_src_image,
    int           wx,
    int           wy,
    double        i_wing,
    BorderOption  bc,
    const void   *p_bg_pixel IS_BY_DEFAULT(0));

/**
 * @brief Morphological restoration by dilation: dilates the image by
 *        8-connectihood with pMask upper bound
 *        NB: works only for TYP_UINT8 images
 * @param p_dst_image The destination image
 * @param p_src_image The source image
 * @param p_mask      The mask image (upper bound for dilation)
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
*/
MINIMGPRC_API int DilateRestoreImage(
    const MinImg* p_dst_image,
    const MinImg* p_src_image,
    const MinImg* p_mask);

/**
 * @brief Morphological restoration by erosion: erodes the image by
 *        8-connectihood with pMask lower bound
 *        NB: works only for TYP_UINT8 images
 * @param p_dst_image The destination image
 * @param p_src_image The source image
 * @param p_mask      The mask image (lower bound for erosion)
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
*/
MINIMGPRC_API int ErodeRestoreImage(
    const MinImg* p_dst_image,
    const MinImg* p_src_image,
    const MinImg* p_mask);

/**
 * @brief Morphological restoration by dilation of (p_src_image - h) with
 *        p_src_image as mask.
 *        Suppresses all local maxima not higher than H
 *        NB: works only for TYP_UINT8 images
 * @param p_dst_image The destination image
 * @param p_src_image The source image
 * @param p_h_values  H values for channels
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
*/
MINIMGPRC_API int ImageHMax(
    const MinImg* p_dst_image,
    const MinImg* p_src_image,
    const void*   p_h_values);

/**
 * @brief Morphological restoration by erosion of (p_src_image + h) with
 *        p_src_image as mask.
 *        Suppresses all local minima not deeper than H
 *        NB: works only for TYP_UINT8 images
 * @param p_dst_image The destination image
 * @param p_src_image The source image
 * @param p_h_values  H values for channels
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
*/
MINIMGPRC_API int ImageHMin(
    const MinImg* p_dst_image,
    const MinImg* p_src_image,
    const void*   p_h_values);

typedef unsigned int conn_comps_height_t;

// !! TODO: move to mingeo.h
#if defined(__cplusplus) && defined(MINGEO_MINGEO_H_INCLUDED)
  typedef se::rect2<unsigned int> MinRect2ui;
#else
  typedef struct MinRect2ui {
    unsigned int x; ///< The x-coordinate of the left-top corner point
    unsigned int y; ///< The y-coordinate of the left-top corner point
    unsigned int width;  ///< The width (x-size) of the rectangle
    unsigned int height; ///< The height (y-size) of the rectangle
  } MinRect2ui;
#endif

/**
 * @brief Structure to describe a run of pixels with the same index.
 */
typedef struct MinRun {
  int x0;
  conn_comps_height_t y;
  int width;
} MinRun;

/**
 * @brief Structure to describe a leaf of connected components list.
 */
typedef struct MinConnComp {
  struct MinConnComp *prev;
  struct MinConnComp *next;
  MinRun *runs;
  size_t runs_sz;

  MinRect2ui rect;
  int area; // ?? TODO: change to larger type?
} MinConnComp;

/**
 * @brief Structure to keep internal information of connected components
 *        search.
 */
typedef struct MinCCState* MinCCStateHandle;

/**
 * @brief Specifies pixel connectivity types
 */
typedef enum {
  CT_4_CONNECTED, ///< By edge, not by corner
  CT_8_CONNECTED  ///< By edge and corner
} ConnectivityType;

/**
 * @brief   Allocates a @c #MinCCState object to use in connected components
 *          search.
 * @param   p_handle          Pointer to created handle.
 * @param   width             Width of the processed image
 * @param   use_background    Specifies whether background should be treated as a
 *                            component with some index.
 * @param   connectivity_type Pixel connectivity type
 * @param   max_y_size        Maximal vertical size of a component.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ConnComps
 *
 * The function creates an object of connected components search state, setting
 * parameters of the search: width of the processed image, whether to treat
 * background as a component with it's own value (false by default), pixel
 * connectivity type (by edge and corder by default) and optional restriction
 * on maximal vertical size of a component. If the latter is set, the component
 * will be counted as a separate one upon reaching this height, even if it's
 * continued further down in terms of adjacent pixels.
 */
MINIMGPRC_API int InitializeCCState(
    MinCCStateHandle *p_handle,
    int width,
    bool use_background IS_BY_DEFAULT(false),
    ConnectivityType connectivity_type IS_BY_DEFAULT(CT_8_CONNECTED),
    conn_comps_height_t max_y_size IS_BY_DEFAULT(std::numeric_limits<conn_comps_height_t>::max()));

/**
 * @brief   Deallocates a @c #MinCCState object.
 * @param   p_handle Pointer to handle of a state.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ConnComps
 *
 * The function deallocates the connected components search state data and
 * clears the handle.
 */
MINIMGPRC_API int FreeCCState(MinCCStateHandle p_state); // handle or handle*?

/**
 * @brief   Extracts the first leaf of connected components list from the
 *          components search state.
 * @param   pp_dst_comp Pointer to the connected component leaf extracted.
 * @param   p_state     Handle of the state of connected components search.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks The component extracted is not deleted from the state.
 * @ingroup MinImgPrc_ConnComps
 *
 * The function extracts the first leaf of connected components list from a
 * components search state, which is the first component found with this state
 * that was not deleted from the state yet.
 */
MINIMGPRC_API int GetFirstConnComp(
    MinConnComp **pp_dst_comp,
    MinCCStateHandle p_state);

/**
 * @brief   Deletes leaves from connected components list in a components search
 *          state.
 * @param   p_state Handle of the state of connected components search.
 * @param   p_comp  The first component to be freed.
 * @param   count   The count of components to be freed.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ConnComps
 *
 * The function deletes and deallocates @c count leaves of connected components
 * list in a components search state, starting from given component @c p_comp.
 * If -1 is given as @c count, then all components from @c p_comp to the end of
 * the list are deleted.
 */
MINIMGPRC_API int FreeConnComp(
    MinCCStateHandle p_state,
    MinConnComp *p_comp,
    int count IS_BY_DEFAULT(1));

/**
 * @brief   Find connected components on the image.
 * @param   pp_dst_ccs      Pointer to the first component found in this search.
 * @param   p_state         Handle of the state of connected components search.
 * @param   p_src_image     The source image.
 * @param   to_be_continued Specifies whether another image should be expected
 *                          representing the next vertical region of the same
 *                          input.
 * @param   image_top_y     Y coordinate of the first line of source image
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ConnComps
 *
 * The function searches for connected components on the image, saving the found
 * components in a search state and returning the pointer to the first component
 * found during this particular search. If @c to_be_continued is true, the
 * components reaching the last line of the image are not treated as complete,
 * and the continuation of the image is expected in the next call of the
 * function. The first line of source image is treated as adjacent to the last line
 * of previously passed image regardless of @c image_top_y.
 */
MINIMGPRC_API int FindConnComps(
    MinConnComp **pp_dst_ccs,
    MinCCStateHandle p_state,
    const MinImg *p_src_image,
    bool to_be_continued IS_BY_DEFAULT(false),
    int image_top_y IS_BY_DEFAULT(0));

/**
 * @brief   A representation of an image scale pyramid.
 * @details The struct @c ScalePyramid represents an array of depth MinImage's,
 *          where image sizes decrease by scale factors defined in @c p_scales_x
 *          and @c p_scales_y arrays.
 * @ingroup MinImgPrc_ScalePyramid
 */
typedef struct ScalePyramid {
  int32_t   depth;
  real64_t *p_scales_x;
  real64_t *p_scales_y;
  MinImg   *p_layers;
} ScalePyramid;

/**
 * @brief   Makes new allocated ScalePyramid.
 * @param   p_pyramid         Pointer to the pyramid.
 * @param   p_scale_factors_x Pointer to array of layer scale factors for x axis.
 * @param   p_scale_factors_y Pointer to array of layer scale factors for y axis.
 * @param   p_image_info      Pointer to the base image prototype for pyramid.
 * @param   depth             Depth (number of layers) of the pyramid.
 * @param   period            Period of scale factors sequences.
 * @param   alignment         Alignment for layer images rows, by default 16 bytes.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks elements of p_scale_factors_x and p_scale_factors_y arrays must be
 *          such that all resulting scales are in the interval (0, 1] and no
 *          layer except the base on must have scale 1.0 for both axes at once.
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function fills the image scale pyramid structure. The sizes of images in the pyramid
 * are changed multiplicatively by elements of scale factor sequences. The scale factor
 * of the first layer is 1, while scale factors of the other layers are defined by periodic
 * sequence consisting of p_scale_factors elements. Scale factor is applied multiplicatively:
 * scale_i = scale_{i - 1} * p_scale_factors[(i - 1) % period]
 */
MINIMGPRC_API int NewScalePyramid(
    ScalePyramid   *p_pyramid,
    const real64_t *p_scale_factors_x,
    const real64_t *p_scale_factors_y,
    const MinImg   *p_image_info,
    int32_t         depth,
    int32_t         period,
    int32_t         alignment IS_BY_DEFAULT(16));

/**
 * @brief   Checks whether scale pyramid is valid or not.
 * @param   p_pyramid      The pyramid.
 * @returns @c NO_ERRORS if the pyramid is valid or @c BAD_ARGS otherwise.
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function checks scale pyramid to have valid depth. If the pyramid has
 * at least one layer, the function checks @c p_pyramid->p_layers,
 * p_pyramid->p_scales_x, and p_pyramid->p_scales_y to be non-zero and located
 * in memory sequentially, checks all layer images to be valid and located in
 * memory sequentially (each layer should take exactly it's height * stride),
 * checks all scales along both axes to be in interval (0, 1] and for all layers
 * except the base one that at least one of x and y scales is less than 1.0,
 * checks that all layers except the base one have sizes corresponding to the
 * sizes of the base layer and nominal scales.
 * Note, that the function returns @c NO_ERRORS on success, which should not be
 * wrongly interpreted as false.
 */
MINIMGPRC_API int AssureScalePyramidIsValid(
    const ScalePyramid *p_pyramid);

/**
 * @brief   Destroys ScalePyramid.
 * @param   p_pyramid       The pointer to the pyramid.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function frees ScalePyramid structure.
 */
MINIMGPRC_API int FreeScalePyramid(
    ScalePyramid *p_pyramid);

/**
 * @brief   Constructs ScalePyramid from input image.
 * @param   p_pyramid       The pointer to the initialized pyramid.
 * @param   p_image         The pointer to the source image.
 * @param   quality     Interpolation method.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function constructs image scale pyramid and stores it to a pre-initialized
 * structure pointed by p_pyramid.
 */
MINIMGPRC_API int ConstructScalePyramid(
    const ScalePyramid *p_pyramid,
    const MinImg       *p_image,
    QualityOption       quality);

/**
 * @brief   Specifies category of pyramid scale to use.
 * @details The enum specifies two categories of scale. The first is nominal
 *          scale, given during pyramid initialization, and the second is actual
 *          scale, resulting from rounding to integer pixel counts along layer
 *          sides.
 */
typedef enum PyramidScaleCategory {
  PSC_NOMINAL,
  PSC_ACTUAL
} PyramidScaleCategory;

/**
 * @brief   Retrieves scales along x and y axes for specified pyramid layer.
 * @param   p_scale_x       The pointer to x scale of the layer.
 * @param   p_scale_y       The pointer to y scale of the layer.
 * @param   p_pyramid       The pointer to the initialized pyramid.
 * @param   layer_ind       The index of the layer.
 * @param   scale_category  The enum specifying scale category to use for search
 *                          (nominal or actual scale).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function gives x and y scales for specified pyramid layer.
 * Depending on @c scale_category, it gives either nominal scale (passed during
 * pyramid initialization) or actual scale (resulted from rounding for integer
 * layer sizes).
 */
MINIMGPRC_API int GetScale(
    real64_t            *p_scale_x,
    real64_t            *p_scale_y,
    const ScalePyramid  *p_pyramid,
    int32_t              layer_ind,
    PyramidScaleCategory scale_category);

/**
 * @brief   Converts coordinates from one ScalePyramid layer to another.
 * @param   p_pyramid       The pointer to the initialized pyramid.
 * @param   p_dst_point     The pointer to the destination coordinates.
 * @param   p_src_point     The pointer to the source coordinates.
 * @param   dst_layer       The number of destination layer.
 * @param   src_layer       The number of source layer.
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function converts coordinates of a point on one image scale pyramid layer to
 * coordinates of point on another pyramid layer.
 */
MINIMGPRC_API int ConvertScalePyramidCoordinates(
    const ScalePyramid  *p_pyramid,
    MinPoint2d          *p_dst_point,
    const MinPoint2d    *p_src_point,
    int32_t              dst_layer,
    int32_t              src_layer,
    PyramidScaleCategory scale_category);

/**
 * @brief   Specifies search mode for the pyramid layer search.
 * @details The enum specifies available search modes. This can be used
 *          to specify which layer is returned: with the nearest scales,
 *          with the closest smaller scales or with the closest larger scales.
 */
typedef enum ScaleLayerSearchMode {
  SLSM_NEAREST,
  SLSM_TO_SMALLER_LAYER,
  SLSM_TO_LARGER_LAYER
} ScaleLayerSearchMode;

/**
 * @brief   Finds a pyramid layer with specified x and y scales and x and y
 *          residuals to this scale.
 * @param   p_layer_index   The pointer to an index of the found pyramid layer.
 * @param   p_residual_x    The pointer to multiplicative residual scale for the x axis.
 * @param   p_residual_y    The pointer to multiplicative residual scale for the y axis.
 * @param   p_pyramid       The pointer to the initialized pyramid.
 * @param   scale_x         The x axis scale to find the layer.
 * @param   scale_y         The y axis scale to find the layer.
 * @param   scale_category  The enum specifying scale category to use for search
 *                          (nominal or actual scale).
 * @param   search_mode     The enum specifying the search mode (nearest, smaller or
 *                          larger layer).
 * @returns @c NO_ERRORS on success or an error code otherwise (see @c #MinErr).
 * @remarks p_residual_x or p_residual_y can be NULL for the case only the layer index
 * is needed.
 * @ingroup MinImgPrc_ScalePyramid
 *
 * The function finds a pyramid layer with the closest scales to speficied and residuals
 * to these scales.
 */
MINIMGPRC_API int FindScalePyramidLayer(
    int32_t             *p_layer_index,
    real64_t            *p_residual_x,
    real64_t            *p_residual_y,
    const ScalePyramid  *p_pyramid,
    real64_t             scale_x,
    real64_t             scale_y,
    PyramidScaleCategory scale_category,
    ScaleLayerSearchMode search_mode IS_BY_DEFAULT(SLSM_NEAREST));

MINIMGPRC_API int GuidedFilter(
    const MinImg *dst_image,
    const MinImg *src_image,
    const MinImg *guid_image,
    int           wx,
    int           wy,
    real32_t      i_wing,
    BorderOption  bo,
    const void   *p_bg_pixel IS_BY_DEFAULT(NULL));

#ifdef __cplusplus
  } // extern "C"
#endif

#endif // #ifndef MINIMGPRC_H_INCLUDED
