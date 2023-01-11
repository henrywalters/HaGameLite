//
// Created by henry on 12/28/22.
//

#include "../../../include/hagame/core/entity.h"

template<class T>
T *hg::Entity::addComponent() {
    T* component = &m_registry->emplace<T>(m_enttId);
    return component;
}

template<class T, class... Args>
T *hg::Entity::addComponent(Args &&... args) {
    T* component = &m_registry->emplace<T>(m_enttId, std::forward<Args>(args)...);
    return component;
}


template<class T>
T *hg::Entity::getComponentInChildren() {
    for (auto child : children()) {
        if (((Entity*)child)->hasComponent<T>()) {
            return ((Entity*)child)->getComponent<T>();
        }
    }

    return NULL;
}

template<class T>
T *hg::Entity::getOrCreateComponent() {
    return hasComponent<T>() ? &m_registry->get<T>(m_enttId) : addComponent<T>();
}

template<class T>
T *hg::Entity::getComponent() {
    return hasComponent<T>() ? &m_registry->get<T>(m_enttId) : NULL;
}

template<class... T>
bool hg::Entity::hasComponents() {
    return m_registry->all_of<T...>(m_enttId);
}

template<class T>
bool hg::Entity::hasComponent() {
    return m_registry->all_of<T>(m_enttId);
}

template<class T>
void hg::Entity::removeComponent() {
    m_registry->erase<T>(m_enttId);
}

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

void hg::EntityManager::forEach(std::function<void(Entity * )> lambda) {
    for (const auto &[id, entity] : m_idMap) {
        lambda(entity.get());
    }
}

hg::EntityManager::EntityManager() {
    m_registry = std::make_unique<entt::basic_registry<uint32_t, std::allocator<uint32_t>>>();
}
