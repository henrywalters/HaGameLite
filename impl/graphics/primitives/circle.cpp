//
// Created by henry on 9/11/26.
//
#include <hagame/graphics/primitives/circle.h>

#include "hagame/graphics/triangle.h"

using namespace hg::graphics::primitives;

void Circle::computeMesh() {
    vertices.clear();
    indices.clear();

    for (int i = 0; i < m_divisions; i++) {
        float angleA = (float)i / m_divisions * math::PI * 2.0f;
        float angleB = (float)(i + 1) / m_divisions * math::PI * 2.0f; // wraps to i=0 when i+1 == divisions

        auto normalA = Vec3(cos(angleA), sin(angleA), 0);
        auto normalB = Vec3(cos(angleB), sin(angleB), 0);

        auto pointA = m_offset + normalA * m_radius;
        auto pointB = m_offset + normalB * m_radius;

        auto a = pointA - normalA * m_thickness;
        auto b = pointB - normalB * m_thickness;
        auto c = pointB + normalB * m_thickness;
        auto d = pointA + normalA * m_thickness;

        Triangle triA = Triangle(a, c, b);
        Triangle triB = Triangle(a, c, d);

        triA.setTextures({ Vec2(0, 1), Vec2(0, 0), Vec2(1, 0) });
        triB.setTextures({ Vec2(0, 1), Vec2(1, 0), Vec2(1, 1) });

        triA.insert(vertices, indices);
        triB.insert(vertices, indices);
    }
}