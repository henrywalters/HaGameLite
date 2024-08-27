//
// Created by henry on 8/13/24.
//

#ifndef HAGAME2_DIVIDER_H
#define HAGAME2_DIVIDER_H

#include "../element.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"

namespace hg::ui {
    class Divider : public Element {
    public:

        Divider(float thickness);

        HG_GET_SET_SIMPLE(float, thickness);

    protected:

        OBJECT_NAME(Divider)

        void onRender(GraphicsContext* context, Rect rootRect, double dt) override;

    private:

        graphics::primitives::Quad m_quad;
        graphics::MeshInstance m_mesh;

        float m_thickness;
    };
}

#endif //HAGAME2_DIVIDER_H
