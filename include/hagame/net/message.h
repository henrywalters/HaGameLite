//
// Created by henry on 4/6/23.
//

#ifndef HAGAME2_MESSAGE_H
#define HAGAME2_MESSAGE_H

#include <cinttypes>
#include <vector>

namespace hg::net {
    template <typename T>
    struct MessageHeader {
        T key;
        uint32_t size;
    };

    template <typename T>
    struct Message {
        MessageHeader<T> header;
        std::vector<uint8_t> buffer;
    };
}

#endif //HAGAME2_MESSAGE_H
