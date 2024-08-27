//
// Created by henry on 7/23/24.
//
#include "../../../../include/hagame/ui/elements/container.h"

using namespace hg::ui;
using namespace hg::graphics;

Container::Container():
    m_quad(Vec2::Identity(), Vec2::Zero()),
    m_mesh(&m_quad)
{
    m_quad.centered(false);
    m_mesh.update(&m_quad);
}

void Container::onRender(GraphicsContext* context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    m_quad.setSizeAndOffset(rect.size, rect.pos);
    m_mesh.update(&m_quad);
    context->colorShader.use();
    context->colorShader.setVec4("color", focused() ? style.focusBackgroundColor : style.backgroundColor);
    m_mesh.render();
    //context->renderer.quads.batch(rect.size, rect.getCenter(), focused() ? style.focusBackgroundColor : style.backgroundColor, Mat4::Translation(Vec3(0, 0, depth())));
}

bool Container::onTrigger(hg::utils::enum_t event) {
    if (event == UITriggers::Previous) {
        m_index--;
        if (m_index == -1) {
            m_index = wrapIndex ? children().size() - 1 : 0;
        }
        ((Element*) children()[m_index])->trigger(UITriggers::Focus);
        return true;
    }

    if (event == UITriggers::Next) {
        m_index++;
        if (m_index == children().size()) {
            m_index = wrapIndex ? 0 : children().size() - 1;
        }
        ((Element*) children()[m_index])->trigger(UITriggers::Focus);
        return true;
    }

    return false;
}

