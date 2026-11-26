//
// Created by henry on 11/2/24.
//
#include <hagame/core/scene.h>
#include <hagame/physics/box2D.h>
#include <hagame/physics/physics2D.h>
#include <hagame/physics/rigidbody2D.h>
#include <hagame/utils/profiler.h>
#include <hagame/core/game.h>
#include <hagame/common/components/tilemap.h>
#include "hagame/math/ray.h"
#include <unordered_set>

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
    m_tilemapShapes.clear();
    m_shapeMap.clear();

    m_initialized = true;
}

void Physics2D::initializeWorld() {
    // Inactive entities get a body too, switched off, so switching them on later brings it back
    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        rb->initialize(m_world);
        rb->setEnabled(entity->activeInHierarchy());
    }, {}, {}, true);
    scene->entities.forEach<components::Tilemap>([&](components::Tilemap* tilemap, Entity* entity) {
        createTilemapBody(entity);
    });
}

void Physics2D::createTilemapBody(Entity* entity) {
    auto tilemap = entity->getComponent<components::Tilemap>();
    if (tilemap->tileSize[0] <= 0 || tilemap->tileSize[1] <= 0) {
        return;
    }
    auto outlines = tilemap->getCollisionOutlines();
    auto polygons = tilemap->getCollisionPolygons();
    if (outlines.empty() && polygons.empty()) {
        return;
    }

    auto bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = FromVec(entity->position().resize<2>());
    auto body = b2CreateBody(m_world, &bodyDef);

    // RigidBody2D's default friction, and no bounce off walls
    auto shapeDef = b2DefaultShapeDef();
    shapeDef.friction = 0.25;
    shapeDef.restitution = 0.0;

    // One chain loop per outline rather than a box per tile run: separate boxes leave internal corners that bodies
    // sliding along the surface catch on, while a chain's segments know their neighbours and collide smoothly
    for (const auto& outline : outlines) {
        std::vector<b2Vec2> points;
        for (const auto& corner : outline) {
            points.push_back(FromVec(corner));
        }

        auto chainDef = b2DefaultChainDef();
        chainDef.points = points.data();
        chainDef.count = points.size();
        chainDef.isLoop = true;
        chainDef.friction = shapeDef.friction;
        chainDef.restitution = shapeDef.restitution;
        auto chain = b2CreateChain(body, &chainDef);

        std::vector<b2ShapeId> segments(b2Chain_GetSegmentCount(chain));
        b2Chain_GetSegments(chain, segments.data(), segments.size());
        for (const auto& segment : segments) {
            m_tilemapShapes.insert(std::make_pair(segment.index1, entity->id()));
        }
    }

    for (const auto& polygon : polygons) {
        std::vector<b2Vec2> points;
        for (const auto& point : polygon) {
            points.push_back(FromVec(point));
        }
        // Empty when the points can't make a polygon: collinear, or more than B2_MAX_POLYGON_VERTICES
        auto hull = b2ComputeHull(points.data(), points.size());
        if (hull.count == 0) {
            std::cout << "WARNING: Skipping a tile collider that isn't a valid polygon on " << entity->name << "\n";
            continue;
        }
        auto convex = b2MakePolygon(&hull, 0);
        auto shape = b2CreatePolygonShape(body, &shapeDef, &convex);
        m_tilemapShapes.insert(std::make_pair(shape.index1, entity->id()));
    }
}

hg::Entity* Physics2D::entityForShape(b2ShapeId shape) {
    auto it = m_shapeMap.find(shape.index1);
    if (it == m_shapeMap.end() || !b2Shape_IsValid(shape)) {
        return nullptr;
    }
    auto entity = scene->entities.get(it->second);
    if (!entity) {
        return nullptr;
    }
    // A body's shape is rebuilt when its collider changes, and the index can pass to another body's: only the
    // entity whose shape this exactly is (index and generation) owns it. Tilemap shapes live until the world resets
    if (auto rb = entity->getComponent<RigidBody2D>()) {
        return B2_ID_EQUALS(rb->m_shapeId, shape) ? entity : nullptr;
    }
    return entity->hasComponent<components::Tilemap>() ? entity : nullptr;
}

void Physics2D::onFixedUpdate(double dt) {

    if (!m_running) {
        return;
    }

    utils::Profiler::Start("Physics2D::onFixedUpdate");

    m_elapsedTime += dt;

    m_shapeMap.clear();
    m_shapeMap.insert(m_tilemapShapes.begin(), m_tilemapShapes.end());

    // Everything that can be in a body's collisions this step: entities with a body, and tilemaps
    std::unordered_set<Entity*> live;
    for (const auto& [index, id] : m_tilemapShapes) {
        if (auto entity = scene->entities.get(id)) {
            live.insert(entity);
        }
    }

    // Every body, active or not, so one whose entity was switched off since the last step is taken out of the world
    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        live.insert(entity);
        // Added after play started (spawned, or a component added at runtime): it gets its body here, where it is now
        if (!b2Body_IsValid(rb->m_id)) {
            rb->initialize(m_world);
        }
        bool active = entity->activeInHierarchy();
        rb->setEnabled(active);
        if (active) {
            rb->updateBody();
        }
        m_shapeMap[rb->m_shapeId.index1] = entity->id();
    }, {}, {}, true);

    // Tilemap entities are in the shape map but have no RigidBody2D to record contacts on
    auto addCollision = [](Entity* entity, Entity* other, b2Manifold const& manifold) {
        if (auto rb = entity->getComponent<RigidBody2D>()) {
            rb->m_collisions.insert(std::make_pair(other, manifold));
        }
    };

    auto removeCollision = [](Entity* entity, Entity* other) {
        if (auto rb = entity->getComponent<RigidBody2D>()) {
            rb->m_collisions.erase(other);
        }
    };

    // Box2D v3 recommends 4 sub-steps; 1 gives soft, sinking contacts
    b2World_Step(m_world, dt, 4);

    auto events = b2World_GetContactEvents(m_world);

    for (int i = 0; i < events.beginCount; i++) {
        auto event = events.beginEvents[i];
        auto entityA = entityForShape(event.shapeIdA);
        auto entityB = entityForShape(event.shapeIdB);
        if (!entityA || !entityB) {
            continue;
        }
        addCollision(entityA, entityB, event.manifold);
        addCollision(entityB, entityA, event.manifold);
    }

    for (int i = 0; i < events.endCount; i++) {
        auto event = events.endEvents[i];
        // A destroyed shape (e.g. one rebuilt by RigidBody2D::refreshShape) still reports its contacts ending. Its
        // body's replacement shape may already be touching the same thing, so erasing by entity would drop that
        // (entityForShape is null for a destroyed shape)
        auto entityA = entityForShape(event.shapeIdA);
        auto entityB = entityForShape(event.shapeIdB);
        if (!entityA || !entityB) {
            continue;
        }
        removeCollision(entityA, entityB);
        removeCollision(entityB, entityA);
    }

    // Skipping destroyed shapes above means a removed entity, or one whose body was removed (a death), never has its
    // contacts end, so other bodies kept pointing at it: freed memory once it was removed, which collidingWith()
    // handed out. Drop whatever isn't a body or tilemap any more
    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        std::erase_if(rb->m_collisions, [&](auto const& entry) { return !live.contains(entry.first); });
    }, {}, {}, true);

    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        rb->updateTransform();
    });

    utils::Profiler::End("Physics2D::onFixedUpdate");
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
    m_elapsedTime = 0;
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
    hit.fraction = -1; // Stays negative when the ray hits nothing

    b2World_CastRay(m_world, FromVec(ray.origin), FromVec(ray.direction), filter, _raycast_callback, &hit);

    if (hit.fraction > 0) {
        // Resolved now, not from a pointer saved at the last step: a shot fired after something was removed (a
        // shotgun's first pellet killing what the second hits) handed back freed entities
        hg::Entity* hitEntity = entityForShape(hit.shapeId);
        return Hit {hitEntity, hit.pt, hit.normal, (float)(ray.direction.magnitude() * (1.0 - hit.fraction))};
    }

    return std::nullopt;
}

void Physics2D::onBeforeUpdate() {
    utils::Profiler::Start("Physics2D::onUpdate");
    // Draw bodies between their last two fixed-step positions. This lags up to one step behind but never
    // overshoots, unlike extrapolating from velocity.
    double alpha = scene->game()->fixedAlpha();
    scene->entities.forEach<RigidBody2D>([&](RigidBody2D* rb, Entity* entity) {
        entity->transform.positionOffset = m_running && rb->dynamic ?
            rb->interpolationOffset(alpha).resize<3>() :
            Vec3::Zero();
    });
    utils::Profiler::End("Physics2D::onUpdate");
}

