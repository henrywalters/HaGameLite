//
// Created by henry on 1/9/23.
//

#ifndef HAGAME2_HG_H
#define HAGAME2_HG_H

#include "game.h"

namespace hg {
    class HG {
    public:
        static Game* s_emscriptenGame;
        static void Run(Game* game);
    private:

        static void TickEmscripten();
    };
}

#endif //HAGAME2_HG_H
