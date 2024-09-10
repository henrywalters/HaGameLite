//
// Created by henry on 10/15/23.
//

#ifndef HAGAME2_CIRCLECOLLIDER_H
#define HAGAME2_CIRCLECOLLIDER_H

#include "../aliases.h"
#include "collider.h"

namespace hg::math::components {
    class CircleCollider : public Collider {
    public:

        hg::Vec2 pos;
        float radius;

        Cube getBoundingCube() const override {
            return Cube(Vec3(radius * -1, radius * -1, 0), Vec3(radius * 2, radius * 2, 0));
        }

        Circle getCircle() const {
            return Circle(entity->position().resize<2>() + pos, radius);
        }

    protected:

        OBJECT_NAME(CircleCollider)

    };

    HG_COMPONENT(Math, CircleCollider)
    HG_FIELD(CircleCollider, hg::Vec2, pos)
    HG_FIELD(CircleCollider, float, radius)
}

#endif //HAGAME2_CIRCLECOLLIDER_H
