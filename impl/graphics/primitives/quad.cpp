//
// Created by henry on 12/18/22.
//

#include "../../../include/hagame/graphics/primitives/quad.h"

void hg::graphics::primitives::Quad::computeMesh() {

    vertices.clear();
    indices.clear();

    Vec3 a = Vec3( -m_size[0] / 2.0f + m_offset[0], -m_size[1] / 2.0f + m_offset[1], 0.0f );
    Vec3 b = Vec3( -m_size[0] / 2.0f + m_offset[0], m_size[1] / 2.0f + m_offset[1], 0.0f );
    Vec3 c = Vec3( m_size[0] / 2.0f + m_offset[0], m_size[1] / 2.0f + m_offset[1], 0.0f );
    Vec3 d = Vec3( m_size[0] / 2.0f + m_offset[0], -m_size[1] / 2.0f + m_offset[1], 0.0f );

    Triangle triA = Triangle(a, b, c);
    Triangle triB = Triangle(a, c, d);

    triA.setTextures({ Vec2(0, 1), Vec2(0, 0), Vec2(1, 0) });
    triB.setTextures({ Vec2(0, 1), Vec2( 1, 0 ), Vec2( 1, 1 ) });

    triA.insert(vertices, indices);
    triB.insert(vertices, indices);
}
