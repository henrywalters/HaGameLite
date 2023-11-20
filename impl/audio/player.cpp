//
// Created by henry on 11/18/23.
//
#include <iostream>
#include "../../../include/hagame/audio/player.h"
#include "../../../include/hagame/audio/device.h"

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

        std::cout << "Processing: " << (int) e.type << "\n";

        switch (e.type) {
            case EventType::AddSource:
                m_sources.insert(std::make_pair(e.source, std::make_unique<Source>()));
                m_sources[e.source]->position(e.position);
                break;
            case EventType::AddStream:
                m_streams.insert(std::make_pair(e.stream, std::make_unique<Stream>(e.file)));
                break;
            case EventType::PlayStream:
                m_sources[e.source]->play(*m_streams[e.stream]);
                break;
        }
    }
}

void Player::onAbort() {
    LoopingThread::onAbort();
}

source_t Player::addSource(hg::Vec3 position) {
    Event e;
    e.type = EventType::AddSource;
    e.position = position;
    e.source = utils::UUID::Generate();
    m_events.push_back(e);
    return e.source;
}

void Player::updateSource(source_t source, hg::Vec3 position, hg::Vec3 velocity) {
    Event e;
    e.type = EventType::UpdateSource;
    e.position = position;
    e.velocity = velocity;
    m_events.push_back(e);
}

stream_t Player::addStream(std::string file) {
    Event e;
    e.type = EventType::AddStream;
    e.file = file;
    e.stream = utils::UUID::Generate();
    m_events.push_back(e);
    return e.stream;
}

void Player::playStream(source_t source, stream_t stream) {
    Event e;
    e.type = EventType::PlayStream;
    e.source = source;
    e.stream = stream;
    m_events.push_back(e);
}
