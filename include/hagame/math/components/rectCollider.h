//
// Created by henry on 10/15/23.
//

#ifndef HAGAME2_RECTCOLLIDER_H
#define HAGAME2_RECTCOLLIDER_H

#include "../aliases.h"
#include "collider.h"

namespace hg::math::components {
class RectCollider : public Collider {
public:

    hg::Rect rect;

    Cube getBoundingCube() const override {
        return Cube(Vec3(rect.pos[0], rect.pos[1], 0), Vec3(rect.size[0], rect.size[1], 0));
    }

};

}

#endif //HAGAME2_RECTCOLLIDER_H
