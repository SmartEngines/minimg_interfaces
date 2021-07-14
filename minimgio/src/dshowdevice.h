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
#ifndef MINIMGIO_SRC_DSHOWDEVICE_H_INCLUDE
#define MINIMGIO_SRC_DSHOWDEVICE_H_INCLUDE

#include "stream.h"

#include <map>
#include <string>

#include <dshow.h>
#include <comdef.h>
#include <strmif.h>

struct __declspec(uuid("6B652FFF-11FE-4fce-92AD-0266B5D7C78F")) ISampleGrabber;
DEFINE_GUID(IID_ISampleGrabber,0x6b652fff,0x11fe,0x4fce,0x92,0xad,0x02,0x66,0xb5,0xd7,0xc7,0x8f);
DEFINE_GUID(CLSID_NullRenderer,0xc1f400a4,0x3f08,0x11d3,0x9f,0x0b,0x00,0x60,0x08,0x03,0x9e,0x37);
DEFINE_GUID(CLSID_SampleGrabber,0xc1f400a0,0x3f08,0x11d3,0x9f,0x0b,0x00,0x60,0x08,0x03,0x9e,0x37);

_COM_SMARTPTR_TYPEDEF(ICaptureGraphBuilder2, __uuidof(ICaptureGraphBuilder2));
_COM_SMARTPTR_TYPEDEF(IGraphBuilder, __uuidof(IFilterGraph));
_COM_SMARTPTR_TYPEDEF(IMediaControl, __uuidof(IMediaControl));
_COM_SMARTPTR_TYPEDEF(IMediaFilter, __uuidof(IMediaFilter));
_COM_SMARTPTR_TYPEDEF(IMediaEventEx, __uuidof(IMediaEventEx));
_COM_SMARTPTR_TYPEDEF(IBaseFilter, __uuidof(IBaseFilter));
_COM_SMARTPTR_TYPEDEF(IPin, __uuidof(IPin));
_COM_SMARTPTR_TYPEDEF(ISampleGrabber, __uuidof(ISampleGrabber));

interface ISampleGrabberCB : public IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample) = 0;
  virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, LONG BufferLen) = 0;
};

interface ISampleGrabber : public IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE SetOneShot(
    BOOL OneShot) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetMediaType(
    const AM_MEDIA_TYPE *pType) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(
    AM_MEDIA_TYPE *pType) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(
    BOOL BufferThem) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(
    LONG *pBufferSize,
    LONG *pBuffer) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(
    IMediaSample **ppSample) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetCallback(
    ISampleGrabberCB *pCallback,
    LONG WhichMethodToCallback) = 0;
};

class SampleGrabberCallback : public ISampleGrabberCB
{
public:
  SampleGrabberCallback();
  virtual ~SampleGrabberCallback();

public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
  virtual ULONG STDMETHODCALLTYPE AddRef();
  virtual ULONG STDMETHODCALLTYPE Release();

public:
  STDMETHODIMP SampleCB(double sampleTime, IMediaSample *pSample);
  STDMETHODIMP BufferCB(double sampleTime, BYTE *data, LONG dataLength);

public:
    bool ResizeBuffer(int bufferLength);
    bool CopyImage(const MinImg *pImg);

private:
  long bufferLength;
  unsigned char *buffer;
  CRITICAL_SECTION critSection;
};

class DShowDevice : public Device
{
public:
  static DShowDevice * Instance(const char *pDeviceName);
  virtual ~DShowDevice();

private:
  DShowDevice(const char *pDeviceName);
  DShowDevice(const DShowDevice &);
  DShowDevice &operator=(const DShowDevice &);

public:
  MinResult Take() override;
  MinResult Release() override;
  MinResult PushImage(const MinImg *pImg) override;
  MinResult QueryImage(const MinImg *pImg) override;
  MinResult QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps) override;
  MinResult GetProperty(const char *pKey, char *pValue, int size) override;
  MinResult SetProperty(const char *pKey, const char *pValue) override;

private:
  MinResult createGraphBuilder();
  MinResult setupGrabberFilter();
  MinResult setupCaptureFilter();
  MinResult connectFilterPins();

private:
  HRESULT          hr; 
  int              counter;
  std::string      devName;
  std::string      videoPath;

  int width;
  int height;
  double frameRate;

  ICaptureGraphBuilder2Ptr pCaptureBuilder;
  IGraphBuilderPtr         pGraphBuilder;
  IMediaControlPtr         pMediaControl;
  IMediaFilterPtr          pMediaFilter;
  IMediaEventExPtr         pMediaEvent;

  IBaseFilterPtr    pSourceFilter;
  IBaseFilterPtr    pGrabberFilter;
  IBaseFilterPtr    pRenderFilter;
  ISampleGrabberPtr pGrabber;
  IPinPtr           pOutPin;

  SampleGrabberCallback grabberCB;
};

#endif // #ifndef MINIMGIO_SRC_DSHOWDEVICE_H_INCLUDE
