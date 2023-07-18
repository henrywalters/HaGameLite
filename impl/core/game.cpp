//
// Created by henry on 1/9/23.
//

#include <iostream>
#include "../../../include/hagame/core/game.h"
#include "../../include/hagame/graphics/windows.h"

using namespace hg::utils;
using namespace hg::graphics;

void hg::Game::initialize() {

    m_scenes->events.subscribe(StateMode::Init, [&](auto scene) {
        scene->m_game = this;
        scene->init();
    });

    m_scenes->events.subscribe(StateMode::Active, [&](auto scene) {
        scene->activate();
    });

    m_scenes->events.subscribe(StateMode::Inactive, [&](auto state) {
        state->deactivate();
    });

    m_lastTick = Clock::Now();
    m_dt = 0;

    onInit();
}

void hg::Game::tick() {

    onBeforeUpdate();

    if (!running()) {
        return;
    }

    onUpdate(m_dt);

    if (!running()) {
        return;
    }

    if (scenes()->hasActive()) {
        scenes()->active()->update(m_dt);
        if (!running()) {
            return;
        }
    }

    if (!running()) {
        return;
    }
    onAfterUpdate();

    auto now = Clock::Now();
    auto delta = now - m_lastTick;
    m_dt = Clock::ToSeconds(delta);
    m_elapsedTime += m_dt;
    m_lastTick = now;
}

void hg::Game::destroy() {
    m_scenes->clear();
    onDestroy();
}
