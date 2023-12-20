//
// Created by henry on 8/23/23.
//

#ifndef HAGAME2_SYSTEM_H
#define HAGAME2_SYSTEM_H

#include <vector>
#include <type_traits>
#include <memory>
#include <unordered_map>
#include "../utils/uuid.h"

namespace hg {

    class Scene;

    class System {
    public:

        static hg::utils::uuid_t id;

        bool active = true;

        Scene *scene;

        virtual void onInit() {}

        virtual void onBeforeUpdate() {}

        virtual void onUpdate(double dt) {}

        virtual void onFixedUpdate(double dt) {}

        virtual void onAfterUpdate() {}

        virtual void onRender(double dt) {}

    };

    template<typename T>
    concept IsSystem = std::is_base_of<System, T>::value;

    class SystemFactory {
    public:

        struct RegisteredSystem {
            size_t hashCode;
            std::string category;
            std::string name;
        };

        // Register a new system
        template <IsSystem _System>
        static RegisteredSystem Register(std::string category, std::string className);

        static std::vector<RegisteredSystem> All();

        static RegisteredSystem Get(std::string name);

    private:

        static std::unordered_map<std::string, RegisteredSystem>& SystemMap();
        static std::unique_ptr<std::unordered_map<std::string, RegisteredSystem>> s_systems;
    };

    template<IsSystem _System>
    SystemFactory::RegisteredSystem SystemFactory::Register(std::string category, std::string className) {

        RegisteredSystem system;
        system.name = className;
        system.category = category;
        system.hashCode = typeid(_System).hash_code();

        SystemMap().insert(std::make_pair(className, system));

        return system;
    }
}

#endif //HAGAME2_SYSTEM_H
