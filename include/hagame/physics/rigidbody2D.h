//
// Created by henry on 11/2/24.
//

#ifndef HAGAME2_RIGIDBODY2D_H
#define HAGAME2_RIGIDBODY2D_H

#include "../core/component.h"
#include "../core/entity.h"

#include <box2d/box2d.h>

namespace hg::physics {

    class Physics2D;

    class RigidBody2D : public hg::Component {
    public:

        friend class Physics2D;

        RigidBody2D();
        ~RigidBody2D();

        utils::Watcher<bool> dynamic = true;
        utils::Watcher<bool> fixRotation = false;
        utils::Watcher<bool> isBullet = false;
        utils::Watcher<float> density = 1.0;
        utils::Watcher<float> friction = 1.0;
        utils::Watcher<float> restitution = 1.0;
        utils::Watcher<float>  damping = 1.0;

        Vec2 velocity() const;

        void applyForce(Vec2 force);
        void applyForce(Vec2 force, Vec2 centerPoint);
        void applyTorque(float torque);
        void applyImpulse(Vec2 impulse);
        void applyImpulse(Vec2 impulse, Vec2 centerPoint);

        std::vector<hg::Entity*> collidingWith() const;

        bool isBlocked(Vec2 axis) const;

    protected:

        OBJECT_NAME(RigidBody2D)

    private:

        b2WorldId m_world;
        b2BodyDef m_body;
        b2ShapeDef m_shape;
        b2ShapeId m_shapeId;
        b2BodyId m_id;

        std::unordered_map<hg::Entity*, b2Manifold> m_collisions;

        b2Vec2 m_vel;
        float m_angVel;

        void initialize(b2WorldId world);

        void updateBody();
        void updateTransform();

    };

    HG_COMPONENT(Physics, RigidBody2D)
    HG_FIELD(RigidBody2D, bool, dynamic)
    HG_FIELD(RigidBody2D, bool, fixRotation)
    HG_FIELD(RigidBody2D, bool, isBullet)
    HG_NUMBER_FIELD(RigidBody2D, float, restitution, 0, 1, 0.01)
    HG_NUMBER_FIELD(RigidBody2D, float, density, 0, 100, 0.1)
    HG_NUMBER_FIELD(RigidBody2D, float, friction, 0, 100, 0.1)
    HG_NUMBER_FIELD(RigidBody2D, float, damping, 0, 100, 0.1)
}

#endif //HAGAME2_RIGIDBODY2D_H
