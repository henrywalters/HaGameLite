//
// Created by henry on 11/17/23.
//

#ifndef HAGAMELITE_DEVICE_H
#define HAGAMELITE_DEVICE_H

#include <vector>
#include <string>

namespace hg::audio {
    struct Device {
        std::string name;
        static std::vector<Device> List();
    };
}

#endif //HAGAMELITE_DEVICE_H
