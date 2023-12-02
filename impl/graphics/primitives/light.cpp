//
// Created by henry on 12/1/23.
//
#include "../../../include/hagame/graphics/primitives/light.h"

hg::graphics::primitives::Light::Light(std::vector<math::LineSegment> triangles, Vec3 origin, float radius):
    m_triangles(triangles),
    m_origin(origin),
    m_radius(radius)
{
    computeMesh();
}

void hg::graphics::primitives::Light::computeMesh() {
    vertices.clear();
    indices.clear();

    for (const auto& triangle : m_triangles) {
        Triangle tri(triangle.b, m_origin, triangle.a);
        std::array<Vec3, 2> deltas = {
            triangle.a - m_origin,
            triangle.b - m_origin
        };

        for (int i = 0; i < 2; i++) {
            deltas[i] = std::clamp<float>(deltas[i].magnitude(), 0, m_radius) * deltas[i].normalized();
        }

        tri.setTextures({Vec2::Zero(), deltas[0].resize<2>(), deltas[1].resize<2>()});

        tri.insert(vertices, indices);
    }
}
