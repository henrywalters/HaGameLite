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

        GameObject() = default;
        GameObject(utils::uuid_t id):
            Object(id)
        {}

        // Transform relative to the parent object
        math::Transform transform;
        std::string name = "";

        virtual void update(double dt) {}

        HG_GET(GameObject*, parent);
        std::vector<GameObject*>& children() {
            return m_children;
        }

        void addChild(GameObject* child);
        void removeChild(GameObject* child);
        GameObject* getChildByName(std::string name);

        // Get the global position of the game object
        hg::Vec3 position();

        // Get the global scale of the game object
        hg::Vec3 scale();

        // Get the global rotation of the game object
        hg::Quat rotation();

        // Get the global model of the game object
        hg::Mat4 model();

    protected:

        OBJECT_NAME(GameObject)

    private:

        GameObject* m_parent = nullptr;
        std::vector<GameObject*> m_children;

    };
}

#endif //HAGAME2_GAMEOBJECT_H
