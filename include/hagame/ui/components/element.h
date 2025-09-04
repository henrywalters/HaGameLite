//
// Created by henry on 9/2/25.
//

#ifndef HAGAME2_ELEMENT_COMPONENT_H
#define HAGAME2_ELEMENT_COMPONENT_H

#include <hagame/core/entity.h>
#include <hagame/core/component.h>
#include <hagame/ui/style.h>
#include "hagame/ui/math.h"

namespace hg::ui::components {
    class Element : public hg::Component, public Style {
    public:

        Rect getRect(Rect rect) const {
            auto parent = static_cast<Entity*>(entity->parent());
            if (positionAbsolute) {
                return absoluteRect;
            } else if (parent && parent->hasComponent<ui::components::Element>()) {
                auto parentElement = parent->getComponent<ui::components::Element>();
                return ui::adjustedRect(parentElement->getRect(rect), marginInPixels, parentElement->paddingInPixels, margin, parentElement->padding);
            } else {
                return ui::adjustedRect(rect, marginInPixels, paddingInPixels, margin, padding);
            }
        }

    protected:

        OBJECT_NAME(Element)
};

HG_COMPONENT(UI, Element)
HG_FIELD(Element, hg::graphics::Color, borderColor)
HG_FIELD(Element, hg::graphics::Color, focusBorderColor)
HG_FIELD(Element, hg::graphics::Color, backgroundColor)
HG_FIELD(Element, hg::graphics::Color, foregroundColor)
HG_FIELD(Element, hg::graphics::Color, focusBackgroundColor)
HG_FIELD(Element, hg::graphics::Color, focusForegroundColor)
HG_FIELD(Element, bool, positionAbsolute)
HG_FIELD(Element, hg::Rect, absoluteRect)
HG_NUMBER_FIELD(Element, int, borderThickness, 0, 100, 1)
HG_FIELD(Element, bool, paddingInPixels)
HG_FIELD(Element, bool, marginInPixels)
HG_FIELD(Element, hg::Vec4, padding)
HG_FIELD(Element, hg::Vec4, margin)

}

#endif
