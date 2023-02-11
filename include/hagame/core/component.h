//
// Created by henry on 2/11/23.
//

#ifndef HAGAME2_COMPONENT_H
#define HAGAME2_COMPONENT_H

#include "object.h"
#include <any>

namespace hg {

    typedef std::unordered_map<std::string, std::any> ComponentParams;

    class Component : public Object {
    public:

        void update(double dt) {
            onUpdate(dt);
        }

        void uiUpdate(double dt) {
            onUiUpdate(dt);
        }

        void updateParams(ComponentParams params) {
            onUpdateParams(params);
        }

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
}

#endif //HAGAME2_COMPONENT_H
