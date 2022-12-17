//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_HSV_H
#define HAGAME2_HSV_H

#include "../math/aliases.h"

namespace hg::graphics {
    struct HSV : public Vec3 {

        HSV() : HSV(0, 0, 0) {}
        HSV(float h, float s, float v) {
            vector[0] = h;
            vector[1] = s;
            vector[2] = v;
        }
    };
}

#endif //HAGAME2_HSV_H
