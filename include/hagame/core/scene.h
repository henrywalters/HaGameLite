//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_SCENE_H
#define HAGAME2_SCENE_H

#include "object.h"
#include "entity.h"
#include "../utils/random.h"

namespace hg {

    class Game;

    class Scene : public Object {
    public:

        Scene():
            m_random(std::make_unique<utils::Random>()),
            m_game(nullptr)
        {}

        friend class Game;

        EntityManager entities;

        virtual void onInit() {}
        virtual void onActivate() {}
        virtual void onDeactivate() {}
        virtual void onUpdate(double dt) {}

        Game* game() { return m_game; }

        utils::Random* random() { return m_random.get(); }

    protected:
        std::string toString() const {
            return "Scene<" + std::to_string(id()) + ">";
        }

    private:

        std::unique_ptr<utils::Random> m_random;

        Game* m_game;
    };
}

#endif //HAGAME2_SCENE_H
