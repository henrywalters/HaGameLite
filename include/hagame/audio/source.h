//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_SOURCE_H
#define HAGAME2_SOURCE_H

#include "../../../include/hagame/math/aliases.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace hg::audio {

    struct SourceSettings {
        uint16_t buffer;
        float pitch = 1.0f;
        float gain = 1.0f;
        Vec3 position;
        Vec3 velocity;
        bool looping = false;
    };

    class Source {
    public:

        Source() {
            alGenSources((ALuint)1, &m_source);
            alSourcei(m_source, AL_SOURCE_RELATIVE, 1);
        }

        Source(const SourceSettings& _settings):
        Source() {
            settings(_settings);
        }

        void bind(uint16_t buffer) {
            m_settings.buffer = buffer;
            alSourcei(m_source, AL_BUFFER, m_settings.buffer);
        }

        SourceSettings settings() const { return m_settings; }

        void settings(const SourceSettings& settings) {
            pitch(settings.pitch);
            gain(settings.gain);
            position(settings.position);
            velocity(settings.velocity);
            looping(settings.looping);
            bind(settings.buffer);
        }

        float pitch() const { return m_settings.pitch; }

        void pitch(float value) {
            m_settings.pitch = value;
            alSourcef(m_source, AL_PITCH, m_settings.pitch);
        }

        float gain() const { return m_settings.gain; }

        void gain(float value) {
            m_settings.gain = value;
            alSourcef(m_source, AL_GAIN, m_settings.gain);
        }

        bool looping() const { return m_settings.looping; }

        void looping(bool value) {
            m_settings.looping = value;
            alSourcei(m_source, AL_LOOPING, m_settings.looping);
        }

        Vec3 position() const { return m_settings.position; }

        void position(Vec3 pos) {
            m_settings.position = pos;
            alSource3f(m_source, AL_POSITION, m_settings.position[0], m_settings.position[1], m_settings.position[2]);
        }

        Vec3 velocity() const { return m_settings.velocity; }

        void velocity(Vec3 vel) {
            m_settings.velocity = vel;
            alSource3f(m_source, AL_VELOCITY, m_settings.velocity[0], m_settings.velocity[1], m_settings.velocity[2]);
        }

        void play() {
            alSourcePlay(m_source);
        }

        void stop() {
            alSourceStop(m_source);
        }

        void pause() {
            alSourcePause(m_source);
        }

        void rewind() {
            alSourceRewind(m_source);
        }

        bool playing() const {
            ALint state;
            alGetSourcei(m_source, AL_SOURCE_STATE, &state);
            return state == AL_PLAYING;
        }

    private:

        ALCuint m_source;

        SourceSettings m_settings;
    };
}

#endif //HAGAME2_SOURCE_H
