//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_ENTITY_H
#define HAGAME2_ENTITY_H

#include <memory>

#include "gameObject.h"
#include <entt/entity/registry.hpp>

namespace hg {

    // An entity is a GameObject that functions within the ECS paradigm
    class Entity : public GameObject {
    public:

        Entity(uint32_t enttId, entt::basic_registry<uint32_t, std::allocator<uint32_t>>* registry):
                m_enttId(enttId),
                m_registry(registry)
        {};

        // Constructs a new instance of the component in memory. Be careful with the returned pointer! Another addComponent call or loss of scope may invalidate the pointer
        template <class T>
        T* addComponent();

        template <class T, class... Args>
        T* addComponent(Args &&... args);

        template <class T>
        void removeComponent();

        template <class T>
        bool hasComponent();

        template <class ...T>
        bool hasComponents();

        template <class T>
        T* getComponent();

        template <class T>
        T* getOrCreateComponent();

        template <class T>
        T* getComponentInChildren();

    protected:
        [[nodiscard]] std::string toString() const override {
            return "Entity<" + std::to_string(id()) + ">";
        }

    private:

        friend class EntityManager;

        uint32_t m_enttId;
        entt::basic_registry<uint32_t, std::allocator<uint32_t>> * m_registry;

    };

    class EntityManager {
    public:

        EntityManager();

        // Instantiate a new entity belonging to the registry
        Entity* add();

        // Instantiate a new child entity belonging to the registry
        Entity* add(Entity*parent);

        // Destroy an entity
        void remove(Entity* entity);

        void forEach(std::function<void(Entity*)> lambda);

        [[nodiscard]] entt::basic_registry<uint32_t, std::allocator<uint32_t>>* registry() const { return m_registry.get(); }

    private:

        std::unique_ptr<entt::basic_registry<uint32_t, std::allocator<uint32_t>>> m_registry;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_enttMap;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> m_idMap;

    };
}

#endif //HAGAME2_ENTITY_H
