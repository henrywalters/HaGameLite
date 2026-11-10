//
// Created by henry on 1/9/23.
//

#include <iostream>
#include <algorithm>
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
    m_fixedAccumulator = 0;
    m_dt = 0;

    onInit();
}

double hg::Game::timeSinceLastFixedTick() const {
    return m_fixedAccumulator;
}

double hg::Game::fixedAlpha() const {
    return m_fixedDt > 0 ? m_fixedAccumulator / m_fixedDt : 0;
}

void hg::Game::tick() {

    auto now = Clock::Now();

    m_dt = Clock::ToSeconds(now - m_lastTick);
    m_elapsedTime += m_dt;
    m_lastTick = now;

    // Fixed timestep: bank real time and spend it in constant-size steps, so the simulation advances at the same
    // rate and in the same increments regardless of frame rate. Time beyond the per-frame cap is dropped
    // (the game briefly slows down) rather than trying to catch up forever.
    m_fixedDt = 1.0 / (double) std::max(1, fixedTicksPerSecond);
    int maxTicks = std::max(1, maxFixedTicksPerFrame);
    m_fixedAccumulator = std::min(m_fixedAccumulator + m_dt, m_fixedDt * maxTicks);

    onBeforeUpdate();

    onUpdate(m_dt);

    int ticks = 0;
    while (m_fixedAccumulator >= m_fixedDt && ticks < maxTicks) {
        onFixedUpdate(m_fixedDt);
        if (scenes()->hasActive()) {
            scenes()->active()->fixedUpdate(m_fixedDt);
        }
        m_fixedAccumulator -= m_fixedDt;
        ticks++;
    }

    if (scenes()->hasActive()) {
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

