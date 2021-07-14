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
 * @file   minresult.h
 * @brief  Definition of possible return values and some macros to operate them.
 */

#pragma once
#ifndef MINSBASE_MINRESULT_H_INCLUDED
#define MINSBASE_MINRESULT_H_INCLUDED

#include "crossplat.h"
//#include <minutils/minlog.h>

/**
 * @defgroup MinBase_MinErr Possible Return Codes
 * @brief    The module specifies the return values used thought the library.
 *           Every function in the library follows the rule: it returns integer
 *           value. Meanwhile, a nonnegative return value indicates that the
 *           function completed successfully whereas a negative value indicates
 *           erroneous execution and specifies the error code. The enum @c #MinErr
 *           contains codes for the most common errors. It is convenient enough
 *           to use special defines for handling return codes. Below you can find
 *           two defines which are widely used in the library.
 */


typedef enum MinResult {
  MR_NO                 =  1,
  MR_YES                =  0,
  MR_SUCCESS            =  0,
  MR_CONTRACT_VIOLATION = -1,
  MR_ENV_ERROR          = -2,
  MR_INTERNAL_ERROR     = -3,
  MR_STRIPPED_VERSION   = -4,
  MR_NOT_IMPLEMENTED    = -5,

  MR_MRR = MR_SUCCESS
} MinResult;


///// TODO: Why inline instead of MUSTINLINE ?
static inline MinResult MinResBreakHere(MinResult pass) {
  return pass;
}


#define MR_BACKED_CALL_EX(call, converter, backup)  \
  if (1) {  \
    auto MIN_BACKED_CALL_res = (call);  \
    MinResult MIN_BACKED_CALL_res_conv = converter(MIN_BACKED_CALL_res);  \
    if (MR_MRR > MIN_BACKED_CALL_res_conv && \
        MinResBreakHere(MIN_BACKED_CALL_res_conv))  \
      backup; \
  } else (void)0


/// FIXME: These don't work, because they have different num of args,
///        and 'err' is not used in BACKED_CALL
#define MR_MinResTrans_default(res) (res)

#define MR_MinResTrans_int(res, err) ((res) < 0 ? (err) : MR_MRR)

#define MR_MinResTrans_ptr(res, err) ((res) ? MR_MRR : (err))

#define MR_MinResTrans_MinTyp(res, err) ((res) == TYP_INVALID ? (err) : MR_MRR)


#define MR_BACKED_CALL(call, backup)  \
  MR_BACKED_CALL_EX(call, MR_MinResTrans_default, backup)


#define MR_PROPAGATE_ERROR_EX(call, converter)  \
  MR_BACKED_CALL_EX(call, converter, return MIN_BACKED_CALL_res_conv)


#define MR_PROPAGATE_ERROR(call)  \
  MR_BACKED_CALL(call, return MIN_BACKED_CALL_res)


#define MR_SHOULD_WORK_EX(call, converter)  \
  MR_BACKED_CALL_EX(call, converter, return MR_INTERNAL_ERROR)


#define MR_SHOULD_WORK(call)  \
  MR_BACKED_CALL(call, return MR_INTERNAL_ERROR)


/**
 * @brief   Specifies basic error codes.
 * @details The enum specifies a list of basic error codes that is such ones
 *          which can be returned by any function in the library.
 * @ingroup MinBase_MinErr
 */
typedef enum
{
  NO_ERRORS       =  0,   ///< No error has occurred. It indicates that the
                          ///  function completed successfully.
  BAD_ARGS        = -1,   ///< This error indicates that one or more arguments
                          ///  passed to the function are not correct.
  NO_MEMORY       = -2,   ///< Not enough memory is available. This can result
                          ///  from low memory conditions.
  NOT_IMPLEMENTED = -3,   ///< This error indicates that the requested function
                          ///  is not implemented.
  INTERNAL_ERROR  = -4,   ///< An internal error has occurred. This error
                          ///  indicates that something went wrong.
  FILE_ERROR      = -5,   ///< An error occurred while working with files. The
                          ///  most likely cause is a full disk or a corrupted
                          ///  file to be open.
  NOT_SUPPORTED   = -6,   ///<

  BAD_STATE       = -7,   ///<

  NO_SENSE        = -8    ///<
} MinErr;

inline int BreakHere( int pass )
{
  return pass; // << set breakpoint here to catch PROPAGATE_ERROR()
}

#define BACKED_CALL(call, backup)                                       \
  do {                                                                  \
    int _minerr_BACKED_UP_CALL_res = (call);                            \
    if (_minerr_BACKED_UP_CALL_res < 0) {                               \
      backup;                                                           \
    }                                                                   \
  } while (0)

/**
 * @brief   If function failed then propagate the error code.
 * @details This define macro describes a code that helps to propagate an
 *          exception if an error occurs.
 * @ingroup MinBase_MinErr
 */
/*#ifdef WITH_MIN_LOG
#define PROPAGATE_ERROR(call)                                           \
  do {                                                                  \
    int _minerr_PROPAGATE_ERROR_res = (call);                           \
    if (_minerr_PROPAGATE_ERROR_res < 0) {                              \
    MinLog("Trace: %d {%s} %s:%d\n", _minerr_PROPAGATE_ERROR_res,       \
           MIN_FUNCNAME_MACRO, __FILE__, __LINE__);                     \
      return BreakHere(_minerr_PROPAGATE_ERROR_res);                    \
    }                                                                   \
  } while (0)
#else // WITH_MIN_LOG*/
#define PROPAGATE_ERROR(call)                                           \
  do {                                                                  \
    int _minerr_PROPAGATE_ERROR_res = (call);                           \
    if (_minerr_PROPAGATE_ERROR_res < 0) {                              \
      return BreakHere(_minerr_PROPAGATE_ERROR_res);                    \
    }                                                                   \
  } while (0)
//#endif // WITH_MIN_LOG

#define BACKED_PROPAGATE_ERROR(call, backup)                            \
  do {                                                                  \
    int _minerr_BACKED_PROPAGATE_ERROR_res = (call);                    \
    if (_minerr_BACKED_PROPAGATE_ERROR_res < 0) {                       \
      backup;                                                           \
      return BreakHere(_minerr_BACKED_PROPAGATE_ERROR_res);             \
    }                                                                   \
  } while (0)

/**
 * @brief   If function returned something except NO_ERRORS then propagate
 *          error.
 * @details This define macro describes a code that helps to check if
 *          function returned NO_ERRORS. If the error occured, it is
 *          propagated, if the function returned positive value, BAD_ARGS
 *          is returned.
 * @ingroup MinBase_MinErr
 */
#define NEED_NO_ERRORS(call)                                            \
  do {                                                                  \
    int _minerr_NEED_NO_ERRORS_res = (call);                            \
    if (_minerr_NEED_NO_ERRORS_res < 0) {                               \
      return BreakHere(_minerr_NEED_NO_ERRORS_res);                     \
    }                                                                   \
    else if (_minerr_NEED_NO_ERRORS_res > 0) {                          \
      return BreakHere(BAD_ARGS);                                       \
    }                                                                   \
  } while (0)

/**
 * @brief   If function failed then propagate INTERNAL_ERROR.
 * @details This define macro describes a code that propagates INTERNAL_ERROR
 *          exception if an error occurs.
 * @ingroup MinBase_MinErr
 */
/*#ifdef WITH_MIN_LOG
#define SHOULD_WORK(call)                                               \
  do {                                                                  \
    int _minerr_SHOULD_WORK_res = (call);                               \
    if (_minerr_SHOULD_WORK_res < 0) {                                  \
    MinLog("Trace: %d {%s} %s:%d\n", _minerr_SHOULD_WORK_res,           \
           MIN_FUNCNAME_MACRO, __FILE__, __LINE__);                     \
      return BreakHere(INTERNAL_ERROR);                                 \
    }                                                                   \
  } while (0)
#else // WITH_MIN_LOG*/
#define SHOULD_WORK(call)                                               \
  do {                                                                  \
    int _minerr_SHOULD_WORK_res = (call);                               \
    if (_minerr_SHOULD_WORK_res < 0) {                                  \
      return BreakHere(INTERNAL_ERROR);                                 \
    }                                                                   \
  } while (0)
//#endif // WITH_MIN_LOG

/*#ifdef WITH_MIN_LOG
#define IGNORE_ERRORS(call)                                       \
  do {                                                            \
    int _minerr_IGNORE_ERRORS_res = (call);                       \
    if (_minerr_IGNORE_ERRORS_res < 0) {                          \
      MinLog("Trace: %d {%s} %s:%d\n", _minerr_IGNORE_ERRORS_res, \
             MIN_FUNCNAME_MACRO, __FILE__, __LINE__);             \
    }                                                             \
  } while (0)
#else*/
#define IGNORE_ERRORS(call) (call)
//#endif // WITH_MIN_LOG

#endif // #ifndef MINSBASE_MINRESULT_H_INCLUDED
