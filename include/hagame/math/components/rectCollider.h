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
    bool centered = false;

    Cube getBoundingCube() const override {
        return Cube(
            entity->position() + Vec3(pos[0], pos[1], 0) - (centered ? Vec3(size[0], size[1], 0) * 0.5 : Vec3::Zero()),
            Vec3(size[0], size[1], 0)
        );
    }

    Rect getRect() const {
        auto adjSize = entity->transform.scale.resize<2>().prod(size);
        auto adjPos = entity->position().resize<2>() + pos - (centered ? adjSize * 0.5 : Vec2::Zero());
        return Rect(adjPos, adjSize);
    }

protected:

    OBJECT_NAME(RectCollider);

};

HG_COMPONENT(Math, RectCollider)
HG_FIELD(RectCollider, hg::Vec2, pos)
HG_FIELD(RectCollider, hg::Vec2, size)
HG_FIELD(RectCollider, bool, centered)

}

#endif //HAGAME2_RECTCOLLIDER_H
