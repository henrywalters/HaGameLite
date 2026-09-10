//
// Created by henry on 2/17/23.
//
#include "../../../include/hagame/core/component.h"
#include "../../../include/hagame/core/entity.h"

using namespace hg;

nlohmann::json Component::save() {
    nlohmann::json comp;
    comp["id"] = id();
    comp["name"] = className();
    comp["params"] = nlohmann::json::object();
    for (const auto &field: ComponentFactory::GetFields(className())) {
        std::visit(utils::overloaded{
            [&](ComponentFactory::PrimitiveDetails const& details) {
                auto value = details.getter(this);
                comp["params"][field.field] = hg::utils::serializeToJson(value);
            },
            [&](ComponentFactory::JsonDetails const& details) {
                auto value = details.getter(this);
                comp["params"][field.field] = value;
            }
        }, field.details);
    }
    return comp;
}

void Component::load(nlohmann::json const &j) {

}

std::unique_ptr<std::unordered_map<std::string, ComponentFactory::RegisteredComponent>> ComponentFactory::s_components;
std::unique_ptr<std::unordered_map<std::string, std::unordered_map<std::string, ComponentFactory::ComponentField>>> ComponentFactory::s_fields;

std::vector<ComponentFactory::RegisteredComponent> ComponentFactory::GetAll() {
    std::vector<RegisteredComponent> list;
    for (const auto& [name, component] : ComponentMap()) {
        list.push_back(component);
    }
    return list;
}

Component* ComponentFactory::Attach(Entity *entity, std::string componentName) {
    return Get(componentName).attach(entity);
}

std::unordered_map<std::string, ComponentFactory::RegisteredComponent> &ComponentFactory::ComponentMap() {
    if (!s_components) {
        s_components = std::make_unique<std::unordered_map<std::string, RegisteredComponent>>();
    }
    return *s_components;
}

std::unordered_map<std::string, ComponentFactory::ComponentField> &ComponentFactory::Fields(std::string component) {
    if (!s_fields) {
        s_fields = std::make_unique<std::unordered_map<std::string, std::unordered_map<std::string, ComponentField>>>();
    }
    if (s_fields->find(component) == s_fields->end()) {
        s_fields->insert(std::make_pair(component, std::unordered_map<std::string, ComponentField>()));
    }

    return s_fields->at(component);
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

ComponentFactory::ComponentField
ComponentFactory::RegisterField(std::string type, std::string component, std::string fieldName, setter_fn setter, getter_fn getter) {
    ComponentField field;
    field.type = type;
    field.field = fieldName;
    field.details = PrimitiveDetails{
        .setter = setter,
        .getter = getter,
        .number = std::nullopt
    };
    Fields(component).insert(std::make_pair(fieldName, field));
    return field;
}

ComponentFactory::ComponentField
ComponentFactory::RegisterField(std::string type, std::string component, std::string fieldName, setter_fn setter, getter_fn getter, double min, double max, double step) {
    ComponentField field;
    field.type = type;
    field.field = fieldName;
    field.details = PrimitiveDetails{
        .setter = setter,
        .getter = getter,
        .number = NumberSettings{min, max, step}
    };
    Fields(component).insert(std::make_pair(fieldName, field));
    return field;
}

ComponentFactory::ComponentField ComponentFactory::RegisterJsonField(std::string type, std::string component,
    std::string fieldName, json_setter_fn setter, json_getter_fn getter) {
    ComponentField field;
    field.type = type;
    field.field = fieldName;
    field.details = JsonDetails {
        .setter = setter,
        .getter = getter
    };
    Fields(component).insert(std::make_pair(fieldName, field));
    return field;
}

std::vector<ComponentFactory::ComponentField> ComponentFactory::GetFields(std::string component) {
    std::vector<ComponentFactory::ComponentField> fields;

    for (const auto&[name, field] : Fields(component)) {
        fields.push_back(field);
    }

    return fields;
}

utils::variant ComponentFactory::DefaultValue(std::string type) {
    if (type == "int" || type == "float" || type == "double") {
        return 0;
    } else if (type == "bool") {
        return false;
    } else if (type == "string") {
        return "";
    } else {
        throw std::runtime_error("This Type: " + type + " does not have a default value defined");
    }
}
