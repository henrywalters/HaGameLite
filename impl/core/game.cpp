//
// Created by henry on 1/9/23.
//

#include <iostream>
#include "../../../include/hagame/core/game.h"
#include "../../include/hagame/graphics/windows.h"

void hg::Game::initialize() {

    m_window = graphics::Windows::Create(m_name, m_resolution);

    m_scenes.events.subscribe(utils::StateMode::Init, [&](auto scene) {
        scene->m_game = this;
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

void hg::Game::tick() {
    auto now = utils::Clock::Now();
    auto delta = now - m_lastTick;
    auto dt = utils::Clock::ToSeconds(delta);
    m_elapsedTime += dt;
    m_lastTick = now;

    window()->clear();

    onBeforeUpdate();

    if (renderPipeline() != nullptr) {
        renderPipeline()->onBeforeRender();
    }

    onUpdate(dt);

    if (scenes()->hasActive()) {
        scenes()->active()->onUpdate(dt);
    }

    if (renderPipeline() != nullptr) {
        renderPipeline()->onRender();
    }

    onAfterUpdate();

    if (renderPipeline() != nullptr) {
        renderPipeline()->onAfterRender();
    }

    window()->render();
}
