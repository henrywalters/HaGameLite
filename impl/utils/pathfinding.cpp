//
// Created by henry on 5/27/23.
//
#include "../../include/hagame/utils/pathfinding.h"

float distanceMetric(hg::Vec2i a, hg::Vec2i b) {
    return (b.cast<float>() - a.cast<float>()).magnitude();
}

