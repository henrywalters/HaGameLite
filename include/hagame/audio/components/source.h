//
// Created by henry on 11/20/23.
//

#ifndef HAGAME2_SOURCE_COMPONENT_H
#define HAGAME2_SOURCE_COMPONENT_H

#include <optional>
#include "../player.h"
#include "../../core/entity.h"
#include "../../core/component.h"

namespace hg::audio {
    class AudioSource : public hg::Component {
    public:

        int channel = 0;
        StreamAsset stream;
        float pitch = 1.0f;
        float gain = 1.0f;
        Vec3 velocity;
        bool looping = false;
        bool playOnStart = false;

        // Fade with distance from the AudioListener, instead of playing at full volume everywhere
        bool spatial = false;
        float referenceDistance = 1.0f;
        float maxDistance = 20.0f;
        float rolloff = 1.0f;

        // Keep playing when this entity is removed and re-created elsewhere, e.g. the player going through a door.
        // A new AudioSource with the same key picks the running sound back up. Defaults to the stream path when empty
        bool persistent = false;
        std::string persistentKey;

        // Start playing, or carry on from where pause() left off
        void play() { request(SourceState::Playing); }

        // Hold the sound where it is, to carry on with play()
        void pause() { request(SourceState::Paused); }

        // Stop the sound and rewind it, so play() starts it from the beginning
        void stop() { request(SourceState::Stopped); }

        // These read back a request straight away, e.g. isPlaying() is true right after play(). Otherwise they follow
        // the sound itself, so a sound that isn't looping goes back to Stopped when it reaches the end
        SourceState state() const { return m_state; }
        bool isPlaying() const { return m_state == SourceState::Playing; }
        bool isPaused() const { return m_state == SourceState::Paused; }
        bool isStopped() const { return m_state == SourceState::Stopped; }

        // For the audio system: takes the latest play/pause/stop request, if there's one waiting
        std::optional<SourceState> takeRequest() {
            auto request = m_request;
            m_request.reset();
            return request;
        }

        // For the audio system: reports what the sound is doing
        void setState(SourceState state) { m_state = state; }

    protected:
        OBJECT_NAME(AudioSource)

    private:

        std::optional<SourceState> m_request;
        SourceState m_state = SourceState::Stopped;

        void request(SourceState state) {
            m_request = state;
            m_state = state;
        }
    };

    HG_COMPONENT(Audio, AudioSource)
    HG_FIELD(AudioSource, int, channel)
    HG_FIELD(AudioSource, hg::audio::StreamAsset, stream)
    HG_FIELD(AudioSource, float, pitch)
    HG_FIELD(AudioSource, float, gain)
    HG_FIELD(AudioSource, hg::Vec3, velocity)
    HG_FIELD(AudioSource, bool, looping)
    HG_FIELD(AudioSource, bool, playOnStart)
    HG_FIELD(AudioSource, bool, spatial)
    HG_FIELD(AudioSource, float, referenceDistance)
    HG_FIELD(AudioSource, float, maxDistance)
    HG_FIELD(AudioSource, float, rolloff)
    HG_FIELD(AudioSource, bool, persistent)
    HG_FIELD(AudioSource, std::string, persistentKey)
}

#endif //HAGAME2_SOURCE_H
