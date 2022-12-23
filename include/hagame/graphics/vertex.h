//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_VERTEX_H
#define HAGAME2_VERTEX_H

#include "../math/aliases.h"

namespace hg::graphics {

    struct Vertex {
        Vec3 position;
        Vec3 normal;
        Vec2 texCoords;
    };
}

#endif //HAGAME2_VERTEX_H
