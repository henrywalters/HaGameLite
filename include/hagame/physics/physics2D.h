//
// Created by henry on 11/2/24.
//

#ifndef HAGAME2_PHYSICS2D_H
#define HAGAME2_PHYSICS2D_H

#include <hagame/core/system.h>
#include <memory>
#include <hagame/math/aliases.h>
#include <hagame/math/transform.h>
#include <hagame/core/entity.h>
#include <optional>

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

        void onBeforeUpdate() override;
        void onFixedUpdate(double dt) override;

        std::optional<Hit> raycast(const math::Ray& ray);

        double lastTick() const { return m_elapsedTime; }

        Vec2 gravity() const { return m_gravity; }

        void play();
        void pause();
        void reset();

    private:

        // Each shape's entity, by the shape's index, as an id rather than a pointer: an entity removed between steps
        // (picked up, killed, cleared by a script) would otherwise leave a dangling pointer here, and Box2D hands a
        // freed shape's index to the next shape made. Look entities up with entityForShape, never directly
        std::unordered_map<int32_t, utils::uuid_t> m_shapeMap;

        // Shapes of the static bodies built from tilemaps on play. They have no RigidBody2D, so they're kept here
        // and merged into m_shapeMap each step
        std::unordered_map<int32_t, utils::uuid_t> m_tilemapShapes;

        void createTilemapBody(hg::Entity* entity);

        // The live entity that owns shape, or null when it's gone or the index now belongs to another shape
        hg::Entity* entityForShape(b2ShapeId shape);

        double m_elapsedTime = 0;

        bool m_running = false;

        bool m_initialized = false;
        Vec2 m_gravity;

        b2WorldId m_world;

    };
}


#endif //HAGAME2_PHYSICS2D_H
