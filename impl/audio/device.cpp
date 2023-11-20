//
// Created by henry on 11/17/23.
//
#include <stdexcept>
#include <cstring>
#include "../../include/hagame/audio/device.h"

#include <AL/al.h>
#include <AL/alc.h>

using namespace hg::audio;


std::vector<Device> Device::List() {
    std::vector<Device> devices;

    ALboolean hasExtension = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");

    if (!hasExtension) {
        throw std::runtime_error("Missing OpenAL Enumeration Ext");
    }

    const ALCchar* device = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    const ALCchar* next = device + 1;
    size_t len = 0;

    while (device && *device != '\0' && next && *next != '\0') {
        len = strlen(device);
        Device dev;
        dev.name = device;
        devices.push_back(dev);
        device += (len + 1);
        next += (len + 2);
    }

    return devices;

}
