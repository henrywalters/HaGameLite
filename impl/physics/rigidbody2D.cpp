//
// Created by henry on 11/2/24.
//
#include <hagame/physics/rigidbody2D.h>
#include <hagame/physics/box2D.h>
#include <hagame/math/collisions.h>
#include <hagame/math/components/rectCollider.h>
#include <hagame/math/components/circleCollider.h>

using namespace hg::physics;
using namespace hg::math::collisions;
using namespace hg::math::components;

#define VALID_ID if (!b2Body_IsValid(m_id)) { \
    return; \
}


RigidBody2D::RigidBody2D() {
    dynamic.onUpdate = [&]() {
        VALID_ID
        b2Body_SetType(m_id, dynamic ? b2_dynamicBody : b2_staticBody);
    };

    fixRotation.onUpdate = [&]() {
        VALID_ID
        b2Body_SetFixedRotation(m_id, fixRotation);
    };

    isBullet.onUpdate = [&]() {
        VALID_ID
        b2Body_SetBullet(m_id, isBullet);
    };

    friction.onUpdate = [&]() {
        VALID_ID
        b2Shape_SetFriction(m_shapeId, friction);
    };

    density.onUpdate = [&]() {
        VALID_ID
        b2Shape_SetDensity(m_shapeId, density, true);
    };

    damping.onUpdate = [&]() {
        VALID_ID
        b2Body_SetLinearDamping(m_id, damping);
    };

    restitution.onUpdate = [&]() {
        VALID_ID
        b2Shape_SetRestitution(m_shapeId, restitution);
    };
}

RigidBody2D::~RigidBody2D() {
    VALID_ID
    b2DestroyBody(m_id);
}

void RigidBody2D::updateBody() {
    VALID_ID

    if (dynamic) {
        auto angle = b2MakeRot(std::acos(entity->transform.rotation.real()) * 2);
        auto currentAngle = b2Body_GetRotation(m_id);
        auto pos = ToVec3(b2Body_GetPosition(m_id));
        if (angle.c != currentAngle.c || angle.s != currentAngle.s || entity->transform.position != pos) {
            b2Body_SetAwake(m_id, true);
            b2Body_SetTransform(m_id, FromVec(entity->transform.position), angle);
        }
    }
}

void RigidBody2D::updateTransform()  {
    VALID_ID

    if (dynamic) {
        auto pos = b2Body_GetPosition(m_id);
        entity->transform.position = Vec3(pos.x, pos.y, 0);
        entity->transform.rotation = Quat(b2Rot_GetAngle(b2Body_GetRotation(m_id)), Vec3::Face());
    }
}

hg::Vec2 RigidBody2D::velocity() const {
    if (!b2Body_IsValid(m_id)) {
        return Vec2::Zero();
    }
    return ToVec(b2Body_GetLinearVelocity(m_id));
}

void RigidBody2D::initialize(b2WorldId world) {
    auto rectColl = entity->getComponent<RectCollider>();
    auto circleColl = entity->getComponent<CircleCollider>();

    b2Vec2 pos;

    if (rectColl) {
        pos = FromVec(rectColl->getRect().getCenter());
    } else if (circleColl) {
        pos = FromVec(circleColl->getCircle().center);
    }

    m_world = world;

    m_body = b2DefaultBodyDef();
    m_body.type = dynamic ? b2_dynamicBody : b2_staticBody;
    m_body.position = pos;
    m_body.fixedRotation = fixRotation;
    m_body.isBullet = isBullet;

    m_id = b2CreateBody(m_world, &m_body);

    m_shape = b2DefaultShapeDef();
    m_shape.friction = friction;
    m_shape.density = density;
    m_shape.restitution = restitution;

    if (rectColl) {
        auto rect = rectColl->getRect();
        auto box = b2MakeBox(rect.size[0] / 2, rect.size[1] / 2);
        m_shapeId = b2CreatePolygonShape(m_id, &m_shape, &box);
    } else {
        b2Circle circle;
        circle.center = FromVec(circleColl->getCircle().center);
        circle.radius = circleColl->radius;
        m_shapeId = b2CreateCircleShape(m_id, &m_shape, &circle);
    }
}

void RigidBody2D::applyForce(hg::Vec2 force) {
    VALID_ID
    b2Body_ApplyForceToCenter(m_id, FromVec(force), true);
}

void RigidBody2D::applyForce(hg::Vec2 force, hg::Vec2 centerPoint) {
    VALID_ID
    b2Body_ApplyForce(m_id, FromVec(force), FromVec(centerPoint), true);
}

void RigidBody2D::applyTorque(float torque) {
    VALID_ID
    b2Body_ApplyTorque(m_id, torque, true);
}

void RigidBody2D::applyImpulse(hg::Vec2 impulse) {
    VALID_ID
    b2Body_ApplyLinearImpulseToCenter(m_id, FromVec(impulse), true);
}

void RigidBody2D::applyImpulse(hg::Vec2 impulse, hg::Vec2 centerPoint) {
    VALID_ID
    b2Body_ApplyLinearImpulse(m_id, FromVec(impulse), FromVec(centerPoint),true);
}

std::vector<hg::Entity *> RigidBody2D::collidingWith() const {
    std::vector<hg::Entity *> out;
    for (const auto& [entity, manifold] : m_collisions) {
        out.push_back(entity);
    }
    return out;
}

bool RigidBody2D::isBlocked(Vec2 axis) const {
    for (const auto& [entity, manifold] : m_collisions) {
        //std::cout << entity->name << ": " << manifold.normal.x << ", " << manifold.normal.y << "\n";
        float dot = axis.dot(ToVec(manifold.normal));
        //std::cout << dot << "\n";
        if (std::abs(dot) > 0.5) {
            return true;
        }
    }
    return false;
}



