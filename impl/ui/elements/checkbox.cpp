//
// Created by henry on 8/9/24.
//
#include "../../../../include/hagame/ui/elements/checkbox.h"

hg::ui::Checkbox::Checkbox(bool *ref, hg::Vec2 size, float borderSize):
    m_ref(ref),
    m_size(size),
    m_borderSize(borderSize),
    m_bgQuad(),
    m_fgQuad(),
    m_bg(&m_bgQuad),
    m_fg(&m_fgQuad)
{}

void hg::ui::Checkbox::render(hg::ui::GraphicsContext *context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    auto innerSize = hg::Vec2(m_size[0] - m_borderSize * 2, m_size[1] - m_borderSize * 2);
    m_bgQuad.setSizeAndOffset(m_size, rect.getCenter());
    m_fgQuad.setSizeAndOffset(innerSize, rect.getCenter());
    m_bg.update(&m_bgQuad);
    m_fg.update(&m_fgQuad);

    context->colorShader.use();
    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth())));
    context->colorShader.setVec4("color", focused() ? style.focusBackgroundColor : style.backgroundColor);
    m_bg.render();

    graphics::Color color;

    if (focused()) {
        color = *m_ref ? style.focusBackgroundColor : style.focusForegroundColor;
    } else {
        color = *m_ref ? style.backgroundColor : style.foregroundColor;
    }

    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth() + 1)));
    context->colorShader.setVec4("color", color);
    m_fg.render();
}

bool hg::ui::Checkbox::onTrigger(hg::utils::enum_t event) {
    if (event == UIEvents::Selected) {
        (*m_ref) = !(*m_ref);
        return true;
    }
    return false;
}


