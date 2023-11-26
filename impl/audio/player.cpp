//
// Created by henry on 11/18/23.
//
#include <iostream>
#include "../../../include/hagame/audio/player.h"


using namespace hg::audio;

void Player::onStart() {
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

void Player::onTick() {
    while (!m_events.empty()) {
        auto e = m_events.pop_front();
        EventType type = std::visit([&](auto&& e) -> EventType {return e.type; }, e);

        if (type == EventType::AddSource) {
            auto event = std::get<SourceEvent>(e);
            m_sources.insert(std::make_pair(event.source, std::make_unique<Source>(event.settings)));
        } else if (type == EventType::UpdateSource) {
            auto event = std::get<SourceEvent>(e);
            m_sources[event.source]->settings(event.settings);
        } else if (type == EventType::AddBuffer) {
            auto event = std::get<AddBufferEvent>(e);
            m_buffers.insert(std::make_pair(event.buffer, std::make_unique<StreamBuffer>()));
            m_buffers[event.buffer]->load(event.stream);
        } else if (type == EventType::BindBuffer) {
            auto event = std::get<BindBufferEvent>(e);
            m_sources[event.source]->bind(m_buffers[event.buffer]->id());
        } else if (type == EventType::PlaySource) {
            auto event = std::get<PlaySourceEvent>(e);
            m_sources[event.source]->play();
        } else {
            throw std::runtime_error("Unhandled Audio Event in Player.cpp");
        }
    }
}

void Player::onAbort() {
    for (const auto& [name, source] : m_sources) {
        source->stop();
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

void Player::updateSource(source_t source, hg::Vec3 position, hg::Vec3 velocity) {
    SourceEvent e;
    e.type = EventType::UpdateSource;
    e.source = source;
    e.settings = m_sources[source]->settings();
    e.settings.position = position;
    e.settings.velocity = velocity;
    m_events.push_back(e);
}

void Player::updateSource(source_t source, float pitch, float gain) {
    SourceEvent e;
    e.type = EventType::UpdateSource;
    e.source = source;
    e.settings = m_sources[source]->settings();
    e.settings.pitch = pitch;
    e.settings.gain = gain;
    m_events.push_back(e);
}

void Player::updateSource(source_t source, const SourceSettings &settings) {
    SourceEvent e;
    e.type = EventType::UpdateSource;
    e.source = source;
    e.settings = settings;
    m_events.push_back(e);
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
    PlaySourceEvent e;
    e.type = EventType::PlaySource;
    e.source = source;
    m_events.push_back(e);
}

Source* Player::getSource(source_t source) {
    return m_sources.find(source) == m_sources.end() ? nullptr : m_sources[source].get();
}

