//
// Created by henry on 8/9/24.
//

#ifndef HAGAME2_CHECKBOX_H
#define HAGAME2_CHECKBOX_H

#include "../element.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/primitives/quadBorder.h"
#include "../../graphics/mesh.h"

namespace hg::ui {
    class Checkbox : public Element {
    public:

        graphics::Color checkedColor = graphics::Color::blue();

        Checkbox(bool* ref, hg::Vec2 size, float borderSize);

        bool contains(Rect rootRect, Vec2 pos) override;

        bool checked() const;
        void checked(bool value);

    protected:
        OBJECT_NAME(Checkbox)

        void onRender(GraphicsContext* context, Rect rootRect, double dt) override;
        bool onTrigger(utils::enum_t event) override;

    private:

        bool* m_ref;
        hg::Vec2 m_size;
        float m_borderSize;
        graphics::primitives::QuadBorder m_borderQuad;
        graphics::primitives::Quad m_fgQuad;
        graphics::MeshInstance m_border, m_fg;

    };
}

#endif //HAGAME2_CHECKBOX_H
