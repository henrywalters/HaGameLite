//
// Created by henry on 2/27/24.
//
#include "../../../include/hagame/ui/element.h"

using namespace hg::graphics;

hg::ui::Element::Element():
    m_quadBorder(Rect::Zero(), 0),
    m_border(&m_quadBorder)
{

}

hg::Vec2 hg::ui::Element::mousePos(hg::graphics::Window *window) {
    hg::Vec2 pos = window->input.devices.keyboardMouse()->mousePosition();
    pos[1] = window->size()[1] - pos[1];
    return pos;
}

bool hg::ui::Element::mouseClicked(hg::graphics::Window *window) {
    return window->input.devices.keyboardMouse()->buttons[input::devices::MouseButtons::Left];
}

void hg::ui::Element::trigger(hg::utils::enum_t triggerType) {

    std::function<void(Element*)> triggerUp = [&](Element* el) {
        if (!el->onTrigger(triggerType) && el->parent()) {
            triggerUp((Element*) el->parent());
        }
    };

    if (triggerType == UITriggers::Focus) { // In the case of a focus, override all other events
        unFocus();
        m_focused = true;
    } else {
        Element* node = this;
        if (!m_focused) {
            auto root = Root(this);
            DepthFirstTraverse<Element>(root, [&](auto other) {
                if ((other)->m_focused) {
                    node = other;
                    // node->trigger(triggerType);
                    return false;
                }
                return true;
            });
        }

        triggerUp(static_cast<Element*>(node));
    }
}

void hg::ui::Element::unFocus() {
    auto root = Root(this);
    Traverse<Element>(root, [](auto node) {
        node->m_focused = false;
    });
}

void hg::ui::Element::focus() {
    trigger(UITriggers::Focus);
}

hg::Rect hg::ui::Element::getRect(Rect rootRect) {
    if (style.positionAbsolute) {
        return style.absoluteRect;
    } else if (!parent()) {
        return adjustedRect(rootRect, offset(0), style.margin);
    } else {
        auto parentNode = static_cast<Element*>(parent());
        return adjustedRect(parentNode->getRect(rootRect), parentNode->style.padding, style.margin);
    }
}

hg::Rect hg::ui::Element::adjustedRect(Rect rect, offset_t margin, offset_t padding) {
    float x0 = rect.pos[0] + margin[(int)OffsetType::Left].value(rect.size[0]) + padding[(int)OffsetType::Left].value(rect.size[0]);
    float x1 = rect.pos[0] + rect.size[0] - margin[(int)OffsetType::Right].value(rect.size[0]) - padding[(int)OffsetType::Right].value(rect.size[0]);
    float y0 = rect.pos[1] + margin[(int)OffsetType::Top].value(rect.size[1]) + padding[(int)OffsetType::Top].value(rect.size[1]);
    float y1 = rect.pos[1] + rect.size[1] - margin[(int)OffsetType::Bottom].value(rect.size[1]) - padding[(int)OffsetType::Bottom].value(rect.size[1]);
    return hg::Rect(Vec2(x0, y0), Vec2(x1 - x0, y1 - y0));
}

bool hg::ui::Element::contains(hg::Rect rootRect, hg::Vec2 pos) {
    return getRect(rootRect).contains(pos);
}

void hg::ui::Element::render(hg::ui::GraphicsContext *context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    if (style.borderThickness > 0) {
    }
    m_quadBorder.rect(rect);
    m_quadBorder.thickness(style.borderThickness);
    m_border.update(&m_quadBorder);
    context->colorShader.use();
    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0,depth() + 1)));
    context->colorShader.setVec4("color", focused() ? style.focusBorderColor : style.borderColor);
    m_border.render();

    onRender(context, rootRect, dt);
}

