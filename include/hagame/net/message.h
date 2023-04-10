//
// Created by henry on 4/6/23.
//

#ifndef HAGAME2_MESSAGE_H
#define HAGAME2_MESSAGE_H

#include "net.h"
#include "../utils/uuid.h"

namespace hg::net {
    template <typename T>
    struct MessageHeader {
        T id{};
        utils::uuid_t size;
    };

    template <typename T>
    struct Message {
        MessageHeader<T> header;
        std::vector<uint8_t> body;

        size_t size() const {
            return body.size();
        }

        friend std::ostream& operator << (std::ostream& os, const Message<T>& msg) {
            os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
            return os;
        }

        template <class DataType>
        friend Message<T>& operator << (Message<T>&msg, const DataType& data) {
            std::cout << "LOADING DATA TO MESSAGE\n";
            static_assert(std::is_standard_layout<DataType>::value, "Data must be standard layout");
            size_t msgSize = msg.body.size();
            msg.body.resize(msg.body.size() + sizeof(DataType));
            std::memcpy(msg.body.data() + msgSize, &data, sizeof(DataType));
            msg.header.size = msg.size();
            std::cout << msg.size() << "\n";
            return msg;
        }

        template <class DataType>
        friend Message<T>& operator >> (Message<T>& msg, DataType& data) {
            static_assert(std::is_standard_layout<DataType>::value, "Data must be standard layout");
            size_t msgSize = msg.body.size() - sizeof(DataType);
            std::memcpy(&data, msg.body.data() + msgSize, sizeof(DataType));
            msg.body.resize(msgSize);
            msg.header.size = msg.size();
            return msg;
        }
    };

    template <typename T>
    class Connection;

    template <typename T>
    struct OwnedMessage {
        std::shared_ptr<Connection<T>> remote = nullptr;
        Message<T> msg;

        friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg) {
            os << msg.msg;
            return os;
        }
    };
}

#endif //HAGAME2_MESSAGE_H
