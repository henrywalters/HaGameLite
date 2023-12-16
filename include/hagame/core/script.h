//
// Created by henry on 2/11/23.
//

#ifndef HAGAME2_SCRIPT_H
#define HAGAME2_SCRIPT_H

#include <dlfcn.h>
#include "object.h"
#include "../utils/file.h"

namespace hg {

    class Scene;

    enum class ScriptType {
        Cpp,
    };

    struct ScriptDef {
        ScriptType type;
        std::string name;
        std::string path;
    };

    class Script : public Object {
    public:

        friend class Scene;

        hg::Scene* scene() const {
            return m_scene;
        }

        void init() {
            onInit();
        }

        void update(double dt) {
            onUpdate(dt);
        }

        void close() {
            onClose();
        }

    protected:

        OBJECT_NAME(Script)

        virtual void onInit() {}
        virtual void onUpdate(double dt) {}
        virtual void onClose() {}

        hg::Scene* m_scene;

    };

    template <typename T>
    concept IsScript = std::is_base_of<Script, T>::value;

}

#endif //HAGAME2_SCRIPT_H
