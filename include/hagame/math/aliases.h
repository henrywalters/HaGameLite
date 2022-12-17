//
// Created by henry on 12/14/22.
//

#ifndef HAGAME2_ALIASES_H
#define HAGAME2_ALIASES_H

#include "vector.h"
#include "hypercube.h"

namespace hg {
    using Vec4 = math::Vector<4, float>;
    using Vec3 = math::Vector<3, float>;
    using Vec2 = math::Vector<2, float>;

    using Vec4i = math::Vector<4, int>;
    using Vec3i = math::Vector<3, int>;
    using Vec2i = math::Vector<2, int>;

    using Rect = math::Hypercube<2, float>;
    using Cube = math::Hypercube<3, float>;

    using Recti = math::Hypercube<2, int>;
    using Cubei = math::Hypercube<3, int>;
}

#endif //HAGAME2_ALIASES_H
