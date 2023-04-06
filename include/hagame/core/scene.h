//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_SCENE_H
#define HAGAME2_SCENE_H

#include "object.h"
#include "entity.h"
#include "script.h"
#include "../utils/random.h"
#include "../utils/config.h"

class b2World;

namespace hg {

    class Game;

    struct SceneDescriptor {
        std::vector<std::string> scripts;
    };

    class Scene : public Object {
    public:

        Scene():
            m_random(std::make_unique<utils::Random>()),
            m_game(nullptr)
        {}

        friend class Game;

        std::vector<std::shared_ptr<Script>> scripts;
        EntityManager entities;

        Game* game() { return m_game; }
        b2World* physics2d() { return m_physics2d; }

        utils::Random* random() { return m_random.get(); }

        void init();

        void activate() {
            for (const auto& script : scripts) {
                script->init();
            }
            onActivate();
        }

        void update(double dt) {
            for (const auto& script : scripts) {
                script->update(dt);
            }
            onUpdate(dt);
        }

        void deactivate() {
            for (const auto& script : scripts) {
                script->close();
            }
            onDeactivate();
        }

        hg::utils::MultiConfig save();
        void load(hg::utils::MultiConfig scene);

    protected:
        std::string toString() const {
            return "Scene<" + std::to_string(id()) + ">";
        }

        virtual void onInit() {}
        virtual void onActivate() {}
        virtual void onDeactivate() {}
        virtual void onUpdate(double dt) {}

    private:

        std::unique_ptr<utils::Random> m_random;
        b2World* m_physics2d;

        Game* m_game;
    };

}

#endif //HAGAME2_SCENE_H
