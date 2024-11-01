//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_ENTITY_H
#define HAGAME2_ENTITY_H

#include <memory>
#include <iostream>

#include "gameObject.h"
#include "component.h"
#include "groupManager.h"
#include <entt/entity/registry.hpp>

namespace hg {

    class Scene;

    // An entity is a GameObject that functions within the ECS paradigm
    class Entity : public GameObject {
    public:

        std::vector<Component*>& components() {
            return m_components;
        }

        Scene* scene;

        bool active = true;

        Entity(utils::uuid_t enttId, entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>>* registry);

        Entity(utils::uuid_t id, utils::uuid_t enttId, entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>>* registry);

        // Constructs a new instance of the component in memory. Be careful with the returned pointer! Another addComponent call or loss of scope may invalidate the pointer
        template <IsComponent T>
        T* addComponent() {
            T* component = &m_registry->emplace<T>(m_enttId);
            component->entity = this;
            m_components.push_back(component);
            return component;
        }

        template <IsComponent T, class... Args>
        T* addComponent(Args &&... args) {
            T* component = &m_registry->emplace<T>(m_enttId, std::forward<Args>(args)...);
            component->entity = this;
            m_components.push_back(component);
            return component;
        }


        template <class T>
        void removeComponent() {
            if (hasComponent<T>()) {
                m_components.erase(std::find(m_components.begin(), m_components.end(), getComponent<T>()));
                m_registry->erase<T>(m_enttId);
            }
        }

        template <class T>
        bool hasComponent() {
            return m_registry->all_of<T>(m_enttId);
        }

        template <class ...T>
        bool hasComponents() {
            return m_registry->all_of<T...>(m_enttId);
        }

        template <class T>
        T* getComponent() {
            return hasComponent<T>() ? &m_registry->get<T>(m_enttId) : nullptr;
        }

        template <class T>
        T* getOrCreateComponent() {
            return hasComponent<T>() ? &m_registry->get<T>(m_enttId) : addComponent<T>();
        }

        template <class T>
        T* getComponentInChildren() {
            for (auto child : children()) {
                if (((Entity*)child)->hasComponent<T>()) {
                    return ((Entity*)child)->getComponent<T>();
                }
            }

            return NULL;
        }

        // Add a child entity
        hg::Entity* add();

        // Remove this entity and all of its children
        void remove();

    protected:
        std::string toString() const override;


    private:

        friend class EntityManager;

        uint32_t m_enttId;
        entt::basic_registry<uint32_t, std::allocator<uint32_t>> * m_registry;

        std::vector<Component*> m_components;

    };

    struct EntityIdentity {
        hg::utils::uuid_t operator()(hg::Entity* data) {
            return data->id();
        }
    };

    class EntityManager {
    public:

        std::shared_ptr<Entity> root;

        GroupManager groups;

        EntityManager(Scene* scene):
            m_scene(scene)
        {
            m_registry = std::make_unique<entt::basic_registry<uint32_t, std::allocator<uint32_t>>>();
            root = createEntity();
            root->name = "root";
        }

        void clear() {
            m_registry->clear();
            m_enttMap.clear();
            m_idMap.clear();
            root = createEntity();
            root->name = "root";
        }

        bool exists(utils::UUID id) {
            return m_idMap.find(id) != m_idMap.end();
        }

        Entity* get(utils::UUID id) {
            if (!exists(id)) {
                return nullptr;
            }

            return m_idMap[id].get();
        }

        // Instantiate a new entity belonging to the registry
        Entity* add() {
            auto entity = createEntity();
            root->addChild(entity.get());
            return entity.get();
        }

        // Instantiate a new entity with a custom ID belonging to the registry
        Entity* add(utils::uuid_t id) {
            auto entity = createEntity(id);
            root->addChild(entity.get());
            return entity.get();
        }

        // Instantiate a new child entity belonging to the registry
        Entity* add(Entity*parent) {
            auto entity = createEntity();
            parent->addChild(entity.get());
            return entity.get();
        }

        // Destroy an entity
        void remove(Entity* entity) {

            for (auto child : entity->children()) {
                remove((Entity*) child);
            }

            groups.removeEntity(entity);

            m_registry->destroy(entity->m_enttId);
            m_idMap[entity->id()].reset();
            m_enttMap[entity->m_enttId].reset();
            m_idMap.erase(entity->id());
            m_enttMap.erase(entity->m_enttId);

            if (entity->parent() != nullptr) {
                entity->parent()->removeChild(entity);
            }
        }

        void forEach(std::function<void(Entity*)> lambda) {
            for (const auto &[id, entity] : m_idMap) {
                if (entity == nullptr) {
                    continue;
                }
                lambda(entity.get());
            }
        }

        template<typename T, typename... Other>
        void forEach(std::function<void(T*, Entity * )> lambda, std::vector<utils::UUID> ignoreEntities = {},
                                        std::vector<std::string> ignoreTags = {}) {
            for (auto enttId : m_registry->view<T, Other...>()) {
                auto entity = m_enttMap[enttId].get();
                if (entity == nullptr) {
                    continue;
                }
                if (!entity->active) continue;
                auto isIgnored = std::find(ignoreEntities.begin(), ignoreEntities.end(), entity->id()) != ignoreEntities.end();
                if (!isIgnored) {
                    lambda(&m_registry->get<T>(enttId), entity);
                }
            }
        }

        [[nodiscard]] entt::basic_registry<uint32_t, std::allocator<uint32_t>>* registry() const { return m_registry.get(); }

    private:

        Scene* m_scene;

        std::unique_ptr<entt::basic_registry<uint32_t, std::allocator<uint32_t>>> m_registry;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_enttMap;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_idMap;

        std::shared_ptr<Entity> createEntity(std::optional<utils::uuid_t> id = std::nullopt) {
            auto enttId = registry()->create();
            auto entity = id.has_value() ? std::make_shared<hg::Entity>(id.value(), enttId, registry()) : std::make_shared<hg::Entity>(enttId, registry());
            entity->scene = m_scene;
            m_enttMap.insert(std::make_pair(enttId, entity));
            m_idMap.insert(std::make_pair(entity->id(), entity));
            return entity;
        }

    };
}

#endif //HAGAME2_ENTITY_H
