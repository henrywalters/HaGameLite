//
// Created by henry on 3/28/23.
//

#ifndef HAGAME2_FUNCTIONS_H
#define HAGAME2_FUNCTIONS_H

#include "aliases.h"

namespace hg {
    bool rectContainsPoint(Vec2 pos, Vec2 size, Vec2 point) {
        return point.x() >= pos.x() && point.x() < pos.x() + size.x() && point.y() >= pos.y() && point.y() < pos.y() + size.y();
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

}


#endif //HAGAME2_FUNCTIONS_H
