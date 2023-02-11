//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_UUID_H
#define HAGAME2_UUID_H
#include <random>

namespace hg {
    namespace utils {

        using UUID_SIZE = uint32_t;

        static std::random_device device;
        static std::mt19937_64 engine(device());
        static std::uniform_int_distribution<UUID_SIZE> uniDist;

        class UUID {
        public:

            UUID() : uuid(uniDist(engine)) {}
            UUID(UUID_SIZE _uuid) : uuid(_uuid) {}
            UUID(const UUID& _uuid) = default;

            operator UUID_SIZE() const {
                return uuid;
            }

            bool operator ==(const UUID& _uuid) {
                return uuid == _uuid;
            }

            bool operator !=(const UUID& _uuid) {
                return uuid != _uuid;
            }

            bool operator ==(const UUID_SIZE _id) {
                return uuid == _id;
            }

            bool operator !=(const UUID_SIZE _id) {
                return !(uuid == _id);
            }

            UUID_SIZE id() const { return uuid; }

        private:

            UUID_SIZE uuid;
        };
    }
}

#endif //HAGAME2_UUID_H
