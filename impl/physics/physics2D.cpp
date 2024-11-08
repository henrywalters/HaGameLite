//
// Created by henry on 11/2/24.
//
#include <hagame/core/scene.h>
#include <hagame/physics/box2D.h>
#include <hagame/physics/physics2D.h>
#include <hagame/physics/rigidbody2D.h>
#include "hagame/math/ray.h"

using namespace hg;
using namespace hg::physics;
using namespace hg::math;

Physics2D::Physics2D(Vec2 gravity):
    m_gravity(gravity)
{
    clear();
}

void Physics2D::clear() {
    if (m_initialized) {
        b2DestroyWorld(m_world);
    }

    auto worldDef = b2DefaultWorldDef();
    worldDef.gravity = FromVec(m_gravity);
    m_world = b2CreateWorld(&worldDef);

    m_initialized = true;
}

void Physics2D::initializeWorld() {
    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        rb->initialize(m_world);
    });
}

void Physics2D::onFixedUpdate(double dt) {

    if (!m_running) {
        return;
    }

    m_shapeMap.clear();

    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        rb->updateBody();
        m_shapeMap.insert(std::make_pair(rb->m_shapeId.index1, entity));
    });

    b2World_Step(m_world, dt, 4);

    auto events = b2World_GetContactEvents(m_world);

    for (int i = 0; i < events.beginCount; i++) {
        auto event = events.beginEvents[i];
        m_shapeMap.at(event.shapeIdA.index1)
            ->getComponent<RigidBody2D>()
            ->m_collisions.insert(std::make_pair(m_shapeMap.at(event.shapeIdB.index1), event.manifold));
        m_shapeMap.at(event.shapeIdB.index1)
            ->getComponent<RigidBody2D>()
            ->m_collisions.insert(std::make_pair(m_shapeMap.at(event.shapeIdA.index1), event.manifold));
    }

    for (int i = 0; i < events.endCount; i++) {
        auto event = events.endEvents[i];
        m_shapeMap.at(event.shapeIdA.index1)
            ->getComponent<RigidBody2D>()
            ->m_collisions.erase(m_shapeMap.at(event.shapeIdB.index1));
        m_shapeMap.at(event.shapeIdB.index1)
            ->getComponent<RigidBody2D>()
            ->m_collisions.erase(m_shapeMap.at(event.shapeIdA.index1));
    }

    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        rb->updateTransform();
    });
}

void Physics2D::play() {
    initializeWorld();
//    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
//
//        if (!b2Body_IsValid(rb->m_id)) {
//            return;
//        }
//
//        if (rb->dynamic) {
//            b2Body_SetLinearVelocity(rb->m_id, rb->m_vel);
//            b2Body_SetAngularVelocity(rb->m_id, rb->m_angVel);
//        }
//    });
    m_running = true;
}

void Physics2D::pause() {
    m_running = false;
//    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
//
//        if (!b2Body_IsValid(rb->m_id)) {
//            return;
//        }
//
//        if (rb->dynamic) {
//            rb->m_vel = b2Body_GetLinearVelocity(rb->m_id);
//            rb->m_angVel = b2Body_GetAngularVelocity(rb->m_id);
//        }
//    });
}

void Physics2D::reset() {
    pause();
    clear();
}

struct TmpHit {
    b2ShapeId shapeId;
    Vec3 pt;
    Vec3 normal;
    float fraction;
};

float _raycast_callback(b2ShapeId shape, b2Vec2 pt, b2Vec2 normal, float fraction, void *hit) {
    TmpHit *tmp = static_cast<TmpHit *>(hit);
    tmp->shapeId = shape;
    tmp->pt = ToVec3(pt);
    tmp->normal = ToVec3(normal);
    tmp->fraction = fraction;
    return fraction;
}

std::optional<Hit> Physics2D::raycast(const Ray &ray) {
    b2QueryFilter filter = b2DefaultQueryFilter();

    TmpHit hit;

    b2World_CastRay(m_world, FromVec(ray.origin), FromVec(ray.direction), filter, _raycast_callback, &hit);

    if (hit.fraction > 0) {
        hg::Entity* hitEntity = m_shapeMap.find(hit.shapeId.index1) == m_shapeMap.end() ? nullptr : m_shapeMap.at(hit.shapeId.index1);
        return Hit {hitEntity, hit.pt, hit.normal, (float)(ray.direction.magnitude() * (1.0 - hit.fraction))};
    }

    return std::nullopt;
}

