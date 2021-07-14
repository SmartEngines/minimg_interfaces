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
#ifndef MINIMGIO_SRC_DSHOWUTILS_H_INCLUDED
#define MINIMGIO_SRC_DSHOWUTILS_H_INCLUDED

#include <dshow.h>
#include <dvdmedia.h>

#include <string>

HRESULT GetCaptureDeviceList(std::string &deviceNameList);
HRESULT FindCaptureDevice(IBaseFilter *&pSourceFilter, const char *pDevName);

HRESULT GetCapturePin(IBaseFilter *pFilter, IPin *&pPin, int nPin = 0);
#ifndef STRICT_DATA_CONTAINMENT
HRESULT GetOutPin(IBaseFilter *pFilter, IPin *&pPin, int nPin = 0);
#endif // #ifndef STRICT_DATA_CONTAINMENT
HRESULT GetInPin(IBaseFilter *pFilter, IPin *&pPin, int nPin = 0);

HRESULT GetGainCam(IBaseFilter *pSourceFilter, int *pValue, bool *pIsAuto);
HRESULT SetGainCam(IBaseFilter *pSourceFilter, int value, bool isAuto);

HRESULT GetExposureCam(IBaseFilter *pSourceFilter, double *pValue, bool *pIsAuto);
HRESULT SetExposureCam(IBaseFilter *pSourceFilter, double value, bool isAuto);

HRESULT GetFocusCam(IBaseFilter *pSourceFilter, double *pValue, bool *pIsAuto);
HRESULT SetFocusCam(IBaseFilter *pSourceFilter, double value, bool isAuto);

HRESULT GetFormatCam(IPin *pOutPin, int *pWidth, int *pHeight, int *pFps);
HRESULT SetFormatCam(IPin *pOutPin, int width, int height, int fps);

#endif // #ifndef MINIMGIO_SRC_DSHOWUTILS_H_INCLUDED
