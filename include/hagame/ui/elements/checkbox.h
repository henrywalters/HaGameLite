//
// Created by henry on 8/9/24.
//

#ifndef HAGAME2_CHECKBOX_H
#define HAGAME2_CHECKBOX_H

#include "../element.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"

namespace hg::ui {
    class Checkbox : public Element {
    public:

        Checkbox(bool* ref, hg::Vec2 size, float borderSize);

        void render(GraphicsContext* context, Rect rootRect, double dt) override;

    protected:
        OBJECT_NAME(Checkbox)

        bool onTrigger(utils::enum_t event) override;

    private:

        bool* m_ref;
        hg::Vec2 m_size;
        float m_borderSize;
        graphics::primitives::Quad m_bgQuad, m_fgQuad;
        graphics::MeshInstance m_bg, m_fg;

    };
}

#endif //HAGAME2_CHECKBOX_H
