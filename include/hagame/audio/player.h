//
// Created by henry on 11/18/23.
//

#ifndef HAGAME2_PLAYER_H
#define HAGAME2_PLAYER_H

#include <AL/al.h>
#include <AL/alc.h>

namespace hg::audio {
    class Player {
    public:

        Player();

    private:

        bool m_valid = false;

        ALCdevice* m_device;
        ALCcontext* m_context;

        void checkError();
    };
}

#endif //HAGAME2_PLAYER_H
