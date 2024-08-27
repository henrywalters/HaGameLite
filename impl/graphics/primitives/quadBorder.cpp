//
// Created by henry on 8/13/24.
//
#include "../../../../include/hagame/graphics/primitives/quadBorder.h"

void hg::graphics::primitives::QuadBorder::computeMesh() {
    vertices.clear();
    indices.clear();

    auto tex = Rect(Vec2::Zero(), Vec2::Identity());

    Quad::AddToMesh(Rect(m_rect.pos, Vec2(m_rect.size[0], m_thickness)), tex, false, false, *this);
    Quad::AddToMesh(Rect(m_rect.pos + Vec2(0, m_rect.size[1] - m_thickness), Vec2(m_rect.size[0], m_thickness)), tex, false, false, *this);
    Quad::AddToMesh(Rect(m_rect.pos, Vec2(m_thickness, m_rect.size[1])), tex, false, false, *this);
    Quad::AddToMesh(Rect(m_rect.pos + Vec2(m_rect.size[0] - m_thickness, 0), Vec2(m_thickness, m_rect.size[1])), tex, false, false, *this);
}