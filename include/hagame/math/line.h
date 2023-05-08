//
// Created by henry on 2/1/23.
//

#ifndef HAGAME2_MATH_LINE_H
#define HAGAME2_MATH_LINE_H

#include "aliases.h"

namespace hg::math {
    struct Line {

        Vec3 a;
        Vec3 b;

        Line(Vec3 _a, Vec3 _b) : a(_a), b(_b) {}

        // Get a point on the parameterized line
        Vec3 getPointOnLine(float t) {
            return a + (b - a) * t;
        }

        // Calculates the point on the line closest to another point as well as the parameter t
        Vec3 closestPoint(Vec3 point, float& t) {
            Vec3 ab = b - a;
            t = dot(point - a, ab);

            float denom = dot2(ab);
            t = t / denom;
            return a + ab * t;
        }

        Vec3 center() {
            return getPointOnLine(0.5f);
        }

        Quat rotation() {
            return Quat(Vec3(b[2] - a[2], b[1] - a[1], b[0] - a[0]));
        }

        float magnitude() {
            return (b - a).magnitude();
        }

        float magnitudeSq() {
            return (b - a).magnitudeSq();
        }
    };

}

#endif //HAGAME2_LINE_H
