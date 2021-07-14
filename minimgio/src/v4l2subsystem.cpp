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

#include <minbase/minresult.h>

#include "v4l2subsystem.h"
#include "v4l2device.h"
#include "v4l2utils.h"
#include "camstream.h"

#include <string>
#include <sstream>

V4L2SubSystem::V4L2SubSystem() {
  // empty
}

SubSystem* V4L2SubSystem::Instance() {
  static V4L2SubSystem instance;
  return &instance;
}

MinResult V4L2SubSystem::GetDeviceNameList(
    char* p_device_names,
    int size) const {
  if (!p_device_names)
    return MR_CONTRACT_VIOLATION;

  std::map<std::string, std::string> name_addr_map;
  MR_PROPAGATE_ERROR(GetStreamingCaptureDeviceNameAddrMap(name_addr_map));
  std::string device_names_list = "";
  for (const auto& name_addr_pair : name_addr_map) {
    device_names_list += name_addr_pair.first.c_str();
    device_names_list += "\n";
  }
  if (::snprintf(p_device_names, size, "%s", device_names_list.c_str()) > size)
    return MR_ENV_ERROR;

  return MR_SUCCESS;
}

MinResult V4L2SubSystem::OpenStream(
    const char* p_device_name,
    char*       p_URI,
    int         size) {
  std::istringstream iss(p_device_name);
  std::string device_name;
  if (!std::getline(iss, device_name))
    return MR_CONTRACT_VIOLATION;

  std::string frame_size = "";
  std::string frame_rate = "";
  if (std::getline(iss, frame_size))
    std::getline(iss, frame_rate);

  std::map<std::string, std::string> name_addr_map;
  MR_PROPAGATE_ERROR(GetStreamingCaptureDeviceNameAddrMap(name_addr_map));
  auto it = name_addr_map.find(device_name);
  if (name_addr_map.end() == it)
    return MR_CONTRACT_VIOLATION;

  Device* p_device = V4L2Device::Instance(it->second, frame_size, frame_rate);
  if (!p_device)
    return MR_INTERNAL_ERROR;

  Stream* p_stream = new CamStream(p_device);
  if (!p_stream)
    return MR_INTERNAL_ERROR;

  if (::snprintf(p_URI, size, "dev://%p", static_cast<void*>(p_stream)) > size)
    return MR_ENV_ERROR;

  return MR_SUCCESS;
}
