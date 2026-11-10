//
// Created by henry on 11/8/24.
//

#include <hagame/core/prefab.h>
#include <hagame/core/entity.h>
#include <hagame/utils/config.h>
#include <hagame/utils/file.h>

#include <nlohmann/json.hpp>

#include "hagame/core/assets.h"
#include "hagame/utils/serialization.h"

using json = nlohmann::json;

hg::Prefab::Prefab():
    m_id(hg::utils::UUID::Generate())
{}

hg::Prefab::Prefab(std::string path) {
    load(path);
}

void hg::Prefab::updatePrefab(hg::Entity *rootEntity) {

    onBeforeUpdate.emit();

    std::function<void(hg::Entity*, Node*)> createNode = [&](hg::Entity* entity, Node* node) {

        // Only the root is an instance. Marking children too makes them save as whole-prefab instances.
        if (node == &m_root) {
            entity->prefab = this;
        }

        // Capture the original entity ID but don't count on having it in when instantiated
        node->transform = entity->transform;
        node->name = entity->name;
        node->active = entity->active;
        node->components.clear();
        node->children.clear();

        for (const auto& component: entity->components()) {
            PfbComponent newComponent;
            newComponent.component = ComponentFactory::Get(component->className());

            for (const auto& field : ComponentFactory::GetFields(component->className())) {
                Field newField;
                newField.field = field;
                std::visit(utils::overloaded{
                    [&](ComponentFactory::PrimitiveDetails const& details) {
                        newField.value = details.getter(component);
                    },
                    [&](ComponentFactory::JsonDetails const& details) {
                        newField.value = details.getter(component);
                    }
                }, field.details);

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
            entity->active = node->active;
        }

        for (const auto& component : node->components) {
            auto newComp = component.component.attach(entity);
            for (const auto& field : component.fields) {
                try {
                    std::visit(utils::overloaded{
                        [&](ComponentFactory::PrimitiveDetails const& details) {
                            details.setter(newComp, std::get<utils::variant>(field.value));
                        },
                        [&](ComponentFactory::JsonDetails const& details) {
                            details.setter(newComp, std::get<nlohmann::json>(field.value));
                        }
                    }, field.field.details);
                } catch (...) {
                    std::cout << "Missing field in prefab: " << field.field.field << "\n";
                }
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
        object["active"] = node->active;

        std::vector<json> components;

        for (const auto& component : node->components) {
            json fields;
            for (const auto& field : component.fields) {
                std::visit(utils::overloaded{
                    [&](ComponentFactory::PrimitiveDetails const& details) {
                        fields[field.field.field] = hg::utils::serialize(std::get<utils::variant>(field.value));
                    },
                    [&](ComponentFactory::JsonDetails const& details) {
                        fields[field.field.field] = std::get<nlohmann::json>(field.value);
                    }
                }, field.field.details);
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
        node->active = object.value("active", true);

        for (const auto& component : object["components"]) {
            PfbComponent newComponent;
            newComponent.component = ComponentFactory::Get(component["name"]);

            // Fields added to the component after the prefab was saved take the component's own default,
            // not the type's zero value (e.g. gravityScale 1, not 0)
            std::shared_ptr<hg::Component> defaults;

            for (const auto& field : ComponentFactory::GetFields(component["name"])) {
                Field newField;
                newField.field = field;
                if (component["fields"].contains(field.field)) {
                    newField.value = hg::utils::deserialize(field.type, component["fields"][field.field]);
                } else {
                    if (!defaults) {
                        defaults = newComponent.component.create();
                    }
                    std::visit(utils::overloaded{
                        [&](ComponentFactory::PrimitiveDetails const& details) {
                            newField.value = details.getter(defaults.get());
                        },
                        [&](ComponentFactory::JsonDetails const& details) {
                            newField.value = details.getter(defaults.get());
                        }
                    }, field.details);
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


namespace {
    json fieldToJson(hg::ComponentFactory::ComponentField const& field, hg::Component* component) {
        json value;
        std::visit(hg::utils::overloaded{
            [&](hg::ComponentFactory::PrimitiveDetails const& details) {
                value = hg::utils::serializeToJson(details.getter(component));
            },
            [&](hg::ComponentFactory::JsonDetails const& details) {
                value = details.getter(component);
            }
        }, field.details);
        return value;
    }

    json transformToJson(hg::math::Transform const& transform) {
        return {
            {"position", transform.position},
            {"rotation", transform.rotation},
            {"scale", transform.scale},
        };
    }
}

json hg::Prefab::getOverrides(hg::Entity *instance) const {
    std::function<json(hg::Entity*, const Node*)> diffNode = [&](hg::Entity* entity, const Node* node) -> json {
        json out = json::object();

        if (node != &m_root) { // The root's name and transform belong to the instance, not the prefab
            if (entity->name != node->name) {
                out["name"] = entity->name;
            }
            auto transform = transformToJson(entity->transform);
            if (transform != transformToJson(node->transform)) {
                out["transform"] = transform;
            }
        }

        json components = json::array();
        for (const auto& component : entity->components()) {
            auto name = component->className();
            auto pfbComponent = std::find_if(node->components.begin(), node->components.end(), [&](const PfbComponent& c) {
                return c.component.name == name;
            });

            json params = json::object();
            for (const auto& field : ComponentFactory::GetFields(name)) {
                auto value = fieldToJson(field, component);
                if (pfbComponent != node->components.end()) {
                    auto pfbField = std::find_if(pfbComponent->fields.begin(), pfbComponent->fields.end(), [&](const Field& f) {
                        return f.field.field == field.field;
                    });
                    if (pfbField != pfbComponent->fields.end()) {
                        json pfbValue = std::holds_alternative<nlohmann::json>(pfbField->value) ?
                                std::get<nlohmann::json>(pfbField->value) :
                                hg::utils::serializeToJson(std::get<hg::utils::variant>(pfbField->value));
                        if (pfbValue == value) {
                            continue;
                        }
                    }
                }
                params[field.field] = value;
            }

            // Components the prefab doesn't have are kept whole, even if they have no fields
            if (!params.empty() || pfbComponent == node->components.end()) {
                components.push_back({{"name", name}, {"params", params}});
            }
        }
        if (!components.empty()) {
            out["components"] = components;
        }

        json children = json::array();
        bool anyChild = false;
        auto entityChildren = entity->children();
        for (size_t i = 0; i < entityChildren.size() && i < node->children.size(); i++) {
            auto childJ = diffNode(static_cast<hg::Entity*>(entityChildren[i]), node->children[i].get());
            anyChild = anyChild || !childJ.is_null();
            children.push_back(childJ);
        }
        if (anyChild) {
            out["children"] = children;
        }

        return out.empty() ? json() : out;
    };

    return diffNode(instance, &m_root);
}

void hg::Prefab::applyOverrides(hg::Entity *instance, json const &overrides) const {
    std::function<void(hg::Entity*, json const&)> applyNode = [&](hg::Entity* entity, json const& j) {
        if (j.is_null()) {
            return;
        }

        if (j.contains("name")) {
            entity->name = j["name"].get<std::string>();
        }

        if (j.contains("transform")) {
            entity->transform.position = j["transform"]["position"].get<hg::Vec3>();
            entity->transform.rotation = j["transform"]["rotation"].get<hg::Quat>();
            entity->transform.scale = j["transform"]["scale"].get<hg::Vec3>();
        }

        if (j.contains("components")) {
            for (const auto& compJ : j["components"]) {
                auto name = compJ["name"].get<std::string>();
                hg::Component* component = nullptr;
                for (const auto& existing : entity->components()) {
                    if (existing->className() == name) {
                        component = existing;
                        break;
                    }
                }
                if (!component) {
                    component = ComponentFactory::Attach(entity, name);
                }

                for (const auto& field : ComponentFactory::GetFields(name)) {
                    if (!compJ["params"].contains(field.field)) {
                        continue;
                    }
                    auto const& value = compJ["params"][field.field];
                    try {
                        std::visit(utils::overloaded{
                            [&](ComponentFactory::PrimitiveDetails const& details) {
                                details.setter(component, hg::utils::deserializeFromJson(field.type, value));
                            },
                            [&](ComponentFactory::JsonDetails const& details) {
                                details.setter(component, value);
                            }
                        }, field.details);
                    } catch (std::exception const& e) {
                        std::cout << "Failed to apply prefab override " << name << "." << field.field << ": " << e.what() << "\n";
                    }
                }
            }
        }

        if (j.contains("children")) {
            auto entityChildren = entity->children();
            for (size_t i = 0; i < entityChildren.size() && i < j["children"].size(); i++) {
                applyNode(static_cast<hg::Entity*>(entityChildren[i]), j["children"][i]);
            }
        }
    };

    applyNode(instance, overrides);
}
