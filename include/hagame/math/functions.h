//
// Created by henry on 3/28/23.
//

#ifndef HAGAME2_FUNCTIONS_H
#define HAGAME2_FUNCTIONS_H

#include "aliases.h"

namespace hg {

    template <typename T>
    inline constexpr T lerp(T a, T b, T t) {
        return a + (b - a) * t;
    }

    bool rectContainsPoint(Vec2 pos, Vec2 size, Vec2 point);

    inline Vec2 rotate(const Vec2& vec, float theta) {
        return Vec2(std::cos(theta) * vec[0] - std::sin(theta) * vec[1], std::sin(theta) * vec[0] + std::cos(theta) * vec[1]);
    }

    template <typename T>
    inline constexpr bool approxEqual(T a, T b, T epsilon) {
        return a >= b - epsilon && a <= b + epsilon;
    }

    template <class T>
    std::vector<Vec2i> bresenham(const math::Vector<2, T>& start, const math::Vector<2, T>& end) {
        std::vector<Vec2i> points;

        int x0 = start[0];
        int y0 = start[1];
        int x1 = end[0];
        int y1 = end[1];

        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;

        while (x0 != x1 || y0 != y1) {
            points.push_back(Vec2i(x0, y0));
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
        points.push_back(Vec2i(x0, y0));

        return points;
    }

    // Map a size to a bounds while perserving the aspect ratio
    template <size_t ND, typename T>
    math::Vector<ND, T> mapSizeToBounds(math::Vector<ND, T> size, math::Vector<ND, T> bounds) {
        T minRatio;
        bool hasMin = false;

        for (int i = 0; i < ND; i++) {
            if (!hasMin || bounds[i] / size[i] < minRatio) {
                hasMin = true;
                minRatio = bounds[i] / size[i];
            }
        }

        return size * minRatio;
    }

    template <size_t ND, typename T>
    math::Hypercube<ND, T> mapRectToBounds(math::Vector<ND, T> size, math::Vector<ND, T> bounds) {
        auto boundedSize = mapSizeToBounds(size, bounds);
        return math::Hypercube((bounds - boundedSize) * 0.5, boundedSize);
    }
}


#endif //HAGAME2_FUNCTIONS_H
