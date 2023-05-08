//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_PRIMITIVES_QUAD_H
#define HAGAME2_PRIMITIVES_QUAD_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Quad : public Mesh {
    public:

        Quad(Vec2 size, Vec2 offset = Vec2::Zero(), bool flipY = false):
            m_size(size),
            m_offset(offset),
            m_flipY(flipY)
        {
            update();
        }

        HG_GET_SET(bool, flipY, update);
        HG_GET_SET(Vec2, offset, update);
        HG_GET_SET(Vec2, size, update);

    private:

        Vec2 m_size;
        Vec2 m_offset;
        bool m_flipY;

    protected:
        void computeMesh() override;
    };
}

#endif //HAGAME2_QUAD_H
