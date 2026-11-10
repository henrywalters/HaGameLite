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
#include <optional>
#include "../utils/config.h"
#include "../utils/variant.h"
#include "nlohmann/json.hpp"

namespace hg {

    typedef std::unordered_map<std::string, std::any> ComponentParams;

    class Entity;

    class Component : public virtual Object {
    public:

        // Tells EnTT to leave a hole when a component is removed rather than moving the last one of its type into the
        // gap. Entities keep raw pointers to their components (and other code to the components it uses), which a
        // move would leave pointing at the wrong component or freed memory
        static constexpr bool in_place_delete = true;

        Entity* entity = nullptr;

        void update(double dt) {
            onUpdate(dt);
        }

        void uiUpdate() {
            onUiUpdate();
        }

        void updateParams(ComponentParams params) {
            onUpdateParams(params);
        }

        nlohmann::json save();
        void load(nlohmann::json const& j);

        virtual void onParamChange() {};

    protected:

        OBJECT_NAME(Component)

        template <typename T>
        void setParam(ComponentParams params, std::string key, T& member) {
            if (params.find(key) == params.end()) {
                return;
            }

            member = params[key];
        }

        virtual void onUpdate(double dt) {}
        virtual void onUiUpdate() {}
        virtual void onUpdateParams(ComponentParams params) {}

    private:
    };

    template <typename T>
    concept IsComponent = std::is_base_of<Component, T>::value;

    class ComponentFactory {
    public:

        using attach_fn = std::function<Component*(Entity*)>;
        using remove_fn = std::function<void(Entity*)>;
        using setter_fn = std::function<void(Component*, utils::variant)>;
        using getter_fn = std::function<utils::variant(Component*)>;
        using json_setter_fn = std::function<void(Component*, nlohmann::json const&)>;
        using json_getter_fn = std::function<nlohmann::json(Component*)>;

        struct RegisteredComponent {
            std::string category;
            std::string name;
            attach_fn attach;
            remove_fn remove;
            // A standalone, default-constructed instance. Used to read field defaults, never attached to an entity.
            std::function<std::shared_ptr<Component>()> create;
        };

        struct NumberSettings {
            double min;
            double max;
            double step;
        };

        struct PrimitiveDetails {
            setter_fn setter;
            getter_fn getter;
            std::optional<NumberSettings> number;
        };

        struct JsonDetails {
            json_setter_fn setter;
            json_getter_fn getter;
        };

        using Details = std::variant<PrimitiveDetails, JsonDetails>;

        struct ComponentField {
            std::string type;
            std::string field;
            Details details;
        };

        static utils::variant DefaultValue(std::string type);

        // Register a component in the factory
        template <IsComponent Comp>
        static attach_fn Register(std::string category, std::string className);

        static ComponentField RegisterField(std::string type, std::string component, std::string fieldName, setter_fn setter, getter_fn getter);

        static ComponentField RegisterField(std::string type, std::string component, std::string fieldName, setter_fn setter, getter_fn getter, double min, double max, double step);

        static ComponentField RegisterJsonField(std::string type, std::string component, std::string fieldName, json_setter_fn setter, json_getter_fn getter);

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
        static Component* Attach(Entity* entity, std::string componentName);

    private:

        static std::unordered_map<std::string, RegisteredComponent>& ComponentMap();
        static std::unordered_map<std::string, ComponentField>& Fields(std::string component);

        static std::unique_ptr<std::unordered_map<std::string, std::unordered_map<std::string, ComponentField>>> s_fields;
        static std::unique_ptr<std::unordered_map<std::string, RegisteredComponent>> s_components;

    };

    template<IsComponent Comp>
    ComponentFactory::attach_fn ComponentFactory::Register(std::string category, std::string className) {

        if (className == "Component") {
            throw std::runtime_error("Invalid Component name. Make sure the toClassName method is overwritten. Or use OBJECT_NAME");
        }

        auto attachFunc = [&](auto* entity) {
            return (Component*) entity->template addComponent<Comp>();
        };

        auto removeFunc = [&](auto* entity) {
            return entity->template removeComponent<Comp>();
        };

        RegisteredComponent component;
        component.name = className;
        component.category = category;
        component.attach = attachFunc;
        component.remove = removeFunc;
        component.create = []() { return std::static_pointer_cast<Component>(std::make_shared<Comp>()); };

        ComponentMap().insert(std::make_pair(className, component));
        return attachFunc;
    }
}

#endif //HAGAME2_COMPONENT_H
