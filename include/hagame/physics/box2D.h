//
// Created by henry on 11/3/24.
//

#ifndef HAGAME2_BOX2D_H
#define HAGAME2_BOX2D_H

#include "box2d/math_functions.h"
#include "hagame/math/aliases.h"

namespace hg::physics {
    b2Vec2 FromVec(Vec2 vec);
    b2Vec2 FromVec(Vec3 vec);
    Vec2 ToVec(b2Vec2 vec);
    Vec3 ToVec3(b2Vec2 vec);
}

#endif //HAGAME2_BOX2D_H
