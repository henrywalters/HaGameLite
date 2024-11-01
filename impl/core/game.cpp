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
    m_lastFixedTick = Clock::Now();
    m_dt = 0;

    onInit();
}

void hg::Game::tick() {

    auto now = Clock::Now();
    auto delta = now - m_lastTick;
    auto fixedDelta = now - m_lastFixedTick;
    double tmpFixedDt = Clock::ToSeconds(fixedDelta);

    bool doFixedUpdate = tmpFixedDt >= 1.0 / (double) fixedTicksPerSecond;

    m_dt = Clock::ToSeconds(delta);
    m_elapsedTime += m_dt;
    m_lastTick = now;

    if (doFixedUpdate) {
        m_lastFixedTick = now;
        m_fixedDt = tmpFixedDt;
    }

    onBeforeUpdate();

    onUpdate(m_dt);

    if (doFixedUpdate) {
        onFixedUpdate(m_fixedDt);
    }

    if (scenes()->hasActive()) {
        if (doFixedUpdate) {
            scenes()->active()->fixedUpdate(m_fixedDt);
        }
        scenes()->active()->update(m_dt);
    }

    onAfterUpdate();

    if (m_requestedShutdown) {
        running(false);
    }

}

void hg::Game::destroy() {
    m_scenes->clear();
    onDestroy();
}

void hg::Game::requestShutdown() {
    m_requestedShutdown = true;
}
