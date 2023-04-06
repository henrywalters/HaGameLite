//
// Created by henry on 1/9/23.
//

#include <iostream>
#include "../../../include/hagame/core/game.h"
#include "../../include/hagame/graphics/windows.h"

void hg::Game::initialize() {

    m_scenes->events.subscribe(utils::StateMode::Init, [&](auto scene) {
        scene->m_game = this;
        scene->init();
    });

    m_scenes->events.subscribe(utils::StateMode::Active, [&](auto scene) {
        scene->activate();
    });

    m_scenes->events.subscribe(utils::StateMode::Inactive, [&](auto state) {
        state->deactivate();
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

    if (!running()) {
        return;
    }

    onUpdate(dt);

    if (!running()) {
        return;
    }

    if (scenes()->hasActive()) {
        scenes()->active()->update(dt);
        if (!running()) {
            return;
        }
    }

    if (!running()) {
        return;
    }
    onAfterUpdate();
}

void hg::Game::destroy() {
    m_scenes->clear();
    onDestroy();
}
