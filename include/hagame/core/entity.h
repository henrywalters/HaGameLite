//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_ENTITY_H
#define HAGAME2_ENTITY_H

#include <memory>
#include <iostream>

#include "gameObject.h"
#include "component.h"
#include <entt/entity/registry.hpp>

namespace hg {

    // An entity is a GameObject that functions within the ECS paradigm
    class Entity : public GameObject {
    public:


        HG_GET(std::vector<Component*>, components);

        bool active = true;

        Entity(uint32_t enttId, entt::basic_registry<uint32_t, std::allocator<uint32_t>>* registry):
                m_enttId(enttId),
                m_registry(registry)
        {
            name = toString();
        };

        entt::meta_any addComponent(std::string id) {
            //&m_registry->emplace<GetComponent(id)::Type>m_enttId);
            auto emplace_fn = GetComponent(id).func(entt::hashed_string{(id + "_emplace").c_str()}.value());
            auto invoked = emplace_fn.invoke({}, entt::forward_as_meta(m_registry), m_enttId);
            //return invoked;
            auto component = invoked.try_cast<Component>();
            std::cout << component << "\n";
            if (component == nullptr) {
                std::cout << "WARNING: Failed to find Component: " << id << " Are you sure its registered?\n";
            } else {
                m_components.push_back(component);
                component->entity = this;
            }

            for (auto&& curr : m_registry->storage()) {
                auto id = curr.first;

                std::cout << id << " = " << "\n";
            }

            return invoked;
            //return component;
        }

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
            return hasComponent<T>() ? &m_registry->get<T>(m_enttId) : NULL;
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


    protected:
        [[nodiscard]] std::string toString() const override {
            return "Entity<" + std::to_string(id()) + ">";
        }

    private:

        friend class EntityManager;

        uint32_t m_enttId;
        entt::basic_registry<uint32_t, std::allocator<uint32_t>> * m_registry;

        std::vector<Component*> m_components;

    };

    class EntityManager {
    public:

        EntityManager() {
            m_registry = std::make_unique<entt::basic_registry<uint32_t, std::allocator<uint32_t>>>();
        }

        void clear() {
            m_registry->clear();
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
            auto enttId = registry()->create();
            auto entity = std::make_shared<hg::Entity>(enttId, registry());
            m_enttMap.insert(std::make_pair(enttId, entity));
            m_idMap.insert(std::make_pair(entity->id(), entity));
            return entity.get();
        }

        // Instantiate a new child entity belonging to the registry
        Entity* add(Entity*parent) {
            auto entity = add();
            parent->addChild(entity);
            return entity;
        }

        // Destroy an entity
        void remove(Entity* entity) {

            m_registry->destroy(entity->m_enttId);
            m_idMap[entity->id()].reset();
            m_enttMap[entity->m_enttId].reset();
            m_idMap.erase(entity->id());
            m_enttMap.erase(entity->m_enttId);

            for (auto child : entity->children()) {
                remove((Entity*) child);
            }

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

        template<class T>
        void forEach(std::function<void(T*, Entity * )> lambda, std::vector<utils::UUID> ignoreEntities = {},
                                        std::vector<std::string> ignoreTags = {}) {
            for (auto enttId : m_registry->view<T>()) {
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

        std::unique_ptr<entt::basic_registry<uint32_t, std::allocator<uint32_t>>> m_registry;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_enttMap;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_idMap;

    };
}

#endif //HAGAME2_ENTITY_H
