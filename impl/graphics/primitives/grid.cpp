//
// Created by henry on 11/5/23.
//

#include "../../../../include/hagame/graphics/primitives/grid.h"

void hg::graphics::primitives::Grid::computeMesh() {
    vertices.clear();
    indices.clear();

    Vec3 a, b, c, d;

    for (int x = 0; x <= m_cells[0]; x++) {
        float offset = x * m_cellSize[0];
        a = Vec3(offset - m_thickness / 2, -m_thickness / 2, 0);
        b = Vec3(offset + m_thickness / 2, -m_thickness / 2, 0);
        c = Vec3(offset + m_thickness / 2, m_cells[1] * m_cellSize[1] + m_thickness / 2, 0);
        d = Vec3(offset - m_thickness / 2, m_cells[1] * m_cellSize[1] + m_thickness / 2, 0);

        Triangle triA = Triangle(a, b, c);
        Triangle triB = Triangle(a, c, d);

        triA.setTextures({{0, 0}, {1, 0}, {1, 1}});
        triB.setTextures({{0, 0,}, {1, 1}, {0, 1}});

        triA.insert(vertices, indices);
        triB.insert(vertices, indices);
    }

    for (int y = 0; y <= m_cells[1]; y++) {
        float offset = y * m_cellSize[1];
        a = Vec3(-m_thickness / 2, offset - m_thickness / 2, 0);
        b = Vec3(-m_thickness / 2, offset + m_thickness / 2, 0);
        c = Vec3(m_cells[0] * m_cellSize[0] + m_thickness / 2, offset + m_thickness / 2, 0);
        d = Vec3(m_cells[0] * m_cellSize[0] + m_thickness / 2, offset - m_thickness / 2, 0);

        Triangle triA = Triangle(a, b, c);
        Triangle triB = Triangle(a, c, d);

        triA.setTextures({{0, 0}, {1, 0}, {1, 1}});
        triB.setTextures({{0, 0,}, {1, 1}, {0, 1}});

        triA.insert(vertices, indices);
        triB.insert(vertices, indices);
    }

}
