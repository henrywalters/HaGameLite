//
// Created by henry on 2/26/24.
//

#ifndef HAGAME2_KEYBOARDMOUSEENUMS_H
#define HAGAME2_KEYBOARDMOUSEENUMS_H

#include "../../utils/enum.h"

namespace hg::input::devices {
    constexpr size_t _MOUSE_BUTTON_COUNT = 3;
    constexpr size_t _MOUSE_AXES_COUNT = 8;

    ENUM(MouseButtons)
    ENUM_VALUE(MouseButtons, Left)
    ENUM_VALUE(MouseButtons, Middle)
    ENUM_VALUE(MouseButtons, Right)

    ENUM(MouseAxes)
    ENUM_VALUE(MouseAxes, PosX)
    ENUM_VALUE(MouseAxes, PosY)
    ENUM_VALUE(MouseAxes, PrevPosX)
    ENUM_VALUE(MouseAxes, PrevPosY)
    ENUM_VALUE(MouseAxes, DeltaX)
    ENUM_VALUE(MouseAxes, DeltaY)
    ENUM_VALUE(MouseAxes, WheelX)
    ENUM_VALUE(MouseAxes, WheelY)

    constexpr size_t _KEYBOARD_LETTER_COUNT = 26;
    constexpr size_t _KEYBOARD_NUMBER_COUNT = 10;
    constexpr size_t _KEYBOARD_SPECIAL_COUNT = 11;
    constexpr size_t _KEYBOARD_BUTTON_COUNT = _KEYBOARD_LETTER_COUNT + _KEYBOARD_NUMBER_COUNT + _KEYBOARD_SPECIAL_COUNT;
    constexpr size_t _KEYBOARD_AXES_COUNT = 2;

    ENUM(KeyboardButtons)
    ENUM_VALUE_OFFSET(KeyboardButtons, RCtrl, _MOUSE_BUTTON_COUNT)
    ENUM_VALUE(KeyboardButtons, Space)
    ENUM_VALUE(KeyboardButtons, LCtrl)
    ENUM_VALUE(KeyboardButtons, RShift)
    ENUM_VALUE(KeyboardButtons, LShift)
    ENUM_VALUE(KeyboardButtons, RAlt)
    ENUM_VALUE(KeyboardButtons, LAlt)
    ENUM_VALUE(KeyboardButtons, Enter)
    ENUM_VALUE(KeyboardButtons, Backspace)
    ENUM_VALUE(KeyboardButtons, Tilda)
    ENUM_VALUE(KeyboardButtons, Escape)
    ENUM_VALUE(KeyboardButtons, A)
    ENUM_VALUE(KeyboardButtons, B)
    ENUM_VALUE(KeyboardButtons, C)
    ENUM_VALUE(KeyboardButtons, D)
    ENUM_VALUE(KeyboardButtons, E)
    ENUM_VALUE(KeyboardButtons, F)
    ENUM_VALUE(KeyboardButtons, G)
    ENUM_VALUE(KeyboardButtons, H)
    ENUM_VALUE(KeyboardButtons, I)
    ENUM_VALUE(KeyboardButtons, J)
    ENUM_VALUE(KeyboardButtons, K)
    ENUM_VALUE(KeyboardButtons, L)
    ENUM_VALUE(KeyboardButtons, M)
    ENUM_VALUE(KeyboardButtons, N)
    ENUM_VALUE(KeyboardButtons, O)
    ENUM_VALUE(KeyboardButtons, P)
    ENUM_VALUE(KeyboardButtons, Q)
    ENUM_VALUE(KeyboardButtons, R)
    ENUM_VALUE(KeyboardButtons, S)
    ENUM_VALUE(KeyboardButtons, T)
    ENUM_VALUE(KeyboardButtons, U)
    ENUM_VALUE(KeyboardButtons, V)
    ENUM_VALUE(KeyboardButtons, W)
    ENUM_VALUE(KeyboardButtons, X)
    ENUM_VALUE(KeyboardButtons, Y)
    ENUM_VALUE(KeyboardButtons, Z)
    ENUM_VALUE(KeyboardButtons, One)
    ENUM_VALUE(KeyboardButtons, Two)
    ENUM_VALUE(KeyboardButtons, Three)
    ENUM_VALUE(KeyboardButtons, Four)
    ENUM_VALUE(KeyboardButtons, Five)
    ENUM_VALUE(KeyboardButtons, Six)
    ENUM_VALUE(KeyboardButtons, Seven)
    ENUM_VALUE(KeyboardButtons, Eight)
    ENUM_VALUE(KeyboardButtons, Nine)
    ENUM_VALUE(KeyboardButtons, Zero)

    ENUM(KeyboardAxes)
    ENUM_VALUE_OFFSET(KeyboardAxes, WASD_X, _MOUSE_AXES_COUNT)
    ENUM_VALUE(KeyboardAxes, WASD_Y)
}

#endif //HAGAME2_KEYBOARDMOUSEENUMS_H
