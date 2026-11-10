//
// Created by henry on 11/18/23.
//
#include <iostream>
#include <chrono>
#include "../../../include/hagame/audio/player.h"
#include "AL/alext.h"

using namespace hg::audio;

Player::~Player() {
    // Join here, while our members are still alive; ~LoopingThread would be too late
    stop();
}

void Player::onStart() {
    m_device = alcOpenDevice(nullptr);

    if (!m_device) {
        std::cout << "OpenAL ERROR: failed to open audio device\n";
        return;
    }

    m_context = alcCreateContext(m_device, nullptr);

    // alcMakeContextCurrent is process-wide, so two Players would fight over it. Bind our context to this thread
    // only when the extension is available
    if (alcIsExtensionPresent(m_device, "ALC_EXT_thread_local_context")) {
        auto setThreadContext = (PFNALCSETTHREADCONTEXTPROC) alcGetProcAddress(m_device, "alcSetThreadContext");
        setThreadContext(m_context);
    } else {
        alcMakeContextCurrent(m_context);
    }

    // Linear falloff reaches silence at each source's max distance, which reads better in a 2D game than the
    // default inverse curve that never quite gets there
    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

    // 2D: the listener faces into the screen with +Y up, so +X is to the right
    ALfloat orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_ORIENTATION, orientation);

    checkError();
    m_valid = true;
}

void Player::onTick() {
    if (!m_valid) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        return;
    }

    bool processed = false;

    while (!m_events.empty()) {
        processed = true;
        auto e = m_events.pop_front();
        EventType type = std::visit([&](auto&& e) -> EventType {return e.type; }, e);
        if (type == EventType::AddSource) {
            auto event = std::get<SourceEvent>(e);
            m_sources.insert(std::make_pair(event.source, std::make_unique<Source>(event.settings)));
        } else if (type == EventType::UpdateSource) {
            auto event = std::get<SourceEvent>(e);
            if (auto it = m_sources.find(event.source); it != m_sources.end()) {
                event.settings.buffer = it->second->settings().buffer;
                it->second->settings(event.settings);
            }
        } else if (type == EventType::AddBuffer) {
            auto event = std::get<AddBufferEvent>(e);
            m_buffers.insert(std::make_pair(event.buffer, std::make_unique<StreamBuffer>()));
            m_buffers[event.buffer]->load(event.stream);
        } else if (type == EventType::BindBuffer) {
            auto event = std::get<BindBufferEvent>(e);
            auto source = m_sources.find(event.source);
            auto buffer = m_buffers.find(event.buffer);
            if (source != m_sources.end() && buffer != m_buffers.end()) {
                source->second->bind(buffer->second->id());
            }
        } else if (type == EventType::ChangeState) {
            auto event = std::get<ChangeStateEvent>(e);
            if (auto it = m_sources.find(event.source); it != m_sources.end()) {
                it->second->setState(event.state);
            }
        } else if (type == EventType::RemoveSource) {
            auto event = std::get<RemoveSourceEvent>(e);
            m_sources.erase(event.source);
        } else if (type == EventType::UpdateListener) {
            auto event = std::get<ListenerEvent>(e);
            alListener3f(AL_POSITION, event.settings.position[0], event.settings.position[1], event.settings.position[2]);
            alListener3f(AL_VELOCITY, event.settings.velocity[0], event.settings.velocity[1], event.settings.velocity[2]);
            alListenerf(AL_GAIN, event.settings.gain);
        } else {
            throw std::runtime_error("Unhandled Audio Event in Player.cpp");
        }
    }

    {
        std::scoped_lock lock(m_stateMutex);
        m_states.clear();
        for (const auto& [id, source] : m_sources) {
            m_states[id] = source->state();
        }
    }

    if (!processed) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void Player::onEnd() {
    m_sources.clear();
    m_buffers.clear();
    if (m_context) {
        if (alcIsExtensionPresent(m_device, "ALC_EXT_thread_local_context")) {
            auto setThreadContext = (PFNALCSETTHREADCONTEXTPROC) alcGetProcAddress(m_device, "alcSetThreadContext");
            setThreadContext(nullptr);
        } else {
            alcMakeContextCurrent(nullptr);
        }
        alcDestroyContext(m_context);
    }
    if (m_device) {
        alcCloseDevice(m_device);
    }
}

void Player::checkError() {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cout << "OpenAL ERROR: " << error << "\n";
    }
}

source_t Player::addSource(hg::Vec3 position) {
    SourceEvent e;
    e.type = EventType::AddSource;
    e.settings.position = position;
    e.source = utils::UUID::Generate();
    m_events.push_back(e);
    return e.source;
}

void Player::updateSource(source_t source, const SourceSettings &settings) {
    SourceEvent e;
    e.type = EventType::UpdateSource;
    e.source = source;
    e.settings = settings;
    m_events.push_back(e);
}

void Player::removeSource(source_t source) {
    RemoveSourceEvent e;
    e.source = source;
    m_events.push_back(e);
}

std::optional<SourceState> Player::getState(source_t source) {
    std::scoped_lock lock(m_stateMutex);
    auto it = m_states.find(source);
    if (it == m_states.end()) {
        return std::nullopt;
    }
    return it->second;
}

buffer_t Player::addBuffer(Stream* stream) {
    AddBufferEvent e;
    e.type = EventType::AddBuffer;
    e.stream = stream;
    e.buffer = utils::UUID::Generate();
    m_events.push_back(e);
    return e.buffer;
}

void Player::bindBuffer(buffer_t buffer, source_t source) {
    BindBufferEvent e;
    e.type = EventType::BindBuffer;
    e.source = source;
    e.buffer = buffer;
    m_events.push_back(e);
}

void Player::playSource(source_t source) {
    changeState(source, SourceState::Playing);
}

void Player::pauseSource(source_t source) {
    changeState(source, SourceState::Paused);
}

void Player::stopSource(source_t source) {
    changeState(source, SourceState::Stopped);
}

void Player::updateListener(const ListenerSettings& settings) {
    ListenerEvent e;
    e.settings = settings;
    m_events.push_back(e);
}

void Player::changeState(source_t source, SourceState state) {
    ChangeStateEvent e;
    e.type = EventType::ChangeState;
    e.state = state;
    e.source = source;
    m_events.push_back(e);
}
