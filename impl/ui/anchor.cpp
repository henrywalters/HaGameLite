//
// Created by henry on 12/2/23.
//

#ifndef HAGAME2_ANCHOR_CPP
#define HAGAME2_ANCHOR_CPP

#include "../../include/hagame/ui/anchor.h"

using namespace hg;
using namespace hg::ui;

Vec2 calculateAnchorPos(Rect container, Rect rect, AnchorType type, float padding) {
    switch (type) {
        case AnchorType::Center:
            return (container.getCenter() - rect.size * 0.5f) - container.pos;
        case AnchorType::TopLeft:
            return Vec2(padding, container.size[1] - rect.size[1] - padding);
        case AnchorType::Top:
            return Vec2(container.size[0] * 0.5f - rect.size[0] * 0.5f, container.size[1] - rect.size[1] - padding);
        case AnchorType::TopRight:
            return Vec2(container.size[0] - rect.size[0] - padding, container.size[1] - rect.size[1] - padding);
        case AnchorType::Right:
            return Vec2(container.size[0] - rect.size[0] - padding, container.size[1] * 0.5f - rect.size[1] * 0.5f);
        case AnchorType::BottomLeft:
            return Vec2(padding, padding);
        case AnchorType::Bottom:
            return Vec2(container.size[0] * 0.5f - rect.size[0] * 0.5f, padding);
        case AnchorType::BottomRight:
            return Vec2(container.size[0] - rect.size[0] - padding, padding);
        case AnchorType::Left:
            return Vec2(padding, container.size[1] * 0.5f - rect.size[1] * 0.5f);
        default:
            throw std::runtime_error("Unsupported Anchor Type!");
    }
}

#endif //HAGAME2_ANCHOR_CPP
