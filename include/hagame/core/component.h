//
// Created by henry on 2/11/23.
//

#ifndef HAGAME2_COMPONENT_H
#define HAGAME2_COMPONENT_H

#include "object.h"
#include <any>
#include <unordered_map>
#include <entt/core/hashed_string.hpp>
#include <entt/core/utility.hpp>
#include <entt/entity/registry.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include "../utils/config.h"

namespace hg {

    typedef std::unordered_map<std::string, std::any> ComponentParams;

    class Entity;

    class Component : public Object {
    public:

        Entity* entity;

        void update(double dt) {
            onUpdate(dt);
        }

        void uiUpdate(double dt) {
            onUiUpdate(dt);
        }

        void updateParams(ComponentParams params) {
            onUpdateParams(params);
        }

        virtual void load(utils::Config* config, std::string section) {}
        virtual void save(utils::Config* config, std::string section) {}

        virtual void onParamChange() {};

    protected:

        OBJECT_NAME(Component)

        template <typename T>
        void setParam(ComponentParams params, std::string key, T& member) {
            if (params.find(key) == params.end()) {
                return;
            }

            member = std::any_cast<T>(params[key]);
        }

        virtual void onUpdate(double dt) {}
        virtual void onUiUpdate(double dt) {}
        virtual void onUpdateParams(ComponentParams params) {}

    private:
    };

    template <typename T>
    concept IsComponent = std::is_base_of<Component, T>::value;

    template <IsComponent T>
    entt::meta_factory<T> RegisterComponent(std::string id) {
        auto factory = entt::meta<T>().type(entt::hashed_string {id.c_str()}.value())
                .template func<&entt::registry::emplace<T>>(entt::hashed_string{(id + "_emplace").c_str()}.value());
        return factory;
    }

    entt::meta_type GetComponent(std::string id);

    class ComponentFactory {
    public:

        using attach_fn = std::function<Component*(Entity*)>;
        using setter_fn = std::function<void(Component*, std::any)>;
        using getter_fn = std::function<std::any(Component*)>;

        struct RegisteredComponent {
            std::string category;
            std::string name;
            attach_fn attach;
        };

        struct ComponentField {
            std::string type;
            std::string field;
            setter_fn setter;
            getter_fn getter;
        };

        // Register a component in the factory
        template <IsComponent Comp>
        static attach_fn Register(std::string category, std::string className);

        static ComponentField RegisterField(std::string type, std::string component, std::string fieldName, setter_fn setter, getter_fn getter);

        // List all the registered components
        static std::vector<RegisteredComponent> GetAll();

        // Get a specific component by name
        static RegisteredComponent Get(std::string name);

        // Get all the registered categories
        static std::vector<std::string> GetCategories();

        // Get all the components in a category
        static std::vector<RegisteredComponent> GetCategory(std::string category);

        static std::vector<ComponentField> GetFields(std::string component);

        // Attach a component by name to an entity
        static void Attach(Entity* entity, std::string componentName);

    private:

        static std::unordered_map<std::string, RegisteredComponent>& ComponentMap();
        static std::unordered_map<std::string, ComponentField>& Fields(std::string component);

        static std::unique_ptr<std::unordered_map<std::string, std::unordered_map<std::string, ComponentField>>> s_fields;
        static std::unique_ptr<std::unordered_map<std::string, RegisteredComponent>> s_components;

    };

    template<IsComponent Comp>
    ComponentFactory::attach_fn ComponentFactory::Register(std::string category, std::string className) {
        auto attachFunc = [&](auto* entity) {
            return (Component*) entity->template addComponent<Comp>();
        };

        RegisteredComponent component;
        component.name = className;
        component.category = category;
        component.attach = attachFunc;

        ComponentMap().insert(std::make_pair(className, component));
        return attachFunc;
    }
}

#endif //HAGAME2_COMPONENT_H
