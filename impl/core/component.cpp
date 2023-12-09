//
// Created by henry on 2/17/23.
//
#include "../../../include/hagame/core/component.h"
#include "../../../include/hagame/core/entity.h"

using namespace hg;

std::unique_ptr<std::unordered_map<std::string, ComponentFactory::RegisteredComponent>> ComponentFactory::s_components;

entt::meta_type hg::GetComponent(std::string id) {
    return entt::resolve(entt::hashed_string{id.c_str()}.value());
}

std::vector<ComponentFactory::RegisteredComponent> ComponentFactory::GetAll() {
    std::vector<RegisteredComponent> list;
    for (const auto& [name, component] : ComponentMap()) {
        list.push_back(component);
    }
    return list;
}

void ComponentFactory::Attach(Entity *entity, std::string componentName) {
    Get(componentName).attach(entity);
}

std::unordered_map<std::string, ComponentFactory::RegisteredComponent> &ComponentFactory::ComponentMap() {
    if (!s_components) {
        s_components = std::make_unique<std::unordered_map<std::string, RegisteredComponent>>();
    }
    return *s_components;
}

ComponentFactory::RegisteredComponent ComponentFactory::Get(std::string name) {
    if (ComponentMap().find(name) == ComponentMap().end()) {
        throw std::runtime_error("Component: " + name + " does not exist");
    }
    return ComponentMap()[name];
}

std::vector<std::string> ComponentFactory::GetCategories() {
    std::vector<std::string> categories;

    for (const auto&[name, component] : ComponentMap()) {
        if (std::find(categories.begin(), categories.end(), component.category) == categories.end()) {
            categories.push_back(component.category);
        }
    }

    return categories;
}

std::vector<ComponentFactory::RegisteredComponent> ComponentFactory::GetCategory(std::string category) {
    std::vector<RegisteredComponent> components;

    for (const auto&[name, component] : ComponentMap()) {
        if (component.category == category) {
            components.push_back(component);
        }
    }

    return components;
}
