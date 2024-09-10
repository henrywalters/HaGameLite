//
// Created by henry on 12/18/22.
//

#include "../../../include/hagame/graphics/primitives/quad.h"

void hg::graphics::primitives::Quad::computeMesh() {

    vertices.clear();
    indices.clear();

    Quad::AddToMesh(Rect(m_offset, m_size), Rect(m_texOffset, m_texSize), m_flipY, m_centered, *this);
}

void hg::graphics::primitives::Quad::AddToMesh(hg::Rect rect, hg::Rect tex, bool flipY, bool centered,
                                               hg::graphics::Mesh &mesh) {
    Vec3 a, b, c, d;

    if (centered) {
        a = Vec3( -rect.size[0] / 2.0f + rect.pos[0], -rect.size[1] / 2.0f + rect.pos[1], 0.0f );
        b = Vec3( -rect.size[0] / 2.0f + rect.pos[0], rect.size[1] / 2.0f + rect.pos[1], 0.0f );
        c = Vec3( rect.size[0] / 2.0f + rect.pos[0], rect.size[1] / 2.0f + rect.pos[1], 0.0f );
        d = Vec3( rect.size[0] / 2.0f + rect.pos[0], -rect.size[1] / 2.0f + rect.pos[1], 0.0f );
    } else {
        a = Vec3(rect.pos[0], rect.pos[1], 0.0f);
        b = Vec3(rect.pos[0], rect.size[1] + rect.pos[1], 0.0f);
        c = Vec3(rect.pos[0] + rect.size[0], rect.size[1] + rect.pos[1], 0.0f);
        d = Vec3(rect.pos[0] + rect.size[0], rect.pos[1], 0.0f);
    }

    Triangle triA = Triangle(a, b, c);
    Triangle triB = Triangle(a, c, d);

    //triA.setTextures({ Vec2(0, m_flipY ? 0 : 1), Vec2(0, m_flipY ? 1 : 0), Vec2(1, m_flipY ? 1 : 0) });
    //triB.setTextures({ Vec2(0, m_flipY ? 0 : 1), Vec2( 1, m_flipY ? 1 : 0 ), Vec2( 1, m_flipY ? 0 : 1 ) });

    hg::Vec2 minTex(tex.pos[0], !flipY ? tex.pos[1] + tex.size[1] : tex.pos[1]);
    hg::Vec2 maxTex(tex.pos[0] + tex.size[0], !flipY ? tex.pos[1] : tex.pos[1] + tex.size[1]);
    //hg::Vec2 minTex = m_texOffset;
    //hg::Vec2 maxTex = m_texOffset + tex.size;

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

    triA.insert(mesh.vertices, mesh.indices);
    triB.insert(mesh.vertices, mesh.indices);
}
