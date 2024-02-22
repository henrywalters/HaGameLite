//
// Created by henry on 1/4/23.
//

#ifndef HAGAME2_LINESEGMENT_H
#define HAGAME2_LINESEGMENT_H

#include "aliases.h"
#include "functions.h"

namespace hg::math {
    struct LineSegment {
        Vec3 a;
        Vec3 b;

        LineSegment() {}
        LineSegment(Vec3 _a, Vec3 _b) : a(_a), b(_b) {}

        // Get a point on the parameterized line
        Vec3 getPointOnLine(float t) {
            return a + (b - a) * (t < 0 ? 0 : (t > 1 ? 1 : t));
        }

        bool contains(Vec3 point) {
            return approxEqual<float>((a - point).magnitude() + (b - point).magnitude(), (b - a).magnitude(), 0.0001);
        }

        // Calculates the point on the line closest to another point as well as the parameter t
        Vec3 closestPoint(Vec3 point, float& t) {
            Vec3 ab = b - a;
            t = dot(point - a, ab);

            if (t <= 0.0f) {
                t = 0.0f;
                return a;
            }
            else {
                float denom = dot2(ab);
                if (t >= denom) {
                    t = 1.0f;
                    return b;
                }
                else {
                    t = t / denom;
                    return a + ab * t;
                }
            }
        }

        float sqDistanceTo(Vec3 point) {
            Vec3 ab = b - a, ac = point - a, bc = point - b;
            float e = dot(ac, ab);
            if (e <= 0.0f) return dot2(ac);
            float f = dot2(ab);
            if (e >= f) return dot2(bc);
            return dot2(ac) - e * e / f;
        }
    };
}

#endif //HAGAME2_LINESEGMENT_H
