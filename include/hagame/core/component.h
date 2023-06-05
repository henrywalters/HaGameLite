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

    protected:

        std::string toString() const override {
            return "Component<" + std::to_string(id()) + ">";
        }

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
}

#endif //HAGAME2_COMPONENT_H
