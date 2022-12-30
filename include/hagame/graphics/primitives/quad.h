//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_QUAD_H
#define HAGAME2_QUAD_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Quad : public Mesh {
    public:

        Quad(Vec2 size, Vec2 offset = Vec2::Zero()):
            m_size(size),
            m_offset(offset) {
            computeMesh();
        }

        HG_GET_SET(Vec2, offset, computeMesh);
        HG_GET_SET(Vec2, size, computeMesh);

    private:

        Vec2 m_size;
        Vec2 m_offset;

        void computeMesh();
    };
}

#endif //HAGAME2_QUAD_H
