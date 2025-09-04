//
// Created by henry on 2/23/24.
//
#include <GLFW/glfw3.h>
#include "../../../../include/hagame/input/devices/keyboardMouse.h"

using namespace hg::input;
using namespace hg::input::devices;

const std::unordered_map<int, hg::utils::enum_t> MOUSE_BUTTON_MAP {
        { GLFW_MOUSE_BUTTON_LEFT, MouseButtons::Left },
        { GLFW_MOUSE_BUTTON_MIDDLE, MouseButtons::Middle },
        { GLFW_MOUSE_BUTTON_RIGHT, MouseButtons::Right },
};


const std::unordered_map<int, hg::utils::enum_t> KEYBOARD_BUTTON_MAP {
        { GLFW_KEY_SPACE, KeyboardButtons::Space },
        { GLFW_KEY_LEFT_CONTROL, KeyboardButtons::LCtrl },
        { GLFW_KEY_RIGHT_CONTROL, KeyboardButtons::RCtrl },
        { GLFW_KEY_LEFT_SHIFT, KeyboardButtons::LShift },
        { GLFW_KEY_RIGHT_SHIFT, KeyboardButtons::RShift },
        { GLFW_KEY_LEFT_ALT, KeyboardButtons::LAlt },
        { GLFW_KEY_RIGHT_ALT, KeyboardButtons::RAlt },
        { GLFW_KEY_ESCAPE, KeyboardButtons::Escape },
        { GLFW_KEY_BACKSPACE, KeyboardButtons::Backspace },
        { GLFW_KEY_ENTER, KeyboardButtons::Enter },
        { GLFW_KEY_APOSTROPHE, KeyboardButtons::Tilda },
};

KeyboardMouse::KeyboardMouse() {
    for (auto& btn : buttons) {
        btn.onUpdate = [&](){
            onInput.emit();
        };
    }
}

void KeyboardMouse::clearDevice() {
    axes[MouseAxes::WheelX] = 0;
    axes[MouseAxes::WheelY] = 0;
    for (int i = 0; i < buttonsPressed.size(); i++) {
        buttonsPressed[i] = false;
    }
}

void KeyboardMouse::cursorPosCallback(double xPos, double yPos)
{
    float currentX = axes[MouseAxes::PosX];
    float currentY = axes[MouseAxes::PosY];
    axes[MouseAxes::PosX] = xPos;
    axes[MouseAxes::PosY] = yPos;
    axes[MouseAxes::PrevPosX] = currentX;
    axes[MouseAxes::PrevPosY] = currentY;
    axes[MouseAxes::DeltaX] = xPos - currentX;
    axes[MouseAxes::DeltaY] = yPos - currentY;
}


void hg::input::devices::KeyboardMouse::mouseButtonCallback(int button, int action)
{
    if (MOUSE_BUTTON_MAP.find(button) != MOUSE_BUTTON_MAP.end()) {
        UpdateState(buttons[MOUSE_BUTTON_MAP.at(button)], buttonsPressed[MOUSE_BUTTON_MAP.at(button)], action != 0);
    }
}

void hg::input::devices::KeyboardMouse::keyCallback(int key, int action)
{

    if (KEYBOARD_BUTTON_MAP.find(key) != KEYBOARD_BUTTON_MAP.end()) {
        UpdateState(buttons[KEYBOARD_BUTTON_MAP.at(key)], buttonsPressed[KEYBOARD_BUTTON_MAP.at(key)], action != 0);
    }

    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        auto id = key - GLFW_KEY_0 + KeyboardButtons::NumberStart;
        UpdateState(buttons[id], buttonsPressed[id], action != 0);
    }

    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {

        auto id = key - GLFW_KEY_A + KeyboardButtons::LetterStart;
        UpdateState(buttons[id], buttonsPressed[id], action != 0);

        axes[KeyboardAxes::WASD_X] = buttons[KeyboardButtons::D] - buttons[KeyboardButtons::A];
        axes[KeyboardAxes::WASD_Y] = buttons[KeyboardButtons::W] - buttons[KeyboardButtons::S];
    }

    KeyPress press;
    press.key = key;
    press.shiftPressed = buttons[KeyboardButtons::LShift] || buttons[KeyboardButtons::RShift];
    press.ctrlPressed = buttons[KeyboardButtons::LCtrl] || buttons[KeyboardButtons::RCtrl];

    events.emit(action != 0 ? KeyboardEvent::KeyPressed : KeyboardEvent::KeyReleased, press);

}

void hg::input::devices::KeyboardMouse::charCallback(unsigned int codepoint)
{
    KeyPress press;
    press.key = codepoint;
    press.shiftPressed = buttons[KeyboardButtons::LShift] || buttons[KeyboardButtons::RShift];
    press.ctrlPressed = buttons[KeyboardButtons::LCtrl] || buttons[KeyboardButtons::RCtrl];
    events.emit(KeyboardEvent::TextInput, press);
}

std::unordered_map<hg::utils::enum_t, bool> KeyboardMouse::getButtonState() const {

    std::unordered_map<hg::utils::enum_t, bool> out;
    for (int i = 0; i < _MOUSE_BUTTON_COUNT + _KEYBOARD_BUTTON_COUNT; i++) {
        if (buttons[i].value()) {
            out[i] = true;
        } else {
            out[i] = false;
        }
    }
    return out;
}

std::unordered_map<hg::utils::enum_t, bool> KeyboardMouse::getButtonPressedState() const {
    std::unordered_map<hg::utils::enum_t, bool> out;
    for (int i = 0; i < _MOUSE_BUTTON_COUNT + _KEYBOARD_BUTTON_COUNT; i++) {
        if (buttonsPressed[i].value()) {
            out[i] = true;
        } else {
            out[i] = false;
        }
    }
    return out;
}

std::unordered_map<hg::utils::enum_t, float> KeyboardMouse::getAxesState() const {
    std::unordered_map<hg::utils::enum_t, float> out;
    for (int i = 0; i < _MOUSE_AXES_COUNT + _KEYBOARD_AXES_COUNT; i++) {
        out[i] = axes[i].value();
    }

    return out;
}

void KeyboardMouse::scrollCallback(double xOffset, double yOffset) {
    axes[MouseAxes::WheelX] = xOffset;
    axes[MouseAxes::WheelY] = yOffset;
}

hg::Vec2 KeyboardMouse::mousePosition() const {
    return Vec2(axes[MouseAxes::PosX].value(), axes[MouseAxes::PosY].value());
}

hg::Vec2 KeyboardMouse::mouseDelta() const {
    return Vec2(axes[MouseAxes::DeltaX].value(), axes[MouseAxes::DeltaY].value());
}

hg::Vec2 KeyboardMouse::wasd() const {
    return Vec2(axes[KeyboardAxes::WASD_X].value(), axes[KeyboardAxes::WASD_Y].value());
}
