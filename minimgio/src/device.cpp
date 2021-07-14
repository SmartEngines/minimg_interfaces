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

#include <cstring>
#include <cstdio>

#include <minimgio/device.h>
#include <minimgio/define.h>
#include <minbase/minresult.h>

#include "subsystem.h"
#include "stream.h"

namespace minimgio {
MINIMGIO_API MinResult GetDeviceList
(
  const char *pSubSystemName,
  char       *pDeviceNames,
  int         size
)
{
  if (pSubSystemName == NULL || pDeviceNames == NULL)
    return MR_CONTRACT_VIOLATION;

  SubSystem *pSubSystem = SubSystem::Instance(pSubSystemName);
  if (pSubSystem == NULL)
    return MR_CONTRACT_VIOLATION;

  return pSubSystem->GetDeviceNameList(pDeviceNames, size);
}

MINIMGIO_API MinResult OpenStream
(
  const char *pSubSystemName,
  const char *pDeviceName,
  char       *pURI,
  int         size
)
{
  if (pSubSystemName == NULL || pDeviceName == NULL)
    return MR_CONTRACT_VIOLATION;

  SubSystem *pSubSystem = SubSystem::Instance(pSubSystemName);
  if (pSubSystem == NULL)
    return MR_CONTRACT_VIOLATION;

  return pSubSystem->OpenStream(pDeviceName, pURI, size);
}

MINIMGIO_API MinResult CloseStream
(
  const char *pURI
)
{
  if (pURI == NULL)
    return MR_CONTRACT_VIOLATION;

  void *pStream = NULL;
  sscanf(pURI, "dev://%p", &pStream);
  if (pStream == NULL)
    return MR_INTERNAL_ERROR;

  delete static_cast<Stream*>(pStream);
  return MR_SUCCESS;
}

MINIMGIO_API MinResult GetStreamProperty
(
  const char *pURI,
  const char *pPropertyKey,
  char       *pPropertyValue,
  int         size
)
{
  if (pURI == NULL || pPropertyKey == NULL || pPropertyValue == NULL)
    return MR_CONTRACT_VIOLATION;

  void *pStream = NULL;
  sscanf(pURI, "dev://%p", &pStream);
  if (pStream == NULL)
    return MR_INTERNAL_ERROR;

  return static_cast<Stream*>(pStream
      )->GetProperty(pPropertyKey, pPropertyValue, size);
}

MINIMGIO_API MinResult SetStreamProperty
(
  const char *pURI,
  const char *pPropertyKey,
  const char *pPropertyValue
)
{
  if (pURI == NULL || pPropertyKey == NULL || pPropertyValue == NULL)
    return MR_CONTRACT_VIOLATION;

  void *pStream = NULL;
  sscanf(pURI, "dev://%p", &pStream);
  if (pStream == NULL)
    return MR_INTERNAL_ERROR;

  return static_cast<Stream*>(pStream
      )->SetProperty(pPropertyKey, pPropertyValue);
}
}  // namespace minimgio


MINIMGIO_API int GetDeviceList(
    const char *pSubSystemName,
    char       *pDeviceNames,
    int         size) {
  return static_cast<int>(minimgio::GetDeviceList(
    pSubSystemName, pDeviceNames, size));
}

MINIMGIO_API int OpenStream(
    const char *pSubSystemName,
    const char *pDeviceName,
    char       *pURI,
    int         size) {
  return static_cast<int>(minimgio::OpenStream(
    pSubSystemName, pDeviceName, pURI, size));
}

MINIMGIO_API int CloseStream(
    const char *pURI) {
  return static_cast<int>(minimgio::CloseStream(pURI));
}

MINIMGIO_API int GetStreamProperty(
    const char *pURI,
    const char *pPropertyKey,
    char       *pPropertyValue,
    int         size) {
  return static_cast<int>(minimgio::GetStreamProperty(
    pURI, pPropertyKey, pPropertyValue, size));
}

MINIMGIO_API int SetStreamProperty(
    const char *pURI,
    const char *pPropertyKey,
    const char *pPropertyValue) {
  return static_cast<int>(minimgio::SetStreamProperty(
    pURI, pPropertyKey, pPropertyValue));
}
