//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_GAME_H
#define HAGAME2_GAME_H

#include "object.h"
#include "scene.h"
#include "../utils/stateMachine.h"
#include "../utils/loopingThread.h"
#include "../graphics/renderPipeline.h"
#include "../graphics/window.h"

namespace hg {

    class HG;

class Game : public utils::LoopingThread {
    public:

        friend class HG;

        Game(std::string name):
            m_name(name),
            m_lastTick(utils::Clock::Now()),
            m_elapsedTime(0),
            m_running(true) {
            m_scenes = std::make_unique<utils::StateMachine<Scene>>();
        }

        utils::StateMachine<Scene>* scenes() {
            return m_scenes.get();
        }

        void tick();
        void initialize();
        void destroy();

        HG_GET_SET(bool, running, destroy);

    protected:

        virtual void onInit() {}
        virtual void onBeforeUpdate() {}
        virtual void onUpdate(double dt) {}
        virtual void onAfterUpdate() {}
        virtual void onDestroy() {}

        void onTick() override {
            tick();
        }

        void onAbort() override {
            destroy();
        }

    private:

        bool m_running;

        std::string m_name;
        std::unique_ptr<utils::StateMachine<Scene>> m_scenes;
        long long m_lastTick;
        double m_elapsedTime;

    };
}

#endif //HAGAME2_GAME_H
