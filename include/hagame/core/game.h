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

        Game(std::string name, graphics::Resolution resolution = graphics::HD):
            m_name(name),
            m_resolution(resolution),
            m_lastTick(utils::Clock::Now()),
            m_elapsedTime(0) {}

        virtual void onResize(Vec2i size) {}
        virtual void onInit() {}
        virtual void onUpdate(double dt) {}
        virtual void onDestroy() {}

        graphics::Window* window() { return m_window; }

    protected:
        std::string toString() const {
            return "Game<" + std::to_string(id()) + ">";
        }

    private:
        std::string m_name;
        graphics::Resolution m_resolution;
        utils::StateMachine<Scene> m_scenes;
        graphics::Window* m_window;
        long long m_lastTick;
        double m_elapsedTime;

        void initialize();

        void tick() {
            auto now = utils::Clock::Now();
            auto delta = now - m_lastTick;
            auto dt = utils::Clock::ToSeconds(delta);
            m_elapsedTime += dt;
            m_lastTick = now;
            onUpdate(dt);
        }
    };
}

#endif //HAGAME2_GAME_H
