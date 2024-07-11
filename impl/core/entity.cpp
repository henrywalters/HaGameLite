//
// Created by henry on 3/4/24.
//
#include "../../../include/hagame/core/entity.h"
#include "../../../include/hagame/core/scene.h"

hg::Entity *hg::Entity::add() {
    return scene->entities.add(this);
}

void hg::Entity::remove() {
    return scene->entities.remove(this);
}
