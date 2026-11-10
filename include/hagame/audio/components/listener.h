//
// Created by henry on 11/20/23.
//

#ifndef HAGAME2_LISTENER_H
#define HAGAME2_LISTENER_H

#include "../../core/entity.h"
#include "../../core/component.h"

namespace hg::audio {
    // The ears of the scene: spatial AudioSources fade with distance from the entity holding this. Put one on the
    // player or the camera; if a scene has several, the first one found wins
    class AudioListener : public hg::Component {
    public:

        float gain = 1.0f;
        // Added to the entity's position. In 2D a little +Z keeps sounds right on top of you from snapping hard left/right
        Vec3 offset = Vec3(0, 0, 1);

    protected:
        OBJECT_NAME(AudioListener)
    };

    HG_COMPONENT(Audio, AudioListener)
    HG_FIELD(AudioListener, float, gain)
    HG_FIELD(AudioListener, hg::Vec3, offset)
}

#endif //HAGAME2_LISTENER_H
