//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_INPUT_H
#define HAGAME2_INPUT_H

#include "deviceManager.h"
#include "action.h"

namespace hg::input {

    template <typename Action = hg::utils::enum_t>
    struct Actions {
        std::unordered_map<Action, bool> buttons;
        std::unordered_map<Action, bool> buttonsPressed;
        std::unordered_map<Action, float> axes;
    };

    class Input {
    public:

        // Directly access the keyboard/mouse/gamepads
        DeviceManager devices;

        template <typename Action = hg::utils::enum_t>
        Actions<Action> player(ActionMapper<Action> mapping, int index = 0) {
            Actions<Action> out;
            if ((index == 0 && devices.gamepadActive()) || index > 0) {
                auto rawButtons = devices.gamepad(index)->getButtonState();
                auto rawButtonsPressed = devices.gamepad(index)->getButtonPressedState();
                auto rawAxes = devices.gamepad(index)->getAxesState();
                out.buttons = mapping.getGamepadButtons(rawButtons);
                out.buttonsPressed = mapping.getGamepadButtons(rawButtonsPressed);
                out.axes = mapping.getGamepadAxes(rawAxes);
            } else {
                auto rawButtons = devices.keyboardMouse()->getButtonState();
                auto rawButtonsPressed = devices.keyboardMouse()->getButtonPressedState();
                auto rawAxes = devices.keyboardMouse()->getAxesState();
                out.buttons = mapping.getKeyboardButtons(rawButtons);
                out.buttonsPressed = mapping.getKeyboardButtons(rawButtonsPressed);
                out.axes = mapping.getKeyboardAxes(rawAxes);
            }
            return out;
        }

    };

}

#endif //HAGAME2_INPUT_H
