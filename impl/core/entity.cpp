//
// Created by henry on 12/28/22.
//

#include "../../../include/hagame/core/entity.h"

hg::Entity* hg::EntityManager::add() {
    auto enttId = registry()->create();
    auto entity = std::make_shared<hg::Entity>(enttId, registry());
    m_enttMap.insert(std::make_pair(enttId, entity));
    m_idMap.insert(std::make_pair(entity->id(), entity));
    return entity.get();
}

hg::Entity *hg::EntityManager::add(hg::Entity *parent) {
    auto entity = add();
    parent->addChild(entity);
    return entity;
}

void hg::EntityManager::remove(hg::Entity *entity) {
    for (auto child : entity->children()) {
        remove((Entity*) child);
    }

    if (entity->parent() != nullptr) {
        entity->parent()->removeChild(entity);
    }

    m_enttMap[entity->m_enttId].reset();
    m_idMap[entity->id()].reset();
    m_registry->destroy(entity->m_enttId);
}
