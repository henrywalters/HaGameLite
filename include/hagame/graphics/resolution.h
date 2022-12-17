//
// Created by henry on 12/16/22.
//

#ifndef HAGAME2_RESOLUTION_H
#define HAGAME2_RESOLUTION_H

#include "../math/aliases.h"

namespace hg::graphics {
    using Resolution = Vec2i;

    const Resolution SD = Vec2i(640, 480);
    const Resolution HD = Vec2i(1280, 720);
    const Resolution FULL_HD = Vec2i(1920, 1080);
}

#endif //HAGAME2_RESOLUTION_H
