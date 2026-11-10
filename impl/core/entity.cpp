//
// Created by henry on 3/4/24.
//
#include <hagame/core/scene.h>
#include <hagame/core/entity.h>
#include <hagame/utils/config.h>
#include "hagame/physics/rigidbody2D.h"

using namespace hg;

static int entityCount = 0;

hg::Entity::Entity(hg::utils::uuid_t enttId,
                   entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>> *registry):
        m_enttId(enttId),
        m_registry(registry)
{
    name = "Entity " + std::to_string(entityCount++);
};

hg::Entity::Entity(hg::utils::uuid_t id, hg::utils::uuid_t enttId,
                   entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>> *registry):
        GameObject(id),
        m_enttId(enttId),
        m_registry(registry)
{
    name = "Entity " + std::to_string(entityCount++);
};

hg::Entity *hg::Entity::add() {
    return scene->entities.add(this);
}

void hg::Entity::remove() {
    return scene->entities.remove(this);
}

void Entity::clear() {
    auto childrenSnapshot = children(); // copy — remove() below can safely mutate the real list
    for (const auto& child : childrenSnapshot) {
        static_cast<Entity*>(child)->remove();
    }
    children().clear();
    components().clear();

    for (auto [id, storage] : m_registry->storage()) {
        storage.remove(m_enttId);
    }
}

std::string hg::Entity::toString() const {
    return name;
}

nlohmann::json Entity::save() {
    nlohmann::json j;
    j["name"] = name;
    j["id"] = id();
    j["active"] = active;
    j["transform"] = {
        {"position", transform.position},
        {"scale", transform.scale},
        {"rotation", transform.rotation}
    };

    if (prefab) {
        j["prefab"] = prefab->id();
        auto overrides = prefab->getOverrides(this);
        if (!overrides.is_null()) {
            j["overrides"] = overrides;
        }
    }

    j["children"] = nlohmann::json::array();
    j["components"] = nlohmann::json::array();

    if (!prefab) {
        for (hg::Component *component: components()) {
            j["components"].push_back(component->save());
        }

        for (const auto& child : children()) {
            j["children"].push_back(static_cast<Entity*>(child)->save());
        }
    }

    return j;
}

void Entity::load(nlohmann::json const &j) {
    name = j["name"].get<std::string>();
    setId(j["id"].get<utils::uuid_t>());
    // Scenes saved before entities could be switched off don't say
    active = j.value("active", true);
    transform.position = j["transform"]["position"].get<hg::Vec3>();
    transform.rotation = j["transform"]["rotation"].get<hg::Quat>();
    transform.scale = j["transform"]["scale"].get<hg::Vec3>();

    for (const auto& compJ : j["components"]) {
        auto name = compJ["name"].get<std::string>();
        auto componentDef = ComponentFactory::Get(name);
        auto component = componentDef.attach(this);

        for (const auto& field : ComponentFactory::GetFields(name)) {
            // Fields added to a component after the scene was saved keep their default value
            if (!compJ["params"].contains(field.field)) {
                continue;
            }
            auto value = compJ["params"][field.field];
            std::visit(utils::overloaded{
                [&](ComponentFactory::PrimitiveDetails const& details) {
                    details.setter(component, utils::deserializeFromJson(field.type, value));
                },
                [&](ComponentFactory::JsonDetails const& details) {
                    details.setter(component, value);
                }
            }, field.details);
        }
    }

    // Prefab instances are built from the prefab when added, then their own changes are layered on top
    if (prefab && j.contains("overrides")) {
        prefab->applyOverrides(this, j["overrides"]);
    }
}
