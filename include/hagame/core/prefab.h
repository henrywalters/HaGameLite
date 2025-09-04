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

        Notifier onUpdate;

        // Given an entity, update the Prefab to match it
        void updatePrefab(hg::Entity* entity);

        // Given the Prefab, update the entity to match it
        void updateEntity(hg::Entity* entity);

        // Save the Prefab to disc
        void save(std::string path);

        // Load the Prefab from disc
        void load(std::string path);

        HG_GET(hg::utils::uuid_t, id)

    private:

        struct Field {
            hg::ComponentFactory::ComponentField field;
            hg::utils::variant value;
        };

        struct PfbComponent {
            hg::ComponentFactory::RegisteredComponent component;
            std::vector<Field> fields;
        };

        // All the raw info needed to construct an Entity
        struct Node {
            std::string name;
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
