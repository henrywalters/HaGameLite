//
// Created by henry on 8/23/23.
//

#ifndef HAGAME2_SYSTEM_H
#define HAGAME2_SYSTEM_H

#include <vector>
#include <type_traits>
#include <memory>

namespace hg {

    class Scene;

    class System {
    public:

        bool active = true;

        Scene *scene;

        virtual void onInit() {}

        virtual void onBeforeUpdate() {}

        virtual void onUpdate(double dt) {}

        virtual void onFixedUpdate(double dt) {}

        virtual void onAfterUpdate() {}

    };

    template<typename T>
    concept IsSystem = std::is_base_of<System, T>::value;

}

#endif //HAGAME2_SYSTEM_H
