//
// Created by henry on 2/27/24.
//
#include "../../../include/hagame/ui/element.h"

hg::Vec2 hg::ui::Element::mousePos(hg::graphics::Window *window) {
    hg::Vec2 pos = window->input.devices.keyboardMouse()->mousePosition();
    pos[1] = window->size()[1] - pos[1];
    return pos;
}

bool hg::ui::Element::mouseClicked(hg::graphics::Window *window) {
    return window->input.devices.keyboardMouse()->buttons[input::devices::MouseButtons::Left];
}

void hg::ui::Element::trigger(hg::utils::enum_t triggerType) {

    if (triggerType == UITriggers::Focus) { // In the case of a focus, override all other events
        unFocus();
        m_focused = true;
    } else {
        Element* node = this;
        if (!m_focused) {
            auto root = Root(this);
            DepthFirstTraverse(root, [&](auto other) {
                if (((Element*)other)->m_focused) {
                    node = (Element*)other;
                    return false;
                }
                return true;
            });
        }

        std::function<void(Element*)> triggerUp = [&](Element* el) {
            if (!el->onTrigger(triggerType) && el->parent()) {
                triggerUp((Element*) el->parent());
            }
        };

        triggerUp(node);
    }
}

void hg::ui::Element::unFocus() {
    auto root = Root(this);
    Traverse(root, [](auto node) {
        ((Element*) node)->m_focused = false;
    });
}

void hg::ui::Element::focus() {
    trigger(UITriggers::Focus);
}
