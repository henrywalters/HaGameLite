//
// Created by henry on 12/18/22.
//

#include <array>
#include "../../../include/hagame/graphics/primitives/cube.h"

void hg::graphics::primitives::Cube::computeMesh() {
    vertices.clear();
    indices.clear();

    Vec3 a = Vec3( -m_size[0] / 2.0f, -m_size[1] / 2.0f, m_size[0] / 2.0f );
    Vec3 b = Vec3( -m_size[0] / 2.0f, m_size[1] / 2.0f, m_size[0] / 2.0f );
    Vec3 c = Vec3( m_size[0] / 2.0f, m_size[1] / 2.0f, m_size[0] / 2.0f );
    Vec3 d = Vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, m_size[0] / 2.0f );
    Vec3 e = Vec3( -m_size[0] / 2.0f, -m_size[1] / 2.0f, -m_size[0] / 2.0f );
    Vec3 f = Vec3( -m_size[0] / 2.0f, m_size[1] / 2.0f, -m_size[0] / 2.0f );
    Vec3 g = Vec3( m_size[0] / 2.0f, m_size[1] / 2.0f, -m_size[0] / 2.0f );
    Vec3 h = Vec3( m_size[0] / 2.0f, -m_size[1] / 2.0f, -m_size[0] / 2.0f );

    std::array<Triangle, 12> triangles = {
        Triangle(c, b, a), // Face 0 Front
        Triangle(d, c, a),
        Triangle(b, f, e), // Face 1 Left
        Triangle(a, b, e),
        Triangle(g, c, d), // Face 2 Right
        Triangle(h, g, d),
        Triangle(f, g, h), // Face 3 Back
        Triangle(e, f, h),
        Triangle(g, f, b), // Face 4 Top
        Triangle(c, g, b),
        Triangle(d, a, e), // Face 5 Bottom
        Triangle(h, d, e),
    };

    for (int i = 0; i < triangles.size(); i++) {
        if (i % 2 == 0) {
            triangles[i].setTextures({ Vec2(1, 1), Vec2( 0, 1 ), Vec2( 0, 0 ) });

        } else {
            triangles[i].setTextures({ Vec2(1, 0), Vec2(1, 1), Vec2(0, 0) });
        }

        triangles[i].insert(vertices, indices);
    }
}
