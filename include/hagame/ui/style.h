//
// Created by henry on 7/21/24.
//

#ifndef HAGAME2_STYLE_H
#define HAGAME2_STYLE_H

#include "../math/aliases.h"
#include "../utils/enum.h"
#include "../graphics/color.h"
#include <array>

namespace hg::ui {
    ENUM(OffsetType)
    ENUM_VALUE(OffsetType, Left)
    ENUM_VALUE(OffsetType, Top)
    ENUM_VALUE(OffsetType, Right)
    ENUM_VALUE(OffsetType, Bottom)

    /*

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
                throw std::runtime_error("[HAGAME ERROR]: Unhandled Offset unit type");
            }
        }
    };

    using offset_t = std::array<Offset, 4>;*/

    constexpr float unitValue(bool usePixels, float value, float size) {
        if (usePixels) {
            return value;
        } else {
            return value * size;
        }
    }

    struct Style {
        graphics::Color borderColor = graphics::Color(0, 0, 0, 1);
        graphics::Color focusBorderColor = graphics::Color(0, 0, 0, 1);
        graphics::Color backgroundColor = graphics::Color(0, 0, 0, 1);
        graphics::Color foregroundColor = graphics::Color::white();
        graphics::Color focusBackgroundColor = graphics::Color(0, 0, 0, 1);
        graphics::Color focusForegroundColor = graphics::Color::white();
        bool positionAbsolute = false;
        int borderThickness = 0;
        hg::Rect absoluteRect;

        bool paddingInPixels = true;
        bool marginInPixels = true;
        Vec4 padding;
        Vec4 margin;
    };

/*    inline offset_t offset(float left, float top, float right, float bottom, Unit::type unit = Unit::Pixels) {
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
    }*/
}

#endif //HAGAME2_STYLE_H
