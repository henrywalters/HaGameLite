//
// Created by henry on 1/9/23.
//

#include "../../../include/hagame/core/game.h"
#include "../../include/hagame/graphics/windows.h"

void hg::Game::initialize() {

    m_window = graphics::Windows::Create(m_name, m_resolution);

    m_scenes.events.subscribe(utils::StateMode::Init, [&](auto scene) {
        scene->onInit();
    });

    m_scenes.events.subscribe(utils::StateMode::Active, [&](auto scene) {
        scene->onActivate();
    });

    m_scenes.events.subscribe(utils::StateMode::Inactive, [&](auto state) {
        state->onDeactivate();
    });

    graphics::Windows::Events.subscribe(graphics::WindowEvents::Close, [&](auto glfwWindow) {
        m_window->running(false);
    });

    graphics::Windows::Events.subscribe(graphics::WindowEvents::Resize, [&](auto glfwWindow) {
        onResize(m_window->size());
    });

    onInit();
}
