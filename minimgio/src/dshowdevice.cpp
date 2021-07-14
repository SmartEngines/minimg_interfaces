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

#pragma warning (disable : 4995)

#include "dshowutils.h"
#include "dshowdevice.h"
#include "utils.h"

#include <cmath>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <sstream>

#include <minbase/minresult.h>
#include <minbase/crossplat.h>
#include <minimgapi/minimgapi.h>
#include <minimgio/device.h>

static void FreeMediaType(AM_MEDIA_TYPE &mt)
{
  if (mt.cbFormat != 0)
  {
    CoTaskMemFree((PVOID)mt.pbFormat);
    mt.cbFormat = 0;
    mt.pbFormat = NULL;
  }
  if (mt.pUnk != NULL)
  {
    mt.pUnk->Release();
    mt.pUnk = NULL;
  }
}

SampleGrabberCallback::SampleGrabberCallback() : buffer(0), bufferLength(0)
{
  InitializeCriticalSection(&critSection);
}

SampleGrabberCallback::~SampleGrabberCallback()
{
  DeleteCriticalSection(&critSection);
  delete[] buffer;
  buffer = 0;
}

HRESULT STDMETHODCALLTYPE SampleGrabberCallback::QueryInterface(REFIID riid, void **ppvObject)
{
  *ppvObject = static_cast<ISampleGrabberCB*>(this);
  return S_OK;
}

ULONG STDMETHODCALLTYPE SampleGrabberCallback::AddRef(void)
{
  return 1;
}

ULONG STDMETHODCALLTYPE SampleGrabberCallback::Release(void)
{
  return 2;
}

STDMETHODIMP SampleGrabberCallback::SampleCB(double sampleTime, IMediaSample *pSample)
{
  return E_NOTIMPL;
}

STDMETHODIMP SampleGrabberCallback::BufferCB(double sampleTime, BYTE *data, long dataLength)
{
  EnterCriticalSection(&critSection);

  if (dataLength > bufferLength)
  {
    delete[] buffer;
    buffer = new unsigned char[dataLength];
    bufferLength = dataLength;
  }

  memcpy(buffer, data, dataLength);

  LeaveCriticalSection(&critSection);

  return S_OK;
}

bool SampleGrabberCallback::ResizeBuffer(int bufferLength)
{
  delete[] buffer;
  buffer = new unsigned char[bufferLength];
  memset(buffer, 0, bufferLength);
  return true;
}

bool SampleGrabberCallback::CopyImage(const MinImg *pImg)
{
  EnterCriticalSection(&critSection);

  if (pImg == 0 || pImg->p_zero_line == 0)
  {
    LeaveCriticalSection(&critSection);
    return false;
  }

  int bytes_per_line = GetMinImageBytesPerLine(pImg);

  if (pImg->height * bytes_per_line > bufferLength)
  {
    LeaveCriticalSection(&critSection);
    return false;
  }

  int bufferStride = bufferLength / pImg->height;

  for (int y = 0; y < pImg->height; y++)
  {
    uint8_t *pImgLine = pImg->p_zero_line + y * pImg->stride;
    uint8_t *pBufLine = buffer + (pImg->height - y - 1) * bufferStride;

    for (int x = 0, xx = 0; x < pImg->width; x++, xx += 3)
    {
      pImgLine[xx + 2] = pBufLine[xx + 0];
      pImgLine[xx + 1] = pBufLine[xx + 1];
      pImgLine[xx + 0] = pBufLine[xx + 2];
    }
  }

  LeaveCriticalSection(&critSection);
  return true;
}

//////////////////////////////////////////////////////////////////////////

DShowDevice * DShowDevice::Instance(const char *pDeviceName)
{
  float frameRate = 0;
  int width = 0, height = 0;
  char cameraName[1024] = {0};

  std::string deviceNameList;
  HRESULT hr = GetCaptureDeviceList(deviceNameList);

  std::istringstream iss(pDeviceName);
  std::string deviceName; std::getline(iss, deviceName);

  if (FAILED(hr) || deviceName.empty())
    return NULL;

  if (deviceNameList.find(deviceName) == std::string::npos)
    return NULL;

  static std::map<std::string, DShowDevice *> devices;
  DShowDevice *pDevice = devices[deviceName];

  if (pDevice == NULL)
  {
    pDevice = new DShowDevice(deviceName.c_str());
    devices[deviceName] = pDevice;
  }

  if (pDevice->counter == 0)
  {
    if (iss.eof() == false)
    {
      std::string frameSize;
      std::getline(iss, frameSize);
      sscanf(frameSize.c_str(), "%dx%d", &pDevice->width, &pDevice->height);
    }
    if (iss.eof() == false)
    {
      std::string frameRate;
      std::getline(iss, frameRate);
      pDevice->frameRate = atof(frameRate.c_str());
    }
  }

  return pDevice;
}

DShowDevice::DShowDevice(const char *pDeviceName) :
counter(0), hr(S_OK), devName(pDeviceName), width(0), height(0), frameRate(0)
{
  pGraphBuilder  = NULL;
  pMediaControl  = NULL;
  pMediaFilter   = NULL;
  pMediaEvent    = NULL;

  pSourceFilter   = NULL;
  pGrabberFilter  = NULL;
  pRenderFilter   = NULL;

  pGrabber  = NULL;
  pOutPin   = NULL;
}

DShowDevice::~DShowDevice()
{
  pGraphBuilder  = NULL;
  pMediaControl  = NULL;
  pMediaEvent    = NULL;

  pSourceFilter   = NULL;
  pGrabberFilter  = NULL;
  pRenderFilter   = NULL;

  pGrabber = NULL;
  pOutPin   = NULL;
}

MinResult DShowDevice::Take()
{
  if (counter == 0)
  {
    MR_PROPAGATE_ERROR(createGraphBuilder());
    MR_PROPAGATE_ERROR(setupCaptureFilter());
    MR_PROPAGATE_ERROR(setupGrabberFilter());

    if (width > 0 && height > 0)
      SetFormatCam(pOutPin, width, height, static_cast<int>(frameRate));

    MR_PROPAGATE_ERROR(connectFilterPins());
  }

  counter++;
  return MR_SUCCESS;
}

MinResult DShowDevice::Release()
{
  counter--;

  if (counter == 0)
  {
    if (pGrabber) pGrabber->SetCallback(0, 1);

    if (pMediaControl) pMediaControl->Stop();
    if (pOutPin) hr = pOutPin->Disconnect();

    if (pGraphBuilder) pGraphBuilder.Release();
    if (pMediaControl) pMediaControl.Release();
    if (pMediaFilter) pMediaFilter.Release();
    if (pMediaEvent) pMediaEvent.Release();

    if (pSourceFilter) pSourceFilter.Release();
    if (pGrabberFilter) pGrabberFilter.Release();
    if (pRenderFilter) pRenderFilter.Release();

    if (pGrabber) pGrabber.Release();
    if (pOutPin) pOutPin.Release();
  }

  return MR_SUCCESS;
}

#ifndef STRICT_DATA_CONTAINMENT
MinResult DShowDevice::PushImage(const MinImg *pImg)
{
  return MR_INTERNAL_ERROR;
}
#endif // #ifndef STRICT_DATA_CONTAINMENT

MinResult DShowDevice::QueryImage(const MinImg *pImg)
{
  if (grabberCB.CopyImage(pImg) == false)
    return MR_INTERNAL_ERROR;
  return MR_SUCCESS;
}

MinResult DShowDevice::QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps)
{
  if (counter == 0 || pGrabber == 0)
    return MR_CONTRACT_VIOLATION;

  if (pImg == NULL)
    return MR_CONTRACT_VIOLATION;

  AM_MEDIA_TYPE mediaType;
  hr = pGrabber->GetConnectedMediaType(&mediaType);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  if (mediaType.formattype != FORMAT_VideoInfo || mediaType.cbFormat < sizeof(VIDEOINFOHEADER))
  {
    FreeMediaType(mediaType);
    return MR_INTERNAL_ERROR;
  }

  VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)mediaType.pbFormat;
  if (pVih == 0)
  {
    FreeMediaType(mediaType);
    return MR_INTERNAL_ERROR;
  }

  pImg->width = pVih->bmiHeader.biWidth;
  pImg->height = std::abs(pVih->bmiHeader.biHeight);
  pImg->channels = std::max<int>(1, pVih->bmiHeader.biBitCount / 8);
  pImg->scalar_type = pVih->bmiHeader.biBitCount > 1 ? TYP_UINT8 : TYP_UINT1;

  FreeMediaType(mediaType);
  return MR_SUCCESS;
}

MinResult DShowDevice::GetProperty(const char *pKey, char *pValue, int size)
{
  if (pKey == NULL || pValue == NULL)
    return MR_CONTRACT_VIOLATION;

  try
  {
    HRESULT hr = S_OK;

    if (strcmp(pKey, SP_GAIN) == 0)
    {
      int gain = 0;
      hr = GetGainCam(pSourceFilter, &gain, NULL);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, "%d", gain);
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_GAINAUTO) == 0)
    {
      bool isGainAuto = false;
      hr = GetGainCam(pSourceFilter, NULL, &isGainAuto);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, isGainAuto ? "true" : "false");
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_EXPOSURE) == 0)
    {
      double exposure = 0;
      hr = GetExposureCam(pSourceFilter, &exposure, NULL);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, "%f", exposure);
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_EXPOSUREAUTO) == 0)
    {
      bool isExposureAuto = false;
      hr = GetExposureCam(pSourceFilter, NULL, &isExposureAuto);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, isExposureAuto ? "true" : "false");
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_FOCUS) == 0)
    {
      double focus = 0;
      hr = GetFocusCam(pSourceFilter, &focus, NULL);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, "%f", focus);
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_FOCUSAUTO) == 0)
    {
      bool isFocusAuto = false;
      hr = GetFocusCam(pSourceFilter, NULL, &isFocusAuto );
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, isFocusAuto ? "true" : "false");
      return MR_SUCCESS;
    }

    int width = 0, height = 0, frameRate = 0;
    hr = GetFormatCam(pOutPin, &width, &height, &frameRate);
    if (FAILED(hr)) throw hr;

    if (strcmp(pKey, SP_FRAMERATE) == 0)
      snprintf(pValue, size, "%d", frameRate);
    else if (strcmp(pKey, SP_FRAMESIZE) == 0)
      snprintf(pValue, size, "%dx%d", width, height);
    else
      return MR_INTERNAL_ERROR;

    return MR_SUCCESS;
  }
  catch (HRESULT hr)
  {
    if (hr == VFW_E_NO_INTERFACE)
      return MR_NOT_IMPLEMENTED;
    else
      return MR_INTERNAL_ERROR;
  }
}

MinResult DShowDevice::SetProperty(const char *pKey, const char *pValue)
{
  if (pKey == NULL || pValue == NULL)
    return MR_CONTRACT_VIOLATION;

  try
  {
    HRESULT hr = S_OK;

    if (strcmp(pKey, SP_GAIN) == 0)
    {
      int gain = atoi(pValue);
      hr = SetGainCam(pSourceFilter, gain, false);
      if (FAILED(hr)) throw hr;
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_GAINAUTO) == 0)
    {
      bool isGainAuto = atob(pValue);
      hr = SetGainCam(pSourceFilter, 0, isGainAuto);
      if (FAILED(hr)) throw hr;
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_EXPOSURE) == 0)
    {
      double exposure = atof(pValue);
      hr = SetExposureCam(pSourceFilter, exposure, false);
      if (FAILED(hr))
        throw hr;
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_EXPOSUREAUTO) == 0)
    {
      bool isExposureAuto = atob(pValue);
      hr = SetExposureCam(pSourceFilter, 1.0, isExposureAuto);
      if (FAILED(hr))
        throw hr;
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_FOCUS) == 0)
    {
      double focus = atof(pValue);
      hr = SetFocusCam(pSourceFilter, focus, false);
      if (FAILED(hr))
        throw hr;
      return MR_SUCCESS;
    }

    if (strcmp(pKey, SP_FOCUSAUTO) == 0)
    {
      bool isFocusAuto = atob(pValue);
      hr = SetFocusCam(pSourceFilter, 1.0, isFocusAuto);
      if (FAILED(hr))
        throw hr;
      return MR_SUCCESS;
    }

    if (counter > 1)
      return MR_CONTRACT_VIOLATION;

    int width = 0, height = 0, frameRate = 0;
    hr = GetFormatCam(pOutPin, &width, &height, &frameRate);
    if (FAILED(hr))
      throw hr;

    if (strcmp(pKey, SP_FRAMERATE) == 0)
    {
      frameRate = atoi(pValue);
    }
    else if (strcmp(pKey, SP_FRAMESIZE) == 0)
    {
      sscanf(pValue, "%dx%d", &width, &height);
    }
    else if (strcmp(pKey, SP_VIDEOFILEPATH) == 0)
    {
      videoPath = pValue;
    }
    else
    {
      return MR_INTERNAL_ERROR;
    }

    MR_PROPAGATE_ERROR(Release());
    MR_PROPAGATE_ERROR(createGraphBuilder());
    MR_PROPAGATE_ERROR(setupGrabberFilter());
    MR_PROPAGATE_ERROR(setupCaptureFilter());

    hr = SetFormatCam(pOutPin, width, height, frameRate);
    if (FAILED(hr))
      throw hr;

    MR_PROPAGATE_ERROR(connectFilterPins());
    counter++;

    return MR_SUCCESS;
  }
  catch (HRESULT hr)
  {
    if (hr == VFW_E_NO_INTERFACE)
      return MR_NOT_IMPLEMENTED;
    else
      return MR_INTERNAL_ERROR;
  }}

MinResult DShowDevice::createGraphBuilder()
{
  HRESULT hr = pCaptureBuilder.CreateInstance(CLSID_CaptureGraphBuilder2);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGraphBuilder.CreateInstance(CLSID_FilterGraph);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pCaptureBuilder->SetFiltergraph(pGraphBuilder.GetInterfacePtr());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&pMediaControl.GetInterfacePtr());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGraphBuilder->QueryInterface(IID_IMediaEventEx, (void **)&pMediaEvent.GetInterfacePtr());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGraphBuilder->QueryInterface(IID_IMediaFilter, (void **)&pMediaFilter.GetInterfacePtr());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  return MR_SUCCESS;
}

MinResult DShowDevice::setupCaptureFilter()
{
  if (pGraphBuilder == NULL)
    return MR_INTERNAL_ERROR;

  HRESULT hr = FindCaptureDevice(pSourceFilter.GetInterfacePtr(), devName.c_str());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = GetCapturePin(pSourceFilter, pOutPin.GetInterfacePtr());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;


  hr = pGraphBuilder->AddFilter(pSourceFilter, L"Source");
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  return MR_SUCCESS;
}

MinResult DShowDevice::setupGrabberFilter()
{
  if (pGraphBuilder == NULL)
    return MR_INTERNAL_ERROR;

  hr = pGrabberFilter.CreateInstance(CLSID_SampleGrabber);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGraphBuilder->AddFilter(pGrabberFilter.GetInterfacePtr(), L"Grabber");
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pRenderFilter.CreateInstance(CLSID_NullRenderer);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGraphBuilder->AddFilter(pRenderFilter.GetInterfacePtr(), L"Render");
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGrabberFilter.QueryInterface(IID_ISampleGrabber, (void **)&pGrabber.GetInterfacePtr());
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  hr = pGrabber->SetOneShot(FALSE);
  hr = pGrabber->SetBufferSamples(FALSE);
  hr = pGrabber->SetCallback(&grabberCB, 1);

  AM_MEDIA_TYPE mt;
  ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));
  mt.majortype  = MEDIATYPE_Video;
  mt.subtype    = MEDIASUBTYPE_RGB24;
  mt.formattype = FORMAT_VideoInfo;

  hr = pGrabber->SetMediaType(&mt);

  return MR_SUCCESS;
}

MinResult DShowDevice::connectFilterPins()
{
  HRESULT hr = pCaptureBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,
    &MEDIATYPE_Video, pSourceFilter, pGrabberFilter, pRenderFilter);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

//   if (videoPath.length() > 0)
//   {
//     IBaseFilter *pMux = NULL;
//     hr = pCaptureBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi,
//       _bstr_t(videoPath.c_str()), &pMux, NULL);
//
//     hr = pCaptureBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,
//       &MEDIATYPE_Video, pSourceFilter, 0, pMux);
//
//     pMux->Release();
//   }

  hr = pMediaFilter->SetSyncSource(NULL);

  hr = pMediaControl->Run();
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

//   long evCode = 0;
//   hr = pMediaEvent->WaitForCompletion(1000, &evCode);

  return MR_SUCCESS;
}
