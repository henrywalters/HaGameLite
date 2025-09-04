//
// Created by henry on 9/2/25.
//

#ifndef HAGAME2_UI_MATH_H
#define HAGAME2_UI_MATH_H

#include <hagame/math/aliases.h>
#include "style.h"

namespace hg::ui {

    inline hg::Rect adjustedRect(Rect rect, bool marginInPixels, bool paddingInPixels, Vec4 margin, Vec4 padding) {
        float x0 = rect.pos[0] + unitValue(marginInPixels, margin[(int) OffsetType::Left], rect.size[0]) +
                   unitValue(paddingInPixels, padding[(int) OffsetType::Left], rect.size[0]);
        float x1 =
                rect.pos[0] + rect.size[0] - unitValue(marginInPixels, margin[(int) OffsetType::Right], rect.size[0]) -
                unitValue(paddingInPixels, padding[(int) OffsetType::Right], rect.size[0]);
        float y0 = rect.pos[1] + unitValue(marginInPixels, margin[(int) OffsetType::Top], rect.size[1]) +
                   unitValue(paddingInPixels, padding[(int) OffsetType::Top], rect.size[1]);
        float y1 =
                rect.pos[1] + rect.size[1] - unitValue(marginInPixels, margin[(int) OffsetType::Bottom], rect.size[1]) -
                unitValue(paddingInPixels, padding[(int) OffsetType::Bottom], rect.size[1]);

        return hg::Rect(Vec2(x0, y0), Vec2(x1 - x0, y1 - y0));
    }

}

#endif //HAGAME2_MATH_H
