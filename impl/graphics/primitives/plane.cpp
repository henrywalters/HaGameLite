//
// Created by henry on 9/1/24.
//
#include "../../../include/hagame/graphics/primitives/plane.h"
#include "../../../include/hagame/graphics/primitives/quad.h"

hg::graphics::primitives::Plane::Plane(hg::Vec2 size, hg::Vec2i cells) :
    m_size(size),
    m_cells(cells)
{
    computeMesh();
}

void hg::graphics::primitives::Plane::computeMesh() {
    vertices.clear();
    indices.clear();

    auto cellSize = m_size.div(m_cells.cast<float>());

    for (int x = 0; x < m_cells[0]; x++) {
        for (int y = 0; y < m_cells[1]; y++) {
            primitives::Quad::AddToMesh(
                Rect(Vec2(x, y).prod(cellSize), cellSize),
                Rect(Vec2::Zero(), Vec2::Identity()),
                false, false,
                *this
            );
        }
    }
}