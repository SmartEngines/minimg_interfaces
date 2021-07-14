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

#include "dshowsubsystem.h"
#include "dshowdevice.h"
#include "dshowutils.h"
#include "camstream.h"

#include <cstring>
#include <comdef.h>
#include <algorithm>

#include <minbase/minresult.h>
#include <minbase/crossplat.h>

DShowSubSystem::DShowSubSystem()
{
  CoInitialize(NULL);
}

SubSystem * DShowSubSystem::Instance()
{
  static DShowSubSystem *pInstance = NULL;

  if (pInstance == NULL)
    pInstance = new DShowSubSystem();

  return pInstance;
}

MinResult DShowSubSystem::GetDeviceNameList(
    char *pDeviceNames,
    int size) const {
  if (pDeviceNames == NULL)
    return MR_CONTRACT_VIOLATION;

  std::string deviceNameList;
  HRESULT hr = GetCaptureDeviceList(deviceNameList);
  if (FAILED(hr))
    return MR_INTERNAL_ERROR;

  snprintf(pDeviceNames, size, "%s", deviceNameList.c_str());
  return MR_SUCCESS;
}

MinResult DShowSubSystem::OpenStream(
    const char *pDeviceName,
    char *pURI,
    int size) {
  Device *pDevice = DShowDevice::Instance(pDeviceName);
  if (pDevice == NULL)
    return MR_INTERNAL_ERROR;

  Stream *pStream = new CamStream(pDevice);
  if (pStream == NULL)
    return MR_INTERNAL_ERROR;

  snprintf(pURI, size, "dev://%p", pStream);
  return MR_SUCCESS;
}
