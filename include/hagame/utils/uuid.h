//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_UUID_H
#define HAGAME2_UUID_H
#include <random>

namespace hg {
    namespace utils {

        static std::random_device device;
        static std::mt19937_64 engine(device());
        static std::uniform_int_distribution<uint64_t> uniDist;

        class UUID {
        public:

            UUID() : uuid(uniDist(engine)) {}
            UUID(uint64_t _uuid) : uuid(_uuid) {}
            UUID(const UUID& _uuid) = default;

            operator uint64_t() const {
                return uuid;
            }

            bool operator ==(const UUID& _uuid) {
                return uuid == _uuid;
            }

            bool operator !=(const UUID& _uuid) {
                return uuid != _uuid;
            }

            bool operator ==(const uint64_t _id) {
                return uuid == _id;
            }

            bool operator !=(const uint64_t _id) {
                return !(uuid == _id);
            }

            uint64_t id() const { return uuid; }

        private:

            uint64_t uuid;
        };
    }
}

#endif //HAGAME2_UUID_H
