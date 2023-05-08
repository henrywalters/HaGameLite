//
// Created by henry on 4/15/23.
//

#ifndef HAGAME2_HASH_H
#define HAGAME2_HASH_H

#include "../math/vector.h"

namespace hg::utils {
    template <typename Type>
    class Hash {
    public:

        static inline long Vector(hg::math::Vector<3, Type> vec, hg::math::Vector<3, Type> chunkSize) {
            return (((int) (vec[0] / chunkSize[0]) * P1) ^ ((int)(vec[1] / chunkSize[1]) * P2) ^ ((int)(vec[2] / chunkSize[2]) * P3)) % MAX_CHUNKS;
        }

    private:

        static const long MAX_CHUNKS = 100000000;
        static const long P1 = 73856093;
        static const long P2 = 19349663;
        static const long P3 = 83492791;

    };
}

#endif //HAGAME2_HASH_H
