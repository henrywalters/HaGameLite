//
// Created by henry on 10/15/23.
//
#include "../../../include/hagame/math/functions.h"

using namespace hg;
using namespace hg::math;

bool rectContainsPoint(hg::Vec2 pos, hg::Vec2 size, hg::Vec2 point) {
    return point.x() >= pos.x() && point.x() < pos.x() + size.x() && point.y() >= pos.y() && point.y() < pos.y() + size.y();
}