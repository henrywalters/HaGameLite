//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_SCENE_H
#define HAGAME2_SCENE_H

#include "object.h"
#include "entity.h"

namespace hg {

    class Game;

    class Scene : public Object {
    public:

        friend class Game;

        EntityManager entities;

        virtual void onInit() {}
        virtual void onActivate() {}
        virtual void onDeactivate() {}
        virtual void onUpdate(double dt) {}

        Game* game() { return m_game; }

    protected:
        std::string toString() const {
            return "Scene<" + std::to_string(id()) + ">";
        }

    private:

        Game* m_game;
    };
}

#endif //HAGAME2_SCENE_H
