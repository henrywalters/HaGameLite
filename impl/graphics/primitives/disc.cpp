//
// Created by henry on 12/22/22.
//

#include <cmath>

#include "../../../include/hagame/graphics/primitives/disc.h"

void hg::graphics::primitives::Disc::computeMesh() {

    unsigned int idx = 0;

    float theta = (2 * math::PI) / (float) m_divisions;
    float width = (1.0f / (float) m_divisions) * 2 * math::PI * m_radius;

    for (int i = 0; i < m_divisions; i++) {
        // Add two triangles for top and bottom of cylinder and the face between them

        auto x1 = std::cos(i * theta);
        auto x2 = std::cos((i + 1) * theta);
        auto y1 = std::sin(i * theta);
        auto y2 = std::sin((i + 1) * theta);

        Vec3 a = Vec3(x1 * m_radius, y1 * m_radius, 0.0f);
        Vec3 b = Vec3(x2 * m_radius , y2 * m_radius, 0.0f);

        Triangle tri = Triangle(a, Vec3::Zero(), b);

        tri.setTextures({Vec2(x1, y1), Vec2(0.5, 0.5), Vec2(x2, y2) });

        tri.insert(vertices, indices);
    }
}
