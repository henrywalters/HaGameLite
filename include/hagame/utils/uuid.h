//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_UUID_H
#define HAGAME2_UUID_H
#include <random>

namespace hg {
    namespace utils {

        using uuid_t = uint32_t;

        static std::random_device device;
        static std::mt19937_64 engine(device());
        static std::uniform_int_distribution<uuid_t> uniDist;

        class UUID {
        public:

            UUID() : uuid(uniDist(engine)) {}
            UUID(uuid_t _uuid) : uuid(_uuid) {}
            UUID(const UUID& _uuid) = default;

            static uuid_t Generate() {
                return uniDist(engine);
            }

            operator uuid_t() const {
                return uuid;
            }

            bool operator ==(const UUID& _uuid) {
                return uuid == _uuid;
            }

            bool operator !=(const UUID& _uuid) {
                return uuid != _uuid;
            }

            bool operator ==(const uuid_t _id) {
                return uuid == _id;
            }

            bool operator !=(const uuid_t _id) {
                return !(uuid == _id);
            }

            uuid_t id() const { return uuid; }

            uuid_t operator() () const {
                return id();
            }
        private:

            uuid_t uuid;
        };
    }
}

#endif //HAGAME2_UUID_H
