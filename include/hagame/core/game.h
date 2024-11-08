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

        int fixedTicksPerSecond = 120;

        friend class HG;

        Game(std::string name):
            m_name(name),
            m_lastTick(utils::Clock::Now()),
            m_elapsedTime(0),
            m_dt(0),
            m_running(true) {
            m_scenes = std::make_unique<utils::StateMachine<Scene>>();
        }

        utils::StateMachine<Scene>* scenes() {
            return m_scenes.get();
        }

        void tick();
        void initialize();
        void destroy();

        // Gracefully shutdown after a full game loop to prevent crashes.
        void requestShutdown();

        HG_GET(double, dt);
        HG_GET(double, fixedDt);
        HG_GET_SET(bool, running, destroy);
        HG_GET_SET_SIMPLE(std::string, name);

    protected:

        virtual void onInit() {}
        virtual void onBeforeUpdate() {}
        virtual void onUpdate(double dt) {}
        virtual void onFixedUpdate(double dt) {}
        virtual void onAfterUpdate() {}
        virtual void onDestroy() {}

        void onTick() override {
            tick();
        }

        void onAbort() override {
            destroy();
        }

    private:

        bool m_requestedShutdown = false;
        bool m_running;

        std::string m_name;
        std::unique_ptr<utils::StateMachine<Scene>> m_scenes;
        long long m_lastTick;
        long long m_lastFixedTick;
        double m_elapsedTime;
        double m_dt;
        double m_fixedDt;


    };
}

#endif //HAGAME2_GAME_H
