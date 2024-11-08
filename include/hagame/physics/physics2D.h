//
// Created by henry on 11/2/24.
//

#ifndef HAGAME2_PHYSICS2D_H
#define HAGAME2_PHYSICS2D_H

#include <hagame/core/system.h>
#include <memory>
#include <hagame/math/aliases.h>
#include <hagame/math/transform.h>

#include <box2d/box2d.h>
#include "hagame/math/ray.h"

namespace hg::physics {

    struct Hit {
        hg::Entity* entity;
        Vec3 position;
        Vec3 normal;
        float depth;

        // Calculates the vector needed to offset the collision
        Vec3 calcOffset() {
            return position + normal * -depth;
        }
    };

    class Physics2D : public hg::System {
    public:

        explicit Physics2D(Vec2 gravity);

        void clear();

        void initializeWorld();

        void onFixedUpdate(double dt) override;

        std::optional<Hit> raycast(const math::Ray& ray);

        void play();
        void pause();
        void reset();

    private:

        std::unordered_map<int32_t, hg::Entity*> m_shapeMap;

        bool m_running = false;

        bool m_initialized = false;
        Vec2 m_gravity;

        b2WorldId m_world;

    };
}


#endif //HAGAME2_PHYSICS2D_H
