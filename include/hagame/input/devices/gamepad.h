//
// Created by henry on 2/22/24.
//

#ifndef HAGAME2_GAMEPAD_H
#define HAGAME2_GAMEPAD_H

#include "../inputDevice.h"
#include "../../utils/enum.h"
#include "../../utils/watcher.h"

struct GLFWgamepadstate;

namespace hg::input::devices {

    constexpr size_t GAMEPAD_BUTTON_COUNT = 15;
    constexpr size_t GAMEPAD_AXES_COUNT = 6;

    ENUM(GamepadButtons)
    ENUM_VALUE(GamepadButtons, Select)
    ENUM_VALUE(GamepadButtons, Home)
    ENUM_VALUE(GamepadButtons, Start)
    ENUM_VALUE(GamepadButtons, A)
    ENUM_VALUE(GamepadButtons, B)
    ENUM_VALUE(GamepadButtons, X)
    ENUM_VALUE(GamepadButtons, Y)
    ENUM_VALUE(GamepadButtons, RThumb)
    ENUM_VALUE(GamepadButtons, LThumb)
    ENUM_VALUE(GamepadButtons, RShoulder)
    ENUM_VALUE(GamepadButtons, LShoulder)
    ENUM_VALUE(GamepadButtons, Up)
    ENUM_VALUE(GamepadButtons, Down)
    ENUM_VALUE(GamepadButtons, Left)
    ENUM_VALUE(GamepadButtons, Right)

    ENUM(GamepadAxes)
    ENUM_VALUE(GamepadAxes, LAxisX)
    ENUM_VALUE(GamepadAxes, LAxisY)
    ENUM_VALUE(GamepadAxes, RAxisX)
    ENUM_VALUE(GamepadAxes, RAxisY)
    ENUM_VALUE(GamepadAxes, RTrigger)
    ENUM_VALUE(GamepadAxes, LTrigger)

    class Gamepad : public InputDevice {
    public:

        double axisDeadzone = 0.001;

        Gamepad();

        void setGamepadState(const GLFWgamepadstate& state);

        std::array<utils::Watcher<bool>, GAMEPAD_BUTTON_COUNT> buttons, buttonsPressed;
        std::array<utils::Watcher<float>, GAMEPAD_AXES_COUNT> axes;

        std::unordered_map<hg::utils::enum_t, bool> getButtonState() const override;
        std::unordered_map<hg::utils::enum_t, bool> getButtonPressedState() const override;
        std::unordered_map<hg::utils::enum_t, float> getAxesState() const override;

    };
}

#endif //HAGAME2_GAMEPAD_H
