//
// Created by henry on 9/1/24.
//

#ifndef HAGAME2_PLANE_H
#define HAGAME2_PLANE_H

#include "../mesh.h"

namespace hg::graphics::primitives {
    class Plane : public Mesh {
    public:

        Plane(Vec2 size = hg::Vec2::Identity(), hg::Vec2i cells = hg::Vec2i::Identity());

    private:

        Vec2 m_size;
        Vec2i m_cells;

        void computeMesh() override;

    };
}

#endif //HAGAME2_PLANE_H
