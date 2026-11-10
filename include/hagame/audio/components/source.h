//
// Created by henry on 11/20/23.
//

#ifndef HAGAME2_SOURCE_COMPONENT_H
#define HAGAME2_SOURCE_COMPONENT_H

#include "../player.h"
#include "../../core/entity.h"
#include "../../core/component.h"

namespace hg::audio {
    class AudioSource : public hg::Component {
    public:

        int channel;
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

    protected:
        OBJECT_NAME(AudioSource)
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
