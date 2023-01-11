//
// Created by henry on 12/30/22.
//

#ifndef HAGAME2_LINE_H
#define HAGAME2_LINE_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Line : public Mesh {
    public:

        Line(std::vector<Vec3> points): m_points(points) {
            computeMesh();
        }

        HG_GET_SET(std::vector<Vec3>, points, computeMesh);
        HG_GET_SET(float, thickness, computeMesh);

        void addPoint(Vec3 point);
        void addPoints(std::vector<Vec3> points);
        void clearPoints();

    private:

        std::vector<Vec3> m_points;
        float m_thickness = 1.0;

        void computeMesh();

    };
}
#endif //HAGAME2_LINE_H
