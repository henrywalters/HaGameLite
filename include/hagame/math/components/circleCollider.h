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

        hg::Circle circle;

        Cube getBoundingCube() const override {
            return Cube(Vec3(circle.radius * -1, circle.radius * -1, 0), Vec3(circle.radius * 2, circle.radius * 2, 0));
        }

    };
}

#endif //HAGAME2_CIRCLECOLLIDER_H
