//
// Created by henry on 2/23/24.
//
#include <GLFW/glfw3.h>
#include "../../../../include/hagame/input/devices/keyboardMouse.h"

using namespace hg::input;
using namespace hg::input::devices;

MouseState::MouseState()
{
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++) {
        buttons[i] = utils::Watcher<bool>(false, [&](){ onInput.emit(); });
        buttonsPressed[i] = utils::Watcher<bool>(false, [&](){ onInput.emit(); });
    }

    for (int i = 0; i < MOUSE_AXES_COUNT; i++) {
        axes[i] = utils::Watcher<float>(false, [&](){ onInput.emit(); });
    }
}

KeyboardState::KeyboardState()
{
    for (int i = 0; i < KEYBOARD_BUTTON_COUNT; i++) {
        buttons[i] = utils::Watcher<bool>(false, [&](){ onInput.emit(); });
        buttonsPressed[i] = utils::Watcher<bool>(false, [&](){ onInput.emit(); });
    }

    for (int i = 0; i < KEYBOARD_AXES_COUNT; i++) {
        axes[i] = utils::Watcher<float>(false, [&](){ onInput.emit(); });
    }
}

KeyboardMouse::KeyboardMouse() {
    mouse.onInput.subscribe([&](){
        onInput.emit();
    });
    keyboard.onInput.subscribe([&](){
        onInput.emit();
    });
}

void KeyboardMouse::clearDevice() {
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++) {
        mouse.buttonsPressed[i] = false;
    }
    for (int i = 0; i < KEYBOARD_BUTTON_COUNT; i++) {
        keyboard.buttonsPressed[i] = false;
    }
}

void KeyboardMouse::cursorPosCallback(double xPos, double yPos)
{
    mouse.setPosition(xPos, yPos);
}

const std::unordered_map<hg::utils::enum_t, int> MOUSE_BUTTON_MAP {
    { MouseButtons::Left, GLFW_MOUSE_BUTTON_LEFT },
    { MouseButtons::Middle, GLFW_MOUSE_BUTTON_MIDDLE },
    { MouseButtons::Right, GLFW_MOUSE_BUTTON_RIGHT  },
};

void hg::input::devices::KeyboardMouse::mouseButtonCallback(int button, int action)
{
    for (const auto &[id, glfwId] : MOUSE_BUTTON_MAP) {
        if (button == glfwId) {
            UpdateState(mouse.buttons[id], mouse.buttonsPressed[id], action != 0);
        }
    }
}

const std::unordered_map<hg::utils::enum_t, int> KEYBOARD_BUTTON_MAP {
    {KeyboardButtons::Space, GLFW_KEY_SPACE},
    { KeyboardButtons::LCtrl, GLFW_KEY_LEFT_CONTROL },
    { KeyboardButtons::RCtrl, GLFW_KEY_RIGHT_CONTROL },
    { KeyboardButtons::LShift, GLFW_KEY_LEFT_SHIFT },
    { KeyboardButtons::RShift, GLFW_KEY_RIGHT_SHIFT },
    { KeyboardButtons::LAlt, GLFW_KEY_LEFT_ALT },
    { KeyboardButtons::RAlt, GLFW_KEY_RIGHT_ALT },
    { KeyboardButtons::Escape, GLFW_KEY_ESCAPE },
    { KeyboardButtons::Backspace, GLFW_KEY_BACKSPACE },
    { KeyboardButtons::Enter, GLFW_KEY_ENTER },
    { KeyboardButtons::Tilda, GLFW_KEY_APOSTROPHE },
};

void hg::input::devices::KeyboardMouse::keyCallback(int key, int action)
{

    for (const auto& [id, glfwId] : KEYBOARD_BUTTON_MAP) {
        if (key == glfwId) {
            UpdateState(keyboard.buttons[id], keyboard.buttonsPressed[id], action != 0);
        }
    }

    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        auto id = key - GLFW_KEY_0 + KeyboardButtons::NumberStart;
        UpdateState(keyboard.buttons[id], keyboard.buttonsPressed[id], action != 0);
    }

    if (key >= GLFW_KEY_A && key < GLFW_KEY_Z) {
        auto id = key - GLFW_KEY_A + KeyboardButtons::LetterStart;
        UpdateState(keyboard.buttons[id], keyboard.buttons[id], action != 0);
    }

    keyboard.axes[KeyboardAxes::LAxisX] = ((int) keyboard.buttons[LetterIndex('D')]) - ((int) keyboard.buttons[LetterIndex(('A'))]);
    keyboard.axes[KeyboardAxes::LAxisY] = ((int) keyboard.buttons[LetterIndex('W')]) - ((int) keyboard.buttons[LetterIndex(('S'))]);

    KeyPress press;
    press.key = key;
    press.shiftPressed = keyboard.buttons[KeyboardButtons::LShift] || keyboard.buttons[KeyboardButtons::RShift];
    press.ctrlPressed = keyboard.buttons[KeyboardButtons::LCtrl] || keyboard.buttons[KeyboardButtons::RCtrl];

    events.emit(action != 0 ? KeyboardEvent::KeyPressed : KeyboardEvent::KeyReleased, press);

}

void hg::input::devices::KeyboardMouse::charCallback(unsigned int codepoint)
{
    KeyPress press;
    press.key = codepoint;
    press.shiftPressed = keyboard.buttons[KeyboardButtons::LShift] || keyboard.buttons[KeyboardButtons::RShift];
    press.ctrlPressed = keyboard.buttons[KeyboardButtons::LCtrl] || keyboard.buttons[KeyboardButtons::RCtrl];
    events.emit(KeyboardEvent::TextInput, press);
}

std::unordered_map<hg::utils::enum_t, bool> KeyboardMouse::getButtonState() const {

    std::unordered_map<hg::utils::enum_t, bool> out;
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++) {
        if (mouse.buttons[i].value()) {
            out[i] = true;
        }
    }

    for (int i = MOUSE_BUTTON_COUNT; i < MOUSE_BUTTON_COUNT + KEYBOARD_BUTTON_COUNT; i++) {
        if (keyboard.buttons[i].value()) {
            out[i] = true;
        }
    }

    return out;
}

std::unordered_map<hg::utils::enum_t, bool> KeyboardMouse::getButtonPressedState() const {
    std::unordered_map<hg::utils::enum_t, bool> out;
    for (int i = 0; i < MOUSE_BUTTON_COUNT; i++) {
        if (mouse.buttonsPressed[i].value()) {
            out[i] = true;
        }
    }

    for (int i = MOUSE_BUTTON_COUNT; i < MOUSE_BUTTON_COUNT + KEYBOARD_BUTTON_COUNT; i++) {
        if (keyboard.buttonsPressed[i].value()) {
            out[i] = true;
        }
    }

    return out;
}

std::unordered_map<hg::utils::enum_t, float> KeyboardMouse::getAxesState() const {
    std::unordered_map<hg::utils::enum_t, float> out;
    for (int i = 0; i < MOUSE_AXES_COUNT; i++) {
        out[i] = mouse.axes[i].value();
    }

    for (int i = MOUSE_AXES_COUNT; i < MOUSE_AXES_COUNT + KEYBOARD_AXES_COUNT; i++) {
        out[i] = keyboard.axes[i].value();
    }

    return out;
}
