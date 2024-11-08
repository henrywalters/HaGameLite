//
// Created by henry on 11/2/24.
//
#include "../../../include/hagame/physics/world2D.h"

using namespace hg;
using namespace hg::physics;

#define B2VEC(vec) b2Vec2 {vec[0], vec[1] }

hg::physics::World2D::World2D(hg::Vec2 gravity) :
    m_gravity(gravity)
{
    auto worldDef = b2DefaultWorldDef();
    worldDef.gravity = B2VEC(gravity);
    m_world = b2CreateWorld(&worldDef);
}

World2D::~World2D() {
    b2DestroyWorld(m_world);
}

b2BodyId World2D::addRectBody(Vec2 pos, Vec2 size) {
    b2BodyDef def = b2DefaultBodyDef();
    def.position = B2VEC(pos);
    b2BodyId id = b2CreateBody(m_world, &def);
    b2Polygon box = b2MakeBox(size[0] / 2, size[1] / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(id, &shapeDef, &box);
    return id;
}

void World2D::step(double dt) {
    b2World_Step(m_world, dt, 4);
}

b2BodyId World2D::addDynamicBody(Vec2 pos, Vec2 size, float mass, float density, float friction) {
    b2BodyDef def = b2DefaultBodyDef();
    def.type = b2_dynamicBody;
    def.position = B2VEC(pos);
    b2BodyId id = b2CreateBody(m_world, &def);
    b2Polygon box = b2MakeBox(size[0] / 2, size[1] / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.friction = friction;
    shapeDef.density = density;
    b2CreatePolygonShape(id, &shapeDef, &box);
    return id;
}

void World2D::clear() {

}

void World2D::syncTransform(math::Transform &transform, b2BodyId bodyId) {
    auto pos = b2Body_GetPosition(bodyId);
    transform.position = Vec3(pos.x, pos.y, 0);
    transform.rotation = Quat(b2Rot_GetAngle(b2Body_GetRotation(bodyId)), Vec3::Face());
}


