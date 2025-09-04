//
// Created by henry on 11/8/24.
//

#include <hagame/core/prefab.h>
#include <hagame/core/entity.h>
#include <hagame/utils/config.h>
#include <hagame/utils/file.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

hg::Prefab::Prefab():
    m_id(hg::utils::UUID::Generate())
{}

hg::Prefab::Prefab(std::string path) {
    load(path);
}

void hg::Prefab::updatePrefab(hg::Entity *rootEntity) {

    std::function<void(hg::Entity*, Node*)> createNode = [&](hg::Entity* entity, Node* node) {

        entity->prefab = this;

        // Capture the original entity ID but don't count on having it in when instantiated
        node->transform = entity->transform;
        node->name = entity->name;

        for (const auto& component: entity->components()) {
            PfbComponent newComponent;
            newComponent.component = ComponentFactory::Get(component->className());

            for (const auto& field : ComponentFactory::GetFields(component->className())) {
                Field newField;
                newField.field = field;
                newField.value = field.getter(component);
                newComponent.fields.push_back(newField);
            }

            node->components.push_back(newComponent);
        }

        for (const auto& child : entity->children()) {
            node->children.push_back(std::make_unique<Node>());
            node->children.back()->parent = node;
            createNode(static_cast<hg::Entity*>(child), node->children.back().get());
        }
    };

    m_root.children.clear();
    createNode(rootEntity, &m_root);

    onUpdate.emit();
}

void hg::Prefab::updateEntity(hg::Entity *root) {
    root->clear();

    std::function<void(hg::Entity*, Node*)> loadEntity = [&](hg::Entity* entity, Node* node) {

        if (node->parent) { // Don't modify the name and transform or root entity instances
            entity->name = node->name;
            entity->transform = node->transform;
        }

        for (const auto& component : node->components) {
            auto newComp = component.component.attach(entity);
            for (const auto& field : component.fields) {
                field.field.setter(newComp, field.value);
            }
        }

        for (const auto& child : node->children) {
            loadEntity(entity->add(), child.get());
        }
    };

    loadEntity(root, &m_root);
}

void hg::Prefab::save(std::string path) {
    std::function<json(Node*)> saveNode = [&](Node* node) {

        json object;

        object["transform"] = {
            {"position", node->transform.position.vector},
            {"scale", node->transform.scale.vector},
            {"rotation", node->transform.rotation.vector},
        };

        object["name"] = node->name;

        std::vector<json> components;

        for (const auto& component : node->components) {
            json fields;
            for (const auto& field : component.fields) {
                fields[field.field.field] = hg::utils::serialize(field.value);
            }
            components.push_back({
                {"name", component.component.name},
                {"fields", fields},
            });
        }

        object["components"] = components;

        std::vector<json> children;

        for (const auto& child : node->children) {
            children.push_back(saveNode(child.get()));
        }

        object["children"] = children;

        return object;
    };

    json output;
    output["id"] = m_id;
    output["entities"] = saveNode(&m_root);
    hg::utils::f_write(path, output.dump(4));
}

void hg::Prefab::load(std::string path) {

    m_root.children.clear();
    m_root.components.clear();

    std::function<void(Node*, json)> loadNode = [&](Node* node, json object) {
        // Capture the original entity ID but don't count on having it in when instantiated

        for (int i = 0; i < 4; i++) {
            node->transform.rotation.vector[i] = object["transform"]["rotation"][i].template get<float>();
            if (i < 3) {
                node->transform.position.vector[i] = object["transform"]["position"][i].template get<float>();
                node->transform.scale.vector[i] = object["transform"]["scale"][i].template get<float>();
            }
        }
        node->name = object["name"];

        for (const auto& component : object["components"]) {
            PfbComponent newComponent;
            newComponent.component = ComponentFactory::Get(component["name"]);

            for (const auto& field : ComponentFactory::GetFields(component["name"])) {
                Field newField;
                newField.field = field;
                if (component["fields"].contains(field.field)) {
                    newField.value = hg::utils::deserialize(field.type, component["fields"][field.field]);
                } else {
                    newField.value = ComponentFactory::DefaultValue(field.type);
                }
                newComponent.fields.push_back(newField);
            }

            node->components.push_back(newComponent);
        }

        for (const auto& childObj : object["children"]) {
            node->children.push_back(std::make_unique<Node>());
            node->children.back()->parent = node;
            loadNode(node->children.back().get(), childObj);
        }
    };

    auto object = json::parse(hg::utils::f_read(path));

    m_id = object["id"].get<hg::utils::uuid_t>();

    loadNode(&m_root, object["entities"]);

}

