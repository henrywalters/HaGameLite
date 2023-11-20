//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_SOURCE_H
#define HAGAME2_SOURCE_H

#include "../../../include/hagame/math/aliases.h"
#include "stream.h"
#include "AL/al.h"
#include "AL/alc.h"

namespace hg::audio {
    class Source {
    public:

        Source() {
            alGenSources((ALuint)1, &m_source);
        }

        float pitch() const { return m_pitch; }

        void pitch(float value) {
            m_pitch = value;
            alSourcef(m_source, AL_PITCH, m_pitch);
        }

        float gain() const { return m_gain; }

        void gain(float value) {
            m_gain = value;
            alSourcef(m_source, AL_GAIN, m_gain);
        }

        bool looping() const { return m_looping; }

        void looping(bool value) {
            m_looping = value;
            alSourcei(m_source, AL_LOOPING, m_looping);
        }

        Vec3 position() const { return m_position; }

        void position(Vec3 pos) {
            m_position = pos;
            alSource3f(m_source, AL_POSITION, m_position[0], m_position[1], m_position[2]);
        }

        Vec3 velocity() const { return m_velocity; }

        void velocity(Vec3 vel) {
            m_velocity = vel;
            alSource3f(m_source, AL_VELOCITY, m_velocity[0], m_velocity[1], m_velocity[2]);
        }

        void play(const Stream& stream) {
            alSourcei(m_source, AL_BUFFER, stream.m_buffer);
            alSourcePlay(m_source);
            ALint state;
            alGetSourcei(m_source, AL_SOURCE_STATE, &state);
            //while (state == AL_PLAYING) {
            //    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
            //}
        }

    private:

        ALCuint m_source;

        float m_pitch = 1.0f;
        float m_gain = 1.0f;
        Vec3 m_position;
        Vec3 m_velocity;
        bool m_looping = false;

    };
}

#endif //HAGAME2_SOURCE_H
