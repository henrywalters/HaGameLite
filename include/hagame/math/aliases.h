//
// Created by henry on 12/14/22.
//

#ifndef HAGAME2_ALIASES_H
#define HAGAME2_ALIASES_H

#include "vector.h"
#include "hypercube.h"
#include "hypersphere.h"
#include "matrix.h"
#include "quaternion.h"

namespace hg {
    using Vec4 = math::Vector<4, float>;
    using Vec3 = math::Vector<3, float>;
    using Vec2 = math::Vector<2, float>;

    using Vec4i = math::Vector<4, int>;
    using Vec3i = math::Vector<3, int>;
    using Vec2i = math::Vector<2, int>;

    using Vec4d = math::Vector<4, double>;
    using Vec3d = math::Vector<3, double>;
    using Vec2d = math::Vector<2, double>;

    using Vec4b = math::Vector<4, bool>;
    using Vec3b = math::Vector<3, bool>;
    using Vec2b = math::Vector<2, bool>;

    using Mat2 = math::Matrix<2, 2, float>;
    using Mat3 = math::Matrix<3, 3, float>;
    using Mat4 = math::Matrix<4, 4, float>;

    using Circle = math::Hypersphere<2, float>;
    using Sphere = math::Hypersphere<3, float>;

    using Rect = math::Hypercube<2, float>;
    using Cube = math::Hypercube<3, float>;

    using Recti = math::Hypercube<2, int>;
    using Cubei = math::Hypercube<3, int>;

    using Quat = math::Quaternion<float>;

    inline float cross2(Vec2 a, Vec2 b) {
        return a.x() * b.y() - a.y() * b.x();
    }

    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    bool nearEqual(T a, T b, T epsilon) {
        return a >= b - epsilon && a <= b + epsilon;
    }
}

#endif //HAGAME2_ALIASES_H
