//
// Created by henry on 11/20/23.
//

#ifndef HAGAME2_SOURCE_COMPONENT_H
#define HAGAME2_SOURCE_COMPONENT_H

#include "../player.h"
#include "../../core/entity.h"
#include "../../core/component.h"

namespace hg::audio {
    class SourceComponent : public hg::Component {
    public:

        int channel;
        std::string streamName;
        float pitch = 1.0f;
        float gain = 1.0f;
        Vec3 velocity;
        bool looping = false;
        bool playOnStart = false;

    protected:
        OBJECT_NAME(SourceComponent)
    };

    HG_COMPONENT(Audio, SourceComponent)
    HG_FIELD(SourceComponent, int, channel)
    HG_FIELD(SourceComponent, std::string, streamName)
    HG_FIELD(SourceComponent, float, pitch)
    HG_FIELD(SourceComponent, float, gain)
    HG_FIELD(SourceComponent, hg::Vec3, velocity)
    HG_FIELD(SourceComponent, bool, looping)
    HG_FIELD(SourceComponent, bool, playOnStart)
}

#endif //HAGAME2_SOURCE_H
