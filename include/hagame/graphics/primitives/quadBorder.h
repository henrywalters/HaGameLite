//
// Created by henry on 8/13/24.
//

#ifndef HAGAME2_QUADBORDER_H
#define HAGAME2_QUADBORDER_H

#include "quad.h"

namespace hg::graphics::primitives {
    class QuadBorder : public Mesh {
    public:

        QuadBorder(Rect rect, float thickness) :
                m_rect(rect),
                m_thickness(thickness) {
            update();
        }

        HG_GET_SET(Rect, rect, update)
        HG_GET_SET(float, thickness, update)

    private:

        Rect m_rect;
        float m_thickness;

    protected:
        void computeMesh() override;
    };
}

#endif //HAGAME2_QUADBORDER_H
