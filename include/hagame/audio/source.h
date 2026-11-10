//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_SOURCE_H
#define HAGAME2_SOURCE_H

#include "../../../include/hagame/math/aliases.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

namespace hg::audio {

    enum class SourceState {
        Playing,
        Paused,
        Stopped,
    };

    struct SourceSettings {
        ALuint buffer = 0; // AL_NONE until a buffer is bound
        float pitch = 1.0f;
        float gain = 1.0f;
        Vec3 position;
        Vec3 velocity;
        bool looping = false;

        // Spatial sources live in world space and fade with distance from the listener. Non-spatial sources
        // (music, UI) stay glued to the listener at full volume
        bool spatial = false;
        float referenceDistance = 1.0f; // Full volume within this distance
        float maxDistance = 20.0f; // Silent beyond this distance (linear clamped model)
        float rolloff = 1.0f;

        bool operator==(const SourceSettings&) const = default;
    };

    class Source {
    public:

        Source() {
            alGenSources((ALuint)1, &m_source);
            spatial(m_settings.spatial);
        }

        ~Source() {
            alSourceStop(m_source);
            alDeleteSources(1, &m_source);
        }

        Source(const Source&) = delete;
        Source& operator=(const Source&) = delete;

        Source(const SourceSettings& _settings):
        Source() {
            settings(_settings);
        }

        void bind(ALuint buffer) {
            if (m_settings.buffer != buffer) {
                m_settings.buffer = buffer;
                alSourcei(m_source, AL_BUFFER, m_settings.buffer);
            }
        }

        SourceSettings settings() const { return m_settings; }

        void settings(const SourceSettings& settings) {
            pitch(settings.pitch);
            gain(settings.gain);
            position(settings.position);
            velocity(settings.velocity);
            looping(settings.looping);
            spatial(settings.spatial);
            attenuation(settings.referenceDistance, settings.maxDistance, settings.rolloff);
            bind(settings.buffer);
        }

        bool spatial() const { return m_settings.spatial; }

        void spatial(bool value) {
            m_settings.spatial = value;
            alSourcei(m_source, AL_SOURCE_RELATIVE, value ? AL_FALSE : AL_TRUE);
            // Without this OpenAL Soft plays stereo buffers un-attenuated; with it they're downmixed and positioned
            if (alIsExtensionPresent("AL_SOFT_source_spatialize")) {
                alSourcei(m_source, AL_SOURCE_SPATIALIZE_SOFT, value ? AL_TRUE : AL_FALSE);
            }
        }

        void attenuation(float referenceDistance, float maxDistance, float rolloff) {
            m_settings.referenceDistance = referenceDistance;
            m_settings.maxDistance = maxDistance;
            m_settings.rolloff = rolloff;
            alSourcef(m_source, AL_REFERENCE_DISTANCE, referenceDistance);
            alSourcef(m_source, AL_MAX_DISTANCE, maxDistance);
            alSourcef(m_source, AL_ROLLOFF_FACTOR, rolloff);
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

        void setState(SourceState state) {
            if (state == SourceState::Playing) {
                play();
            } else if (state == SourceState::Paused) {
                pause();
            } else {
                stop();
            }
        }

        void rewind() {
            alSourceRewind(m_source);
        }

        SourceState state() const {
            ALint state;
            alGetSourcei(m_source, AL_SOURCE_STATE, &state);
            switch (state) {
                case AL_PLAYING:
                    return SourceState::Playing;
                case AL_PAUSED:
                    return SourceState::Paused;
                case AL_STOPPED:
                    return SourceState::Stopped;
                default:
                    return SourceState::Stopped;
            }
        }

        bool playing() const {
            return state() == SourceState::Playing;
        }

    private:

        ALuint m_source;

        SourceSettings m_settings;
    };
}

#endif //HAGAME2_SOURCE_H
