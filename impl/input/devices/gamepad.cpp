//
// Created by henry on 2/23/24.
//
#include "../../../../include/hagame/input/devices/gamepad.h"
#include <GLFW/glfw3.h>

using namespace hg::input;
using namespace hg::input::devices;

const std::unordered_map<hg::utils::enum_t, int> BUTTON_MAP {
    { GamepadButtons::Start, GLFW_GAMEPAD_BUTTON_START},
    { GamepadButtons::Select, GLFW_GAMEPAD_BUTTON_BACK},
    { GamepadButtons::Home,GLFW_GAMEPAD_BUTTON_GUIDE },
    { GamepadButtons::A, GLFW_GAMEPAD_BUTTON_A },
    { GamepadButtons::B, GLFW_GAMEPAD_BUTTON_B },
    { GamepadButtons::X, GLFW_GAMEPAD_BUTTON_X },
    { GamepadButtons::Y, GLFW_GAMEPAD_BUTTON_Y },
    { GamepadButtons::LThumb, GLFW_GAMEPAD_BUTTON_LEFT_THUMB },
    { GamepadButtons::RThumb, GLFW_GAMEPAD_BUTTON_RIGHT_THUMB },
    { GamepadButtons::LShoulder, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER },
    { GamepadButtons::RShoulder, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER },
    { GamepadButtons::Up, GLFW_GAMEPAD_BUTTON_DPAD_UP },
    { GamepadButtons::Down, GLFW_GAMEPAD_BUTTON_DPAD_DOWN },
    { GamepadButtons::Left, GLFW_GAMEPAD_BUTTON_DPAD_LEFT },
    { GamepadButtons::Right, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
};

const std::unordered_map<hg::utils::enum_t, int> AXES_MAP {
    { GamepadAxes::RAxisX, GLFW_GAMEPAD_AXIS_RIGHT_X },
    { GamepadAxes::RAxisY, GLFW_GAMEPAD_AXIS_RIGHT_Y },
    { GamepadAxes::LAxisX, GLFW_GAMEPAD_AXIS_LEFT_X },
    { GamepadAxes::LAxisY, GLFW_GAMEPAD_AXIS_LEFT_Y },
};

Gamepad::Gamepad() {
    for (const auto& [button, glfwId] : BUTTON_MAP) {
        buttons[button] = utils::Watcher<bool>(false, [&]() { onInput.emit(); });
    }

    for (const auto& [axesId, glfwId] : AXES_MAP) {
        axes[axesId] = utils::Watcher<float>(0, [&]() { onInput.emit(); });
    }
}


void hg::input::devices::Gamepad::setGamepadState(const GLFWgamepadstate &state) {
    for (const auto& [button, glfwId] : BUTTON_MAP) {
        UpdateState(buttons[button], buttonsPressed[button], state.buttons[glfwId] == GLFW_PRESS);
    }

    for (const auto& [axesId, glfwId] : AXES_MAP) {
        float value = state.axes[glfwId];
        if (axesId == GamepadAxes::RAxisY || axesId == GamepadAxes::LAxisY) {
            value *= -1;
        }
        axes[axesId] = std::abs(value) < axisDeadzone ? 0 : value;
    }
}

std::unordered_map<hg::utils::enum_t, bool> Gamepad::getButtonState() const {
    std::unordered_map<hg::utils::enum_t, bool> out;
    for (const auto& [id, _] : BUTTON_MAP) {
        out[id] = buttons.at(id).value();
    }
    return out;
}

std::unordered_map<hg::utils::enum_t, bool> Gamepad::getButtonPressedState() const {
    std::unordered_map<hg::utils::enum_t, bool> out;
    for (const auto& [id, _] : BUTTON_MAP) {
        out[id] = buttonsPressed.at(id).value();
    }
    return out;
}

std::unordered_map<hg::utils::enum_t, float> Gamepad::getAxesState() const {
    std::unordered_map<hg::utils::enum_t, float> out;
    for (const auto& [id, _] : AXES_MAP) {
        out[id] = axes.at(id).value();
    }
    return out;
}