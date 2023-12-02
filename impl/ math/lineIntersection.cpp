//
// Created by henry on 11/30/23.
//
#include "../../../include/hagame/math/lineIntersection.h"

using namespace hg;
using namespace hg::math;

bool math::linesIntersect(Vec2 a, Vec2 b, Vec2 p, Vec2 q, LineIntersection &intersection) {
    auto s1 = b - a;
    auto s2 = q - p;

    auto delta = p - a;
    auto denom = cross2(s1, s2);
    auto collinearTest = cross2(delta, s1) == 0;

    if (denom == 0 && collinearTest) {
        auto s2sq = dot2(s2);
        auto t0 = dot(delta, s2) / s2sq;
        auto t1 = t0 + dot(s1, s2) / s2sq;
        auto t = hg::math::Interval<float>(t0, t1);
        if (t.overlaps(hg::math::Interval<float>(1.0))) {
            intersection = LineIntersection::Collinear(t, a + s1 * t0, a + s1 * t1);
            return true;
        }
        else {
            intersection = LineIntersection::CollinearDisjoint();
            return false;
        }
    }
    else if (denom == 0 && !collinearTest) {
        intersection = LineIntersection::Parallel();
        return false;
    }
    else {
        if (denom == 0) return false;
        auto t = cross2(delta, s2) / denom;
        auto q = cross2(delta, s1) / denom;

        if (t >= 0 && t <= 1 && q >= 0 && q <= 1) {
            intersection = LineIntersection(t, a + s1 * t);
            return true;
        }

        return false;
    }
}
