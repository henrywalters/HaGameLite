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

    hg::Vec2 pos;
    hg::Vec2 size;

    Cube getBoundingCube() const override {
        return Cube(entity->position() + Vec3(pos[0], pos[1], 0), Vec3(size[0], size[1], 0));
    }

    Rect getRect() const {
        return Rect(entity->position().resize<2>() + pos, size);
    }

protected:

    OBJECT_NAME(RectCollider);

};

HG_COMPONENT(Math, RectCollider)
HG_FIELD(RectCollider, hg::Vec2, pos)
HG_FIELD(RectCollider, hg::Vec2, size)

}

#endif //HAGAME2_RECTCOLLIDER_H
