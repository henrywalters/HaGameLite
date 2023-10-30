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

        Quad(Vec2 size = Vec2::Zero(), Vec2 offset = Vec2::Zero(), bool flipY = false):
            m_size(size),
            m_offset(offset),
            m_flipY(flipY)
        {
            update();
        }

        HG_GET_SET(bool, flipY, update);
        HG_GET_SET(Vec2, offset, update);
        HG_GET_SET(Vec2, size, update);
        HG_GET_SET(bool, centered, update);
        HG_GET_SET(Vec2, texOffset, update);
        HG_GET_SET(Vec2, texSize, update);

        void setSizeAndOffset(const Vec2& size, const Vec2& offset) {
            m_size = size;
            m_offset = offset;
            update();
        }

    private:

        Vec2 m_size;
        Vec2 m_offset;
        bool m_flipY;
        bool m_centered = true;
        hg::Vec2 m_texOffset = hg::Vec2::Zero();
        hg::Vec2 m_texSize = hg::Vec2::Identity();

    protected:
        void computeMesh() override;
    };
}

#endif //HAGAME2_QUAD_H
