//
// Created by henry on 11/3/24.
//
#include <hagame/physics/box2D.h>

b2Vec2 hg::physics::FromVec(hg::Vec2 vec) {
    return b2Vec2{vec[0], vec[1]};
}

b2Vec2 hg::physics::FromVec(hg::Vec3 vec) {
    return b2Vec2{vec[0], vec[1]};
}

hg::Vec2 hg::physics::ToVec(b2Vec2 vec) {
    return hg::Vec2(vec.x, vec.y);
}

hg::Vec3 hg::physics::ToVec3(b2Vec2 vec) {
    return hg::Vec3(vec.x, vec.y, 0);
}
