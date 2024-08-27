//
// Created by henry on 8/9/24.
//
#include "../../../../include/hagame/ui/elements/checkbox.h"

hg::ui::Checkbox::Checkbox(bool *ref, hg::Vec2 size, float borderSize):
    m_ref(ref),
    m_size(size),
    m_borderSize(borderSize),
    m_borderQuad(Rect::Zero(), 1),
    m_border(&m_borderQuad),
    m_fgQuad(),
    m_fg(&m_fgQuad)
{}

void hg::ui::Checkbox::onRender(hg::ui::GraphicsContext *context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    auto innerSize = hg::Vec2(m_size[0] - m_borderSize * 2, m_size[1] - m_borderSize * 2);
    auto center = rect.getCenter();
    m_fgQuad.setSizeAndOffset(innerSize, center);
    m_fg.update(&m_fgQuad);

    m_borderQuad.rect(Rect(rect.getCenter() - m_size * 0.5, m_size));
    m_borderQuad.thickness(m_borderSize);
    m_border.update(&m_borderQuad);

    context->colorShader.use();
    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth())));
    context->colorShader.setVec4("color", focused() ? style.focusBackgroundColor : style.backgroundColor);
    m_border.render();

    graphics::Color color;

    if (checked()) {
        color = checkedColor;
    } else {
        color = focused() ? style.focusForegroundColor : style.foregroundColor;
    }

    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth() + 1)));
    context->colorShader.setVec4("color", color);
    m_fg.render();
}

bool hg::ui::Checkbox::contains(hg::Rect rootRect, hg::Vec2 pos) {
    Rect cboxRect(getRect(rootRect).getCenter() - m_size * 0.5, m_size);
    return cboxRect.contains(pos);
}

bool hg::ui::Checkbox::onTrigger(hg::utils::enum_t event) {
    if (event == UITriggers::Select) {
        checked(!checked());
        return true;
    }
    return false;
}

bool hg::ui::Checkbox::checked() const {
    return *m_ref;
}

void hg::ui::Checkbox::checked(bool value) {
    (*m_ref) = value;
}

