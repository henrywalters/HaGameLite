//
// Created by henry on 12/23/22.
//

#ifndef HAGAME2_GAMEOBJECT_H
#define HAGAME2_GAMEOBJECT_H

#include "object.h"
#include "../math/transform.h"

namespace hg {

    // For simple games, a GameObject can be used as a basis for functional objects, rather than an Entity which requires components.
    class GameObject : public Object {
    public:

        math::Transform transform;
        std::string name = "";

        virtual void update(double dt) {}

        HG_GET(GameObject*, parent);
        HG_GET(std::vector<GameObject*>, children);

        void addChild(GameObject* child);
        void removeChild(GameObject* child);

    protected:
        [[nodiscard]] std::string toString() const override {
            return "GameObject<" + std::to_string(id()) + ">";
        }

    private:

        GameObject* m_parent = nullptr;
        std::vector<GameObject*> m_children;

    };
}

#endif //HAGAME2_GAMEOBJECT_H
