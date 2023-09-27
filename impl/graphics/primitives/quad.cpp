//
// Created by henry on 12/18/22.
//

#include "../../../include/hagame/graphics/primitives/quad.h"

void hg::graphics::primitives::Quad::computeMesh() {

    vertices.clear();
    indices.clear();

    Vec3 a, b, c, d;

    if (m_centered) {
        a = Vec3( -m_size[0] / 2.0f + m_offset[0], -m_size[1] / 2.0f + m_offset[1], 0.0f );
        b = Vec3( -m_size[0] / 2.0f + m_offset[0], m_size[1] / 2.0f + m_offset[1], 0.0f );
        c = Vec3( m_size[0] / 2.0f + m_offset[0], m_size[1] / 2.0f + m_offset[1], 0.0f );
        d = Vec3( m_size[0] / 2.0f + m_offset[0], -m_size[1] / 2.0f + m_offset[1], 0.0f );
    } else {
        a = Vec3(m_offset[0], m_offset[1], 0.0f);
        b = Vec3(m_offset[0], m_size[1] + m_offset[1], 0.0f);
        c = Vec3(m_offset[0] + m_size[0], m_size[1] + m_offset[1], 0.0f);
        d = Vec3(m_offset[0] + m_size[0], m_offset[1], 0.0f);
    }

    Triangle triA = Triangle(a, b, c);
    Triangle triB = Triangle(a, c, d);

    //triA.setTextures({ Vec2(0, m_flipY ? 0 : 1), Vec2(0, m_flipY ? 1 : 0), Vec2(1, m_flipY ? 1 : 0) });
    //triB.setTextures({ Vec2(0, m_flipY ? 0 : 1), Vec2( 1, m_flipY ? 1 : 0 ), Vec2( 1, m_flipY ? 0 : 1 ) });

    hg::Vec2 minTex(m_texOffset[0], !m_flipY ? m_texOffset[1] + m_texSize[1] : m_texOffset[1]);
    hg::Vec2 maxTex(m_texOffset[0] + m_texSize[0], !m_flipY ? m_texOffset[1] : m_texOffset[1] + m_texSize[1]);
    //hg::Vec2 minTex = m_texOffset;
    //hg::Vec2 maxTex = m_texOffset + m_texSize;

    triA.setTextures({
         Vec2(minTex[0], minTex[1]),
         Vec2(minTex[0], maxTex[1]),
         Vec2(maxTex[0], maxTex[1])
     });
    triB.setTextures({
         Vec2(minTex[0], minTex[1]),
         Vec2(maxTex[0], maxTex[1]),
         Vec2(maxTex[0], minTex[1]),
     });

    triA.insert(vertices, indices);
    triB.insert(vertices, indices);
}
