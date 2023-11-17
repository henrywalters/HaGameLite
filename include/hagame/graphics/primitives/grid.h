//
// Created by henry on 11/5/23.
//

#ifndef HAGAME2_GRID_H
#define HAGAME2_GRID_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Grid : public Mesh {
    public:

        Grid(hg::Vec2 cellSize, hg::Vec2i cells, float thickness = 1):
            m_cellSize(cellSize),
            m_cells(cells),
            m_thickness(thickness)
        {
            update();
        }

        HG_GET_SET(hg::Vec2, cellSize, update);
        HG_GET_SET(hg::Vec2i, cells, update);
        HG_GET_SET(float, thickness, update);

    protected:

        void computeMesh();

    private:

        float m_thickness;
        hg::Vec2 m_cellSize;
        hg::Vec2i m_cells;
    };
}

#endif //HAGAME2_GRID_H
