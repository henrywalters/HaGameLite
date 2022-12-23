//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_TRIANGLE_H
#define HAGAME2_TRIANGLE_H

#include "aliases.h"

namespace hg::math {
    struct Triangle {

        Vec3 a;
        Vec3 b;
        Vec3 c;

        Triangle(Vec3 _a, Vec3 _b, Vec3 _c) : a(_a), b(_b), c(_c) {}

        Vec3 getBarycentricCoords(Vec3 point) {
            Vec3 v0 = b - a;
            Vec3 v1 = c - a;
            Vec3 v2 = point - a;

            float d00 = dot(v0, v0);
            float d01 = dot(v0, v1);
            float d11 = dot(v1, v1);
            float d20 = dot(v2, v0);
            float d21 = dot(v2, v1);

            float denominator = d00 * d11 - d01 * d01;

            float v = (d11 * d20 - d01 * d21) / denominator;
            float w = (d00 * d21 - d01 * d20) / denominator;
            float u = 1.0f - v - w;

            return Vec3(u, v, w);
        }

        bool containsPoint(Vec3 point) {
            Vec3 barycentric = getBarycentricCoords(point);
            return barycentric[1] >= 0.0f && barycentric[2] >= 0.0f && (barycentric[1] + barycentric[2]) <= 1.0f;
        }

        std::string toString() const {
            return "Triangle(" + a.toString() + ", " + b.toString() + ", " + c.toString() + ")";
        }

        friend std::ostream& operator <<(std::ostream& out, const Triangle& tri) {
            out << tri.toString();
            return out;
        }

        bool operator==(const Triangle& tri) {
            if (a != tri.a && a != tri.b && a != tri.c)
                return false;
            if (b != tri.a && b != tri.b && b != tri.c)
                return false;
            if (c != tri.a && c != tri.b && c != tri.c)
                return false;
            return true;
        }

        Vec3 calcNormal() {
            return cross(b - a, c - a).normalized();
        }

        Vec3 calcCentroid() {
            return (a + b + c) / 3.0f;
        }

        static Triangle Equilateral(Vec3 pos, float radius) {
            return Triangle(pos + Vec3(0, radius, 0), pos + Vec3(radius, -radius, 0), pos + Vec3(-radius, -radius, 0));
        }

        Triangle transformed(Mat4 transform) {
            return Triangle(
                    (transform * a.resize<4>(1.0f)).resize<3>(),
                    (transform * b.resize<4>(1.0f)).resize<3>(),
                    (transform * c.resize<4>(1.0f)).resize<3>()
            );
        }
    };
}

#endif //HAGAME2_TRIANGLE_H
