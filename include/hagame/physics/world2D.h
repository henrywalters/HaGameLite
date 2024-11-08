//
// Created by henry on 11/2/24.
//

#ifndef HAGAME2_WORLD2D_H
#define HAGAME2_WORLD2D_H

#include <memory>
#include "../math/aliases.h"
#include "../math/transform.h"

#include <box2d/box2d.h>

namespace hg::physics {

    class World2D {
    public:

        World2D(hg::Vec2 gravity = hg::Vec2(0, -9.8f));
        ~World2D();

        void clear();

        b2BodyId addRectBody(Vec2 pos, Vec2 size);

        b2BodyId addDynamicBody(Vec2 pos, Vec2 size, float mass, float density = 1.0, float friction = 0.3);

        void syncTransform(math::Transform& transform, b2BodyId bodyId);

        void step(double dt);

    private:

        Vec2 m_gravity;

        b2WorldId m_world;

    };


}

#endif //HAGAME2_WORLD2D_H
