//
// Created by henry on 2/25/24.
//
#include <GLFW/glfw3.h>
#include "../../../include/hagame/input/deviceManager.h"

using namespace hg::input;

DeviceManager::DeviceManager() {
    for (int i = 0; i < MAX_GAMEPAD_COUNT; i++) {
        m_gamepads[i] = std::make_unique<devices::Gamepad>();
        m_gamepads[i]->onInput.subscribe([&](){
            if (m_gamepadOrder.size() > 0) {
                m_gamepadActive = true;
                std::cout << "GAMEPAD ACTIVE\n";
                events.emit(Events::GamepadActive);
            }
        });
        if (hasGamepad(i)) {
            connectGamepad(i);
        }
    }

    m_keyboardMouse = std::make_unique<devices::KeyboardMouse>();
    m_keyboardMouse->onInput.subscribe([&]() {
        m_gamepadActive = false;
        std::cout << "KEYBOARD ACTIVE\n";
        events.emit(Events::KeyboardMouseActive);
    });
}

void DeviceManager::connectGamepad(int gamepadIndex) {
    m_gamepadOrder.push_back(gamepadIndex);
    events.emit(Events::GamepadConnected);
}

void DeviceManager::disconnectGamepad(int gamepadIndex) {
    m_gamepadOrder.erase(std::find(m_gamepadOrder.begin(), m_gamepadOrder.end(), gamepadIndex));
    events.emit(Events::GamepadDisconnected);
}

bool DeviceManager::hasGamepad(int gamepadIndex) const {
    return glfwJoystickPresent(gamepadIndex) && glfwJoystickIsGamepad(gamepadIndex);
}

void DeviceManager::forEachGamepad(std::function<void(int, devices::Gamepad*)> fn) {
    for (auto& index : m_gamepadOrder) {
        fn(index, m_gamepads[index].get());
    }
}

void DeviceManager::clearGamepads() {
    for (auto& index : m_gamepadOrder) {
        m_gamepads[index]->clear();
    }
}

bool DeviceManager::gamepadActive() const {
    return m_gamepadActive;
}

devices::Gamepad *DeviceManager::gamepad(int index) const {
    if (index >= m_gamepadOrder.size()) {
        return nullptr;
    }
    int gamepadIndex = m_gamepadOrder[index];

    if (!hasGamepad(gamepadIndex)) {
        return nullptr;
    } else {
        return m_gamepads[gamepadIndex].get();
    }
}

hg::input::InputDevice *DeviceManager::player(int index) const {
    if (index == 0) {
        return gamepadActive() ? (hg::input::InputDevice*) gamepad(0) : (hg::input::InputDevice*) keyboardMouse();
    }
    return gamepad(index);
}

devices::KeyboardMouse *DeviceManager::keyboardMouse() const {
    return m_keyboardMouse.get();
}
