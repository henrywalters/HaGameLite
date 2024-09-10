//
// Created by henry on 8/13/24.
//
#include "../../../../include/hagame/ui/elements/divider.h"

hg::ui::Divider::Divider(float thickness):
    m_quad(Vec2::Zero(), Vec2::Zero()),
    m_mesh(&m_quad),
    m_thickness(thickness)
{
    m_quad.centered(true);
}

void hg::ui::Divider::onRender(hg::ui::GraphicsContext *context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    m_quad.setSizeAndOffset(Vec2(rect.size[0], thickness()), rect.pos + rect.size * 0.5);
    m_mesh.update(&m_quad);
    context->colorShader.use();
    context->colorShader.setVec4("color", focused() ? style.focusForegroundColor : style.foregroundColor);
    m_mesh.render();
}
