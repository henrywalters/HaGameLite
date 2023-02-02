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
        virtual void onBeforeUpdate() {}
        virtual void onUpdate(double dt) {}
        virtual void onAfterUpdate() {}
        virtual void onDestroy() {}

        utils::StateMachine<Scene>* scenes() {
            return &m_scenes;
        }

        graphics::Window* window() { return m_window; }

        void renderPipeline(graphics::RenderPipeline* pipeline) {
            m_renderPipeline = pipeline;
        }

        graphics::RenderPipeline* renderPipeline() { return m_renderPipeline; }

    protected:
        std::string toString() const {
            return "Game<" + std::to_string(id()) + ">";
        }

    private:
        std::string m_name;
        graphics::Resolution m_resolution;
        utils::StateMachine<Scene> m_scenes;
        graphics::Window* m_window = nullptr;
        graphics::RenderPipeline* m_renderPipeline = nullptr;
        long long m_lastTick;
        double m_elapsedTime;

        void initialize();

        void tick();
    };
}

#endif //HAGAME2_GAME_H
