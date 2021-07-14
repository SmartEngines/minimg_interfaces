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

#include "avfoundationdevice.h"
#include "avfoundationutils.h"
#include "utils.h"

#include <stream.h>
#include <minbase/minresult.h>
#include <minimgio/device.h>

#include <cstring>

#define SAVE_CHECK_IMAGES 0

#if SAVE_CHECK_IMAGES
# import <ImageIO/ImageIO.h>
# import <MobileCoreServices/MobileCoreServices.h>
#endif

AVFoundationDevice * AVFoundationDevice::Instance(const char *pDeviceName)
{
  std::string deviceNameList("iPhoneBackCamera");
  if(deviceNameList.find(pDeviceName) == std::string::npos)
  {
    return NULL;
  }

  static AVFoundationDevice *pDevice;
  if(pDevice == NULL)
  {
    pDevice = new AVFoundationDevice(pDeviceName);
  }
  return pDevice;
}

AVFoundationDevice::AVFoundationDevice(const char *pDeviceName)
: counter(0), deviceName(pDeviceName)
{

}

AVFoundationDevice::~AVFoundationDevice()
{
  clearDevice();
}

int AVFoundationDevice::Take()
{
  if(counter++ == 0)
  {
    PROPAGATE_ERROR(setupDevice());
  }
  return NO_ERRORS;
}

int AVFoundationDevice::Release()
{
  if(--counter == 0)
  {
    PROPAGATE_ERROR(clearDevice());
  }
  return NO_ERRORS;
}

int AVFoundationDevice::QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps)
{
  if(counter == 0)
    return BAD_STATE;
  if(pImg == NULL)
    return BAD_ARGS;

  //if(CMSampleBufferIsValid(pSampleBufferDelegateWrapper->currentBuffer) == false)
   // return BAD_STATE;

  CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(
    pSampleBufferDelegateWrapper->currentBuffer);
  CVPixelBufferLockBaseAddress(imageBuffer, 0);

  int width = 640;//CVPixelBufferGetWidth(imageBuffer);
  int height = 480;//CVPixelBufferGetHeight(imageBuffer);

  if(width == 0 || height == 0)
    return BAD_STATE;

  pImg->width = width, pImg->height = height;
  pImg->channels = 3;
  pImg->channelDepth = 1;
  pImg->format = FMT_UINT;

  CVPixelBufferUnlockBaseAddress(imageBuffer, 0);

  return NO_ERRORS;
}

int AVFoundationDevice::QueryImage(const MinImg *pImg)
{
  if (counter == 0)
    return BAD_STATE;
  if (pImg == NULL || pImg->pScan0 == NULL)
    return BAD_ARGS;
  if (pImg->format != FMT_UINT || pImg->channelDepth != 1)
    return BAD_ARGS;

  __block int success = NO_ERRORS;
  dispatch_sync(pCaptureVideoDataOutput.sampleBufferCallbackQueue, ^(void) {
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(pSampleBufferDelegateWrapper->currentBuffer);
    CVPixelBufferLockBaseAddress(imageBuffer, 0);

    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);

    if(pImg->width != width || pImg->height != height || pImg->channels != 3)
    {
      CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
      success = BAD_ARGS;
      return;
    }

    uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);

    for (size_t y = 0; y < height; ++y)
    {
      for(size_t x = 0; x < width; ++x)
      {
        uint8_t *src = baseAddress + y * bytesPerRow + x * 4;
        uint8_t *dest = pImg->pScan0 + y * pImg->stride + x * pImg->channels;

        for(int i = 0; i < pImg->channels; ++i)
          *(dest + i) = *(src + i);
      }
    }

  #if SAVE_CHECK_IMAGES
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(baseAddress, width, height, 8,
    bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedLast);

    uint8_t *basePtr = (uint8_t *)CGBitmapContextGetData(context);
    CGImageRef img = CGBitmapContextCreateImage(context);


    size_t bpr = CGImageGetBytesPerRow(img);
    size_t bpp = CGImageGetBitsPerPixel(img);
    size_t bpc = CGImageGetBitsPerComponent(img);
    size_t bytes_per_pixel = bpp / bpc;

    CGBitmapInfo info = CGImageGetBitmapInfo(img);


    NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    path = [path stringByAppendingPathComponent:@"img.png"];

    CFURLRef url = (CFURLRef)[NSURL fileURLWithPath:path];
    CGImageDestinationRef dest = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, NULL);
    CGImageDestinationAddImage(dest, img, nil);

    CGImageDestinationFinalize(dest);
    CFRelease(dest);
    CGImageRelease(img);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
  #endif

    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);

  }); // dispatch_sync

  return success;
}

int AVFoundationDevice::setupDevice()
{
  ScopedAutoreleasePool pool;

  pCaptureSession = [[AVCaptureSession alloc] init];
  //pCaptureSession.sessionPreset = AVCaptureSessionPreset640x480;

  pCaptureDeviceInput = [[AVCaptureDeviceInput alloc] initWithDevice:backCamera() error:nil];
  [pCaptureSession addInput:pCaptureDeviceInput];

  pCaptureVideoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
  pCaptureVideoDataOutput.videoSettings =
    [NSDictionary dictionaryWithObject: [NSNumber numberWithUnsignedInt: kCVPixelFormatType_32BGRA]
                                         forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
 // pCaptureVideoDataOutput.alwaysDiscardsLateVideoFrames = YES;
  dispatch_queue_t outputQueue = dispatch_queue_create("OutputQueue", NULL);
  pSampleBufferDelegateWrapper = [[SampleBufferDelegateWrapper alloc] init];
  [pCaptureVideoDataOutput setSampleBufferDelegate:pSampleBufferDelegateWrapper
                                             queue:outputQueue];
  dispatch_release(outputQueue);
  [pCaptureSession addOutput:pCaptureVideoDataOutput];

  pCaptureSession.sessionPreset = AVCaptureSessionPreset640x480;
  [pCaptureSession startRunning];

  return NO_ERRORS;
}

int AVFoundationDevice::clearDevice()
{
  ScopedAutoreleasePool pool;

  [pCaptureSession stopRunning];
  [pCaptureSession release];
  [pCaptureDeviceInput release];
  [pCaptureVideoDataOutput release];
  [pSampleBufferDelegateWrapper release];

  return NO_ERRORS;
}

AVCaptureDevice * AVFoundationDevice::backCamera() const
{
  NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
  for (AVCaptureDevice *device in devices)
    if ([device position] == AVCaptureDevicePositionBack)
      return device;
  return nil;
}

#ifndef STRICT_DATA_CONTAINMENT
MinResult AVFoundationDevice::PushImage(
    const MinImg *pImg) {
  return NO_SENSE;
}
#endif // #ifndef STRICT_DATA_CONTAINMENT

MinResult AVFoundationDevice::GetProperty(
    const char *pKey,
    char *pValue,
    int size) {
  if (pKey == NULL || pValue == NULL)
    return BAD_ARGS;

  if(strcmp(pKey, SP_VIDEOPREVIEW) == 0)
  {
    if(counter == 0)
      return BAD_STATE;

    uint8_t *pPreviewLayer = 0; size_t sz = 0;
    PROPAGATE_ERROR(ExtractMemoryLocation(pValue, &pPreviewLayer, &sz));

    if(pPreviewLayer == NULL)
      return BAD_STATE;

    ((AVCaptureVideoPreviewLayer*)pPreviewLayer).session = pCaptureSession;
  }

  return MR_NOT_IMPLEMENTED;
}

MinResult AVFoundationDevice::SetProperty(
    const char *pKey,
    const char *pValue) {
  if (pKey == NULL || pValue == NULL)
    return BAD_ARGS;
  return MR_NOT_IMPLEMENTED;
}

@implementation SampleBufferDelegateWrapper

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
                                                       fromConnection:(AVCaptureConnection *)connection
{
  //NSLog(@"outframe: started");
  if(currentBuffer)
  {
    ScopedAutoreleasePool pool;
    CFRelease(currentBuffer);
  }
  CMSampleBufferCreateCopy(kCFAllocatorMalloc, sampleBuffer, &currentBuffer);
  //NSLog(@"outframe: ended");
}

@end
