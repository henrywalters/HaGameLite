//
// Created by henry on 10/24/23.
//
#include "../../../include/hagame/math/components/collider.h"
#include "../../../include/hagame/math/components/rectCollider.h"
#include "../../../include/hagame/math/components/circleCollider.h"

using namespace hg::math::components;

Collider *Collider::FromEntity(hg::Entity *entity) {
    Collider* collider = entity->getComponent<RectCollider>();
    if (collider) { return collider; }
    collider = entity->getComponent<CircleCollider>();
    if (collider) { return collider; }

    return nullptr;
}
