//
// Created by henry on 7/21/24.
//

#ifndef HAGAME2_STYLE_H
#define HAGAME2_STYLE_H

#include "../math/aliases.h"
#include "../utils/enum.h"
#include <array>

namespace hg::ui {

    ENUM(OffsetType)
    ENUM_VALUE(OffsetType, Left)
    ENUM_VALUE(OffsetType, Top)
    ENUM_VALUE(OffsetType, Right)
    ENUM_VALUE(OffsetType, Bottom)

    ENUM(Unit)
    ENUM_VALUE(Unit, Pixels)
    ENUM_VALUE(Unit, Percent)

    struct Offset {
        Unit::type unit;
        float rawValue;

        float value(float containerSize) const {
            if (unit == Unit::Pixels) {
                return rawValue;
            } else if (unit == Unit::Percent) {
                return rawValue * containerSize;
            } else {
                throw std::runtime_error("Unhandled Offset unit type");
            }
        }
    };

    using offset_t = std::array<Offset, 4>;

    struct Style {
        bool positionAbsolute = false;
        hg::Rect absoluteRect;
        offset_t padding = {0, 0, 0, 0};
        offset_t margin = {0, 0, 0, 0};
    };

    inline offset_t offset(float left, float top, float right, float bottom, Unit::type unit = Unit::Pixels) {
        return offset_t{
                Offset{unit, left},
                Offset{unit, top},
                Offset{unit, right},
                Offset{unit, bottom},
        };
    }

    inline offset_t offset(float amt, Unit::type unit = Unit::Pixels) {
        return offset_t{
                Offset{unit, amt},
                Offset{unit, amt},
                Offset{unit, amt},
                Offset{unit, amt},
        };
    }
}

#endif //HAGAME2_STYLE_H
