//
// Created by henry on 12/6/23.
//
#include "../../../../include/hagame/ui/elements/button.h"

void hg::ui::Button::update(hg::graphics::Window* window, double dt) {
    Vec2 mPos = mousePos(window);
    bool inButton = getRect().contains(mPos);
    if (inButton && !m_mouseInButton) {
        events.emit(ButtonEvents::MouseEnter);
        m_mouseInButton = true;
    } else if (!inButton && m_mouseInButton) {
        events.emit(ButtonEvents::MouseLeave);
        m_mouseInButton = false;
    }
    if (inButton && mouseClicked(window)) {
        events.emit(ButtonEvents::MouseClick);
    }
}
