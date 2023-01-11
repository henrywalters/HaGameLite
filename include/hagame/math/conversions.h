//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_CONVERSIONS_H
#define HAGAME2_CONVERSIONS_H

#include "constants.h"

namespace hg::math {
    constexpr float deg2rad(float rad) {
        return rad * (PI / 180.0f);
    }

    constexpr float rad2deg(float deg) {
        return deg * (180.f / PI);
    }
}

#endif //HAGAME2_CONVERSIONS_H
