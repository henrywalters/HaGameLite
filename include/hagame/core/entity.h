//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_ENTITY_H
#define HAGAME2_ENTITY_H

#include "gameObject.h"
#include <entt/entity/registry.hpp>

namespace hg {

    using registry_t = entt::basic_registry<uint32_t>;

    // An entity is a GameObject that functions within the ECS paradigm
    class Entity : public GameObject {
    public:

        Entity(uint32_t enttId, registry_t* registry):
                m_enttId(enttId),
                m_registry(registry)
        {};

        // Constructs a new instance of the component in memory. Be careful with the returned pointer! Another addComponent call or loss of scope may invalidate the pointer
        template <class T>
        T* addComponent() {
            T* component = &m_registry->emplace<T>(m_enttId);
            return component;
        }

        template <class T, class... Args>
        T* addComponent(Args &&... args) {
            T* component = &m_registry->emplace<T>(m_enttId, std::forward<Args>(args)...);
            return component;
        }

        template <class T>
        void removeComponent() {
            m_registry->erase<T>(m_enttId);
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
        registry_t* m_registry;

    };

    class EntityManager {
    public:

        EntityManager():
            m_registry(std::make_unique<registry_t>())
        {}

        // Instantiate a new entity belonging to the registry
        Entity* add();

        // Instantiate a new child entity belonging to the registry
        Entity* add(Entity*parent);

        // Destroy an entity
        void remove(Entity* entity);


        [[nodiscard]] registry_t* registry() const { return m_registry.get(); }

    private:

        std::unique_ptr<registry_t> m_registry;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_enttMap;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_idMap;

    };
}

#endif //HAGAME2_ENTITY_H
