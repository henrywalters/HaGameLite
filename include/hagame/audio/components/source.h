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
        std::string streamName;
        float pitch = 1.0f;
        float gain = 1.0f;
        Vec3 velocity;
        bool looping = false;
        bool playOnStart = false;

    protected:
        OBJECT_NAME(AudioSource)
    };

    HG_COMPONENT(Audio, AudioSource)
    HG_FIELD(AudioSource, int, channel)
    HG_FIELD(AudioSource, std::string, streamName)
    HG_FIELD(AudioSource, float, pitch)
    HG_FIELD(AudioSource, float, gain)
    HG_FIELD(AudioSource, hg::Vec3, velocity)
    HG_FIELD(AudioSource, bool, looping)
    HG_FIELD(AudioSource, bool, playOnStart)
}

#endif //HAGAME2_SOURCE_H
