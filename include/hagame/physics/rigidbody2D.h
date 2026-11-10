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
        utils::Watcher<float> friction = 0.25;
        utils::Watcher<float> restitution = 0.25;
        utils::Watcher<float>  damping = 1.0;
        utils::Watcher<float> gravityScale = 1.0;

        // Rounds the corners of a rect collider's box, keeping its outer size. Stops boxes snagging on the seams
        // between neighbouring colliders. Applied when the body is created (on play).
        float cornerRadius = 0.0f;

        Vec2 velocity() const;
        void setVelocity(Vec2 velocity);

        void applyForce(Vec2 force);
        void applyForce(Vec2 force, Vec2 centerPoint);
        void applyTorque(float torque);
        // Takes the body out of the simulation or puts it back, e.g. while its entity is inactive
        void setEnabled(bool enabled);

        void applyImpulse(Vec2 impulse);
        void applyImpulse(Vec2 impulse, Vec2 centerPoint);

        std::vector<hg::Entity*> collidingWith() const;

        // Whether something is touching this body on the side the axis points to, e.g. (0, -1) = something below
        bool isBlocked(Vec2 axis) const;

        // Whether the body is standing on a surface no steeper than maxSlopeDegrees
        bool isGrounded(float maxSlopeDegrees = 45.0f) const;

        // Rebuilds the shape from the collider, e.g. after resizing it. The body keeps its position and velocity
        void refreshShape();

    protected:

        OBJECT_NAME(RigidBody2D)

    private:

        b2WorldId m_world;
        b2BodyDef m_body;
        b2ShapeDef m_shape;
        b2ShapeId m_shapeId = b2_nullShapeId;
        b2BodyId m_id = b2_nullBodyId; // Null until Physics2D creates the body on play

        std::unordered_map<hg::Entity*, b2Manifold> m_collisions;

        b2Vec2 m_vel;
        float m_angVel;

        // Body position after the previous and latest fixed steps, used to interpolate rendering between them
        Vec2 m_prevPos;
        Vec2 m_currPos;

        // Offset from the latest physics position to where the body should be drawn this frame
        Vec2 interpolationOffset(double alpha) const;

        void initialize(b2WorldId world);

        // Creates m_shapeId from the entity's collider, using m_shape for its material
        void createShape();

        // Normals of the surfaces currently touching this body, pointing away from the surface towards this body
        std::vector<Vec2> contactNormals() const;

        void updateBody();
        void updateTransform();

    };

    HG_COMPONENT(Physics2D, RigidBody2D)
    HG_FIELD(RigidBody2D, bool, dynamic)
    HG_FIELD(RigidBody2D, bool, fixRotation)
    HG_FIELD(RigidBody2D, bool, isBullet)
    HG_NUMBER_FIELD(RigidBody2D, float, restitution, 0, 1, 0.01)
    HG_NUMBER_FIELD(RigidBody2D, float, density, 0, 100, 0.1)
    HG_NUMBER_FIELD(RigidBody2D, float, friction, 0, 100, 0.1)
    HG_NUMBER_FIELD(RigidBody2D, float, damping, 0, 100, 0.1)
    HG_NUMBER_FIELD(RigidBody2D, float, gravityScale, 0, 10, 0.1)
    HG_NUMBER_FIELD(RigidBody2D, float, cornerRadius, 0, 1, 0.01)
}

#endif //HAGAME2_RIGIDBODY2D_H
