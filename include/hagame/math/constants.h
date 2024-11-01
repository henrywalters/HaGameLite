//
// Created by henry on 12/13/22.
//

#ifndef HAGAME2_CONSTANTS_H
#define HAGAME2_CONSTANTS_H

namespace hg::math {
    constexpr float PI = 3.14159265359;
    constexpr float RAD2DEG = 180.0f / PI;
    constexpr float DEG2RAD = PI / 180.0;
    constexpr float EPSILON = 0.001;

    template <typename T>
    constexpr T const& clamp(const T& x, const T& hi, const T& lo) {
        return x < lo ? lo : (x > hi ? hi : x);
    }
}

#endif //HAGAME2_CONSTANTS_H
