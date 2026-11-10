//
// Created by henry on 12/27/22.
//

#ifndef HAGAME2_ENTITY_H
#define HAGAME2_ENTITY_H

#include <format>
#include <memory>
#include <iostream>

#include "gameObject.h"
#include "component.h"
#include "groupManager.h"
#include "prefab.h"
#include <entt/entity/registry.hpp>

#include "hagame/utils/helpers.h"
#include "hagame/utils/serialization.h"
#include "nlohmann/json.hpp"

namespace hg {

    class Scene;

    // An entity is a GameObject that functions within the ECS paradigm
    class Entity : public GameObject {
    public:

        Prefab* prefab = nullptr;
        
        std::vector<Component*>& components() {
            return m_components;
        }

        Scene* scene;

        // Inactive entities, and everything under them, are left out of EntityManager::forEach, so the systems
        // that go through components (rendering, physics, audio, events) pass them over. Finding one by name still
        // works, e.g. to switch it back on
        bool active = true;

        // Whether this entity and every one of its parents is active
        bool activeInHierarchy() {
            for (GameObject* node = this; node != nullptr; node = node->parent()) {
                if (!static_cast<Entity*>(node)->active) {
                    return false;
                }
            }
            return true;
        }

        Entity(utils::uuid_t enttId, entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>>* registry);

        Entity(utils::uuid_t id, utils::uuid_t enttId, entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>>* registry);

        int getDepth() {
            int depth = 0;
            auto node = static_cast<hg::GameObject*>(this);
            while (node->parent()) {
                depth++;
                node = node->parent();
            }
            return depth;
        }

        void refresh() {
            if (!prefab) {
                return;
            }
            clear();
            prefab->updateEntity(this);
        }

        // Constructs a new instance of the component in memory. Be careful with the returned pointer! Another addComponent call or loss of scope may invalidate the pointer
        template <IsComponent T>
        T* addComponent() {
            if (hasComponent<T>()) {
                return getComponent<T>();
            }
            T* component = &m_registry->emplace<T>(m_enttId);
            component->entity = this;
            m_components.push_back(component);
            return component;
        }

        template <IsComponent T, class... Args>
        T* addComponent(Args &&... args) {
            if (hasComponent<T>()) {
                return getComponent<T>();
            }
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
            auto comp = getComponent<T>();
            if (comp) return comp;

            for (auto child : children()) {
                auto comp = static_cast<Entity*>(child)->getComponentInChildren<T>();
                if (comp) return comp;
            }

            return nullptr;
        }

        // Add a child entity
        hg::Entity* add();

        // Clear all components and children
        void clear();

        // Remove this entity and all of its children
        void remove();

        nlohmann::json save();

        void load(nlohmann::json const& j);

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

        std::vector<Entity*> getAllByName(std::string const& name) {
            std::vector<Entity*> out;
            for (const auto& [id, entity] : m_idMap) {
                if (entity->name == name) {
                    out.push_back(entity.get());
                }
            }
            return out;
        }

        Entity* getFirstByName(std::string const& name, bool throwExceptionIfMissing = false) {
            Entity* out = nullptr;
            for (const auto& [id, entity] : m_idMap) {
                if (entity->name == name) {
                    out = entity.get();
                    break;
                }
            }

            if (throwExceptionIfMissing && !out) {
                throw std::runtime_error(std::format("Entity '{}' does not exist", name));
            }

            return out;
        }

        std::vector<Entity*> getAllByName(Entity* parent, std::string const& name) {
            std::vector<Entity*> out;
            for (const auto& child : parent->children()) {
                auto childEntity = static_cast<Entity*>(child);
                if (childEntity->name == name) {
                    out.push_back(childEntity);
                }
            }
            return out;
        }

        Entity* getFirstByName(Entity* parent, std::string const& name, bool throwExceptionIfMissing = false) {
            Entity* out = nullptr;
            for (const auto& child : parent->children()) {
                auto childEntity = static_cast<Entity*>(child);
                if (childEntity->name == name) {
                    out = childEntity;
                    break;
                }
            }

            if (throwExceptionIfMissing && !out) {
                throw std::runtime_error(std::format("Entity '{}' does not exist", name));
            }

            return out;
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

        // New instances take the prefab root's scale (where they go is up to the caller). Otherwise they start at 1,
        // and saving the prefab back from one writes 1 over the authored scale
        Entity* add(Prefab* prefab) {
            auto entity = createEntity();
            linkPrefab(entity.get(), prefab);
            entity->transform.scale = prefab->rootTransform().scale;
            root->addChild(entity.get());
            return entity.get();
        }

        Entity* add(Entity*parent, Prefab* prefab) {
            auto entity = createEntity();
            linkPrefab(entity.get(), prefab);
            entity->transform.scale = prefab->rootTransform().scale;
            parent->addChild(entity.get());
            return entity.get();
        }

        // Make the entity an instance of the prefab and rebuild it whenever the prefab changes.
        // Pass rebuild = false when the entity already matches the prefab (e.g. it was just saved as one).
        void linkPrefab(Entity* entity, Prefab* prefab, bool rebuild = true) {
            entity->prefab = prefab;
            if (rebuild) {
                entity->refresh();
            }

            // Prefabs outlive scenes, so the listener must not keep the entity alive: once the entity is removed or
            // the scene is cleared its entt id is recycled, and refreshing it would write components onto another entity.
            std::weak_ptr<Entity> weakEntity = m_enttMap.at(entity->m_enttId);

            // Keep the instance's own changes when the prefab is updated: diff against the old definition,
            // rebuild from the new one, then re-apply the diff
            auto overrides = std::make_shared<nlohmann::json>();
            prefab->onBeforeUpdate.subscribe([weakEntity, prefab, overrides]() {
                auto instance = weakEntity.lock();
                if (instance && instance->prefab == prefab) {
                    *overrides = prefab->getOverrides(instance.get());
                }
            });
            prefab->onUpdate.subscribe([weakEntity, prefab, overrides]() {
                auto instance = weakEntity.lock();
                if (instance && instance->prefab == prefab) {
                    instance->refresh();
                    prefab->applyOverrides(instance.get(), *overrides);
                }
                *overrides = nullptr;
            });
        }

        Entity* duplicate(Entity* entity) {
            std::function<Entity *(Entity *, Entity *)> addNode = [&](Entity *og, Entity *parent) {
                hg::Entity *entity = parent ? parent->add() : add();
                entity->transform = og->transform;
                entity->name = !parent ? "Copy of " + og->name : og->name;
                for (const auto &component: og->components()) {
                    auto newComponent = ComponentFactory::Attach(entity, component->className());
                    for (const ComponentFactory::ComponentField &field: ComponentFactory::GetFields(
                            component->className())) {
                        std::visit(utils::overloaded{
                            [&](ComponentFactory::PrimitiveDetails const& details) {
                                details.setter(newComponent, details.getter(component));
                            },
                            [&](ComponentFactory::JsonDetails const& details) {
                                details.setter(newComponent, details.getter(component));
                            }
                        }, field.details);

                    }
                }

                for (auto &child: og->children()) {
                    addNode(static_cast<Entity *>(child), entity);
                }

                return entity;
            };

            return addNode(entity, static_cast<Entity *>(entity->parent()));
        }


        // Entity* add(Entity*parent, const hg::utils::MultiConfig& config)

        // Destroy an entity
        void remove(Entity* entity) {

            // A copy: removing each child takes it out of this list
            auto children = entity->children();
            for (auto child : children) {
                remove((Entity*) child);
            }

            groups.removeEntity(entity);

            // While the entity is still alive: dropping the maps' references below destroys it
            if (entity->parent() != nullptr) {
                entity->parent()->removeChild(entity);
            }

            auto id = entity->id();
            auto enttId = entity->m_enttId;
            m_registry->destroy(enttId);
            m_idMap.erase(id);
            m_enttMap.erase(enttId);
        }

        // Every active entity (see Entity::active), or every entity with includeInactive
        void forEach(std::function<void(Entity*)> lambda, bool includeInactive = false) {
            for (const auto &[id, entity] : m_idMap) {
                if (entity == nullptr || (!includeInactive && !entity->activeInHierarchy())) {
                    continue;
                }
                lambda(entity.get());
            }
        }

        template<typename T, typename... Other>
        // Every active entity with the components (see Entity::active), or every one with includeInactive
        void forEach(std::function<void(T*, Entity * )> lambda, std::vector<utils::UUID> ignoreEntities = {},
                                        std::vector<std::string> ignoreTags = {}, bool includeInactive = false) {
            for (auto enttId : m_registry->view<T, Other...>()) {
                auto entity = m_enttMap[enttId].get();
                if (entity == nullptr) {
                    continue;
                }
                if (!includeInactive && !entity->activeInHierarchy()) continue;
                auto isIgnored = std::find(ignoreEntities.begin(), ignoreEntities.end(), entity->id()) != ignoreEntities.end();
                if (!isIgnored) {
                    lambda(&m_registry->get<T>(enttId), entity);
                }
            }
        }

        void flushIDs() {
            m_idMap.clear();
            for (const auto& [enttId, entity] : m_enttMap) {
                m_idMap.insert(std::make_pair(entity->id(), entity));
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
