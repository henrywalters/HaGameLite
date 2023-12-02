//
// Created by henry on 4/22/23.
//

#ifndef HAGAME2_POLYGON_H
#define HAGAME2_POLYGON_H

#include <vector>
#include "vector.h"

namespace hg::math {
    struct Edge {
        Vec2 a;
        Vec2 b;
    };
    using Polygon = std::vector<Edge>;
}

#endif //HAGAME2_POLYGON_H
