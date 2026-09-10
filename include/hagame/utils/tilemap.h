//
// Created by henry on 7/28/26.
//

#ifndef HAGAME2_UTILS_TILEMAP_H
#define HAGAME2_UTILS_TILEMAP_H

#include "spatialMap.h"
#include "hagame/math/aliases.h"

namespace hg::utils {
    class Tilemap {
    public:

        Vec2 tileSize = Vec2::Zero();

        Vec2i getIndex(hg::Vec2 pos) const {
            return pos.div(tileSize).floor().cast<int>();
        }

        Vec2 getPos(Vec2i index) const {
            return index.cast<float>().prod(tileSize);
        }

    };
}

#endif //HAGAME2_TILEMAP_H
