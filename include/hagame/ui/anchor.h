//
// Created by henry on 12/2/23.
//

#ifndef HAGAME2_ANCHOR_H
#define HAGAME2_ANCHOR_H

#include <vector>
#include <string>
#include "../../include/hagame/math/aliases.h"

namespace hg::ui {
    enum class AnchorType {
        Center,
        TopLeft,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left
    };

    const std::vector<std::string> ANCHOR_TYPES = {
            "Center",
            "Top Left",
            "Top",
            "Top Right",
            "Right",
            "Bottom Right",
            "Bottom",
            "Bottom Left",
            "Left"
    };

    Vec2 calculateAnchorPos(Rect container, Rect rect, AnchorType type, float padding = 0);
}
#endif //HAGAME2_ANCHOR_H
