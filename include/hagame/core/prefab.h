//
// Created by henry on 11/8/24.
//

#ifndef HAGAME2_PREFAB_H
#define HAGAME2_PREFAB_H

#include "../utils/config.h"
#include "../utils/pubsub.h"
#include "hagame/math/transform.h"
#include "component.h"

namespace hg {

    class Entity;

    // A prefab is a blueprint for an Entity Tree.
    class Prefab {
    public:

        Prefab();

        Prefab(std::string path);

        const std::string EXTENSION = "hg_pfb";

        Notifier onBeforeUpdate; // Emitted before updatePrefab changes the definition
        Notifier onUpdate;

        // Given an entity, update the Prefab to match it
        void updatePrefab(hg::Entity* entity);

        // Given the Prefab, update the entity to match it
        void updateEntity(hg::Entity* entity);

        // The parts of an instance that differ from this Prefab: component params, added components and child
        // names / transforms. Children are matched by index. Returns null when the instance matches the Prefab.
        nlohmann::json getOverrides(hg::Entity* instance) const;

        // Apply overrides from getOverrides to an instance that was built from this Prefab
        void applyOverrides(hg::Entity* instance, nlohmann::json const& overrides) const;

        // Save the Prefab to disc
        void save(std::string path);

        // Load the Prefab from disc
        void load(std::string path);

        HG_GET(hg::utils::uuid_t, id)

        // The root entity's transform as authored. Instances don't take it (they're placed by whoever spawns them),
        // so spawners copy what they need, such as the scale
        hg::math::Transform const& rootTransform() const { return m_root.transform; }

    private:

        using FieldValue = std::variant<hg::utils::variant, nlohmann::json>;

        struct Field {
            hg::ComponentFactory::ComponentField field;
            FieldValue value;
        };

        struct PfbComponent {
            hg::ComponentFactory::RegisteredComponent component;
            std::vector<Field> fields;
        };

        // All the raw info needed to construct an Entity
        struct Node {
            std::string name;
            bool active = true;
            hg::math::Transform transform;
            std::vector<PfbComponent> components;
            std::vector<std::unique_ptr<Node>> children;
            Node* parent = nullptr;
        };

        hg::utils::uuid_t m_id;
        Node m_root;

    };
}

#endif //HAGAME2_PREFAB_H
