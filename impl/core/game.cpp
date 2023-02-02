//
// Created by henry on 1/9/23.
//

#include <iostream>
#include "../../../include/hagame/core/game.h"
#include "../../include/hagame/graphics/windows.h"

void hg::Game::initialize() {

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

    onInit();
}

void hg::Game::tick() {
    auto now = utils::Clock::Now();
    auto delta = now - m_lastTick;
    auto dt = utils::Clock::ToSeconds(delta);
    m_elapsedTime += dt;
    m_lastTick = now;

    onBeforeUpdate();

    onUpdate(dt);

    if (scenes()->hasActive()) {
        scenes()->active()->onUpdate(dt);
    }

    onAfterUpdate();
}

void hg::Game::destroy() {
    m_scenes.clear();
    onDestroy();
}
