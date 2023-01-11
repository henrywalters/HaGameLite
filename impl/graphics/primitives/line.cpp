//
// Created by henry on 12/30/22.
//
#include "../../../include/hagame/graphics/primitives/line.h"
#include "../../../include/hagame/math/conversions.h"

void hg::graphics::primitives::Line::computeMesh() {
    vertices.clear();
    indices.clear();

    for (int i = 1; i < m_points.size(); i++) {
        auto pointA = m_points[i - 1];
        auto pointB = m_points[i];
        auto delta = pointB - pointA;
        auto angle = atan2(delta[1], delta[0]);

        double normalAngleA, normalAngleB;
        float thicknessA = m_thickness;
        float thicknessB = m_thickness;

        normalAngleA = angle + math::PI * 0.5;
        if (i > 1) {
            auto delta2 = pointA - m_points[i - 2];
            auto angleBetween = delta2.angleBetween(delta) * 0.5;
            normalAngleA = normalAngleA - angleBetween;
            thicknessA = m_thickness / std::abs(cos(angleBetween));
        }

        normalAngleB = angle + math::PI * 0.5;
        if (i < m_points.size() - 1) {
            auto delta2 = m_points[i + 1] - pointB;
            auto angleBetween = delta.angleBetween(delta2) * 0.5;
            normalAngleB = normalAngleB + angleBetween;
            thicknessB = m_thickness / std::abs(cos(angleBetween));
        }


        auto normalA = Vec3(cos(normalAngleA), sin(normalAngleA), 0);
        auto normalB = Vec3(cos(normalAngleB), sin(normalAngleB), 0);

        auto a = pointA - normalA * thicknessA;
        auto b = pointB - normalB * thicknessB;
        auto c = pointB + normalB * thicknessB;
        auto d = pointA + normalA * thicknessA;

        Triangle triA = Triangle(a, c, b);
        Triangle triB = Triangle(a, c, d);

        triA.setTextures({ Vec2(0, 1), Vec2(0, 0), Vec2(1, 0) });
        triB.setTextures({ Vec2(0, 1), Vec2( 1, 0 ), Vec2( 1, 1 ) });

        triA.insert(vertices, indices);
        triB.insert(vertices, indices);
    }
}

void hg::graphics::primitives::Line::addPoint(hg::Vec3 point) {
    m_points.push_back(point);
    computeMesh();
}

void hg::graphics::primitives::Line::addPoints(std::vector<Vec3> points) {
    m_points.insert(m_points.end(), points.begin(), points.end());
    computeMesh();
}

void hg::graphics::primitives::Line::clearPoints() {
    m_points.clear();
    computeMesh();
}
