//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_GAME_H
#define HAGAME2_GAME_H

#include "object.h"
#include "scene.h"
#include "../utils/stateMachine.h"
#include "../graphics/renderPipeline.h"
#include "../graphics/window.h"

namespace hg {

    class HG;

    class Game : public Object {
    public:

        friend class HG;

        Game(std::string name):
            m_name(name),
            m_lastTick(utils::Clock::Now()),
            m_elapsedTime(0),
            m_running(true) {}

        virtual void onInit() {}
        virtual void onBeforeUpdate() {}
        virtual void onUpdate(double dt) {}
        virtual void onAfterUpdate() {}
        virtual void onDestroy() {}

        utils::StateMachine<Scene>* scenes() {
            return &m_scenes;
        }

        HG_GET_SET(bool, running, destroy);

    protected:
        std::string toString() const {
            return "Game<" + std::to_string(id()) + ">";
        }

    private:

        bool m_running;



        std::string m_name;
        utils::StateMachine<Scene> m_scenes;
        long long m_lastTick;
        double m_elapsedTime;

        void initialize();

        void tick();

        void destroy();
    };
}

#endif //HAGAME2_GAME_H
