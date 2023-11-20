//
// Created by henry on 11/18/23.
//
#include <iostream>
#include "../../../include/hagame/audio/player.h"
#include "../../../include/hagame/audio/device.h"

using namespace hg::audio;

Player::Player() {
    m_device = alcOpenDevice(nullptr);

    if (!m_device) {
        return;
    }

    m_context = alcCreateContext(m_device, nullptr);
    alcMakeContextCurrent(m_context);

    ALCenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cout << "OpenAL ERROR: " << error << "\n";
    }
}
