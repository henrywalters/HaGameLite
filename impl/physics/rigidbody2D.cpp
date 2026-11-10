//
// Created by henry on 11/2/24.
//
#include <hagame/physics/rigidbody2D.h>
#include <hagame/physics/box2D.h>
#include <hagame/math/collisions.h>
#include <hagame/math/components/rectCollider.h>
#include <hagame/math/components/circleCollider.h>
#include <hagame/math/functions.h>
#include <algorithm>
#include <cmath>

using namespace hg::physics;
using namespace hg::math::collisions;
using namespace hg::math::components;

#define VALID_ID if (!b2Body_IsValid(m_id)) { \
    return; \
}


RigidBody2D::RigidBody2D() {

    std::cout << "INITIALIZING RIGIDBODY 2D\n";

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
        std::cout << "SET DAMPING TO " << damping << "\n";
        b2Body_SetLinearDamping(m_id, damping);
    };

    gravityScale.onUpdate = [&]() {
        VALID_ID
        b2Body_SetGravityScale(m_id, gravityScale);
    };

    restitution.onUpdate = [&]() {
        VALID_ID
        std::cout << "SETTING RESTITUTION TO " << restitution << "\n";
        b2Shape_SetRestitution(m_shapeId, restitution);
    };
}

RigidBody2D::~RigidBody2D() {
    VALID_ID
    b2DestroyBody(m_id);
}

// Bodies live in world space, so they're placed from the entity's world transform, which for a child entity takes in
// its parents. Signed angle about z: acos(w) * 2 drops the sign, which flipped every clockwise rotation back each step
static float worldAngle(hg::Entity* entity) {
    auto q = entity->rotation();
    return 2.0f * std::atan2(q[3], q[0]);
}

// Puts the entity at a world position and angle by setting its own transform relative to its parent. Its z, the draw
// layer, isn't simulated and is left alone
static void setWorldPose(hg::Entity* entity, hg::Vec2 position, float angle) {
    hg::Quat rotation(angle, hg::Vec3::Face());
    auto parent = entity->parent();
    if (!parent) {
        entity->transform.position = hg::Vec3(position[0], position[1], entity->transform.position[2]);
        entity->transform.rotation = rotation;
        return;
    }
    hg::Quat parentRotation = parent->rotation();
    hg::Vec3 parentScale = parent->scale();
    hg::Vec3 local = parentRotation.conjugate().rotatePoint(hg::Vec3(position[0], position[1], 0.0f) - parent->position());
    for (int i = 0; i < 2; i++) {
        local[i] = parentScale[i] != 0.0f ? local[i] / parentScale[i] : 0.0f;
    }
    entity->transform.position = hg::Vec3(local[0], local[1], entity->transform.position[2]);
    entity->transform.rotation = parentRotation.conjugate() * rotation;
}

void RigidBody2D::updateBody() {
    VALID_ID

    // Something outside physics moved the entity (editor, teleport, or a parent moving a static body along with it):
    // push it into the body. Only x / y are simulated; z is the entity's draw layer and must not count as a change.
    float entityAngle = worldAngle(entity);
    float bodyAngle = b2Rot_GetAngle(b2Body_GetRotation(m_id));
    float angleDelta = std::remainder(entityAngle - bodyAngle, 2.0f * (float) hg::math::PI);

    auto pos = ToVec(b2Body_GetPosition(m_id));
    auto entityPos = entity->position().resize<2>();

    // Tolerances so float round-off from our own write-back doesn't count as an external change. A little looser than
    // before, as the write-back now goes through the parent's transform and back
    constexpr float EPS = 1e-4f;
    if (std::abs(angleDelta) > EPS || std::abs(entityPos[0] - pos[0]) > EPS || std::abs(entityPos[1] - pos[1]) > EPS) {
        if (dynamic) {
            b2Body_SetAwake(m_id, true);
        }
        b2Body_SetTransform(m_id, FromVec(entityPos), b2MakeRot(entityAngle));
        // Teleports shouldn't be smeared across frames
        m_prevPos = entityPos;
        m_currPos = entityPos;
    }
}

void RigidBody2D::updateTransform()  {
    VALID_ID

    if (dynamic) {
        auto pos = b2Body_GetPosition(m_id);
        m_prevPos = m_currPos;
        m_currPos = ToVec(pos);
        setWorldPose(entity, ToVec(pos), b2Rot_GetAngle(b2Body_GetRotation(m_id)));
    }
}

void RigidBody2D::setEnabled(bool enabled) {
    VALID_ID
    if (b2Body_IsEnabled(m_id) != enabled) {
        if (enabled) {
            b2Body_Enable(m_id);
        } else {
            b2Body_Disable(m_id);
        }
    }
}

void RigidBody2D::setVelocity(hg::Vec2 velocity) {
    VALID_ID
    b2Body_SetLinearVelocity(m_id, FromVec(velocity));
}

hg::Vec2 RigidBody2D::interpolationOffset(double alpha) const {
    return (m_prevPos + (m_currPos - m_prevPos) * alpha) - m_currPos;
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

    m_world = world;

    // The body sits on the entity, with the collider's pos as the shape's offset from it. Placing the body on the
    // collider instead lost the offset on the first sync, which moves the body back onto the entity
    m_body = b2DefaultBodyDef();
    m_body.type = dynamic ? b2_dynamicBody : b2_staticBody;
    m_body.position = FromVec(entity->position().resize<2>());
    m_body.rotation = b2MakeRot(worldAngle(entity));
    m_body.fixedRotation = fixRotation;
    m_body.isBullet = isBullet;
    m_body.gravityScale = gravityScale;
    m_body.linearDamping = damping;

    m_id = b2CreateBody(m_world, &m_body);

    m_prevPos = entity->position().resize<2>();
    m_currPos = m_prevPos;

    m_shape = b2DefaultShapeDef();
    m_shape.friction = friction;
    m_shape.density = density;
    m_shape.restitution = restitution;

    createShape();
}

void RigidBody2D::refreshShape() {
    VALID_ID
    if (b2Shape_IsValid(m_shapeId)) {
        b2DestroyShape(m_shapeId, false);
    }
    createShape();
}

void RigidBody2D::createShape() {
    auto rectColl = entity->getComponent<RectCollider>();
    auto circleColl = entity->getComponent<CircleCollider>();

    if (rectColl) {
        // Same placement as RectCollider::getRect, in the entity's unrotated local space. Scaled by its parents too, as
        // the body is in world space
        Vec2 size = entity->scale().resize<2>().prod(rectColl->size);
        float hx = size[0] / 2;
        float hy = size[1] / 2;
        Vec2 center = rectColl->centered ? rectColl->pos : rectColl->pos + size * 0.5f;
        float radius = std::clamp(cornerRadius, 0.0f, std::min(hx, hy) * 0.9f);
        auto box = radius > 0 ?
            b2MakeOffsetRoundedBox(hx - radius, hy - radius, FromVec(center), b2Rot_identity, radius) :
            b2MakeOffsetBox(hx, hy, FromVec(center), b2Rot_identity);
        m_shapeId = b2CreatePolygonShape(m_id, &m_shape, &box);
    } else if (circleColl) {
        b2Circle circle;
        circle.center = FromVec(circleColl->pos);
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

std::vector<hg::Vec2> RigidBody2D::contactNormals() const {
    std::vector<Vec2> normals;
    if (!b2Body_IsValid(m_id)) {
        return normals;
    }

    // Live contact data rather than the manifold captured when the contact began, which goes stale
    std::vector<b2ContactData> contacts(b2Body_GetContactCapacity(m_id));
    int count = b2Body_GetContactData(m_id, contacts.data(), contacts.size());

    for (int i = 0; i < count; i++) {
        const auto& contact = contacts[i];
        if (contact.manifold.pointCount == 0) {
            continue;
        }
        // The manifold normal points from shape A to shape B, so flip it when this body is A
        bool isA = B2_ID_EQUALS(b2Shape_GetBody(contact.shapeIdA), m_id);
        Vec2 normal = ToVec(contact.manifold.normal);
        normals.push_back(isA ? normal * -1 : normal);
    }

    return normals;
}

bool RigidBody2D::isBlocked(Vec2 axis) const {
    for (const auto& normal : contactNormals()) {
        // A surface blocks movement along axis when its normal faces back against it
        if ((normal * -1).dot(axis) > 0.5) {
            return true;
        }
    }
    return false;
}

bool RigidBody2D::isGrounded(float maxSlopeDegrees) const {
    float minUp = std::cos(maxSlopeDegrees * hg::math::DEG2RAD);
    for (const auto& normal : contactNormals()) {
        if (normal.y() >= minUp) {
            return true;
        }
    }
    return false;
}
