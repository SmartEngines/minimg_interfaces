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

//#pragma once
#ifndef MINIMGIO_SRC_AVFOUNDATIONDEVICE_H_INCLUDED
#define MINIMGIO_SRC_AVFOUNDATIONDEVICE_H_INCLUDED

#include "stream.h"

#include <string>

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@class SampleBufferDelegateWrapper;

class AVFoundationDevice : public Device
{
public:
  static AVFoundationDevice * Instance(const char *pDeviceName);
  virtual ~AVFoundationDevice();

private:
  AVFoundationDevice(const char *pDeviceName);
  AVFoundationDevice(const AVFoundationDevice &);
  AVFoundationDevice &operator=(const AVFoundationDevice &);

public:
  int Take();
  int Release();
#ifndef STRICT_DATA_CONTAINMENT
  int PushImage(const MinImg *pImg);
#endif // #ifndef STRICT_DATA_CONTAINMENT
  int QueryImage(const MinImg *pImg);
  int QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps);
  MinResult GetProperty(const char *pKey, char *pValue, int size);
  MinResult SetProperty(const char *pKey, const char *pValue);

private:
  int counter;
  std::string deviceName;

  SampleBufferDelegateWrapper *pSampleBufferDelegateWrapper;
  AVCaptureSession *pCaptureSession;
  AVCaptureDeviceInput *pCaptureDeviceInput;
  AVCaptureVideoDataOutput *pCaptureVideoDataOutput;

private:
  int setupDevice();
  int clearDevice();

  AVCaptureDevice * backCamera() const;
};

@interface SampleBufferDelegateWrapper : NSObject
                <AVCaptureVideoDataOutputSampleBufferDelegate>
{
@public
  CMSampleBufferRef currentBuffer;
}

@end

#endif // #ifndef MINIMGIO_SRC_AVFOUNDATIONDEVICE_H_INCLUDED
