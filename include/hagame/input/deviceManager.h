//
// Created by henry on 2/25/24.
//

#ifndef HAGAME2_DEVICEMANAGER_H
#define HAGAME2_DEVICEMANAGER_H

#include <deque>
#include "devices/keyboardMouse.h"
#include "devices/gamepad.h"

constexpr size_t MAX_GAMEPAD_COUNT = 16;

namespace hg::graphics {
    class Window;
}

namespace hg::input {
    class DeviceManager {
    public:

        friend class graphics::Window;

        enum class Events {
            KeyboardMouseActive,
            GamepadActive,
            GamepadConnected,
            GamepadDisconnected,
        };

        hg::EventEmitter<Events> events;
        devices::KeyboardMouse keyboardMouse;

        devices::Gamepad* gamepad(int index) const;
        hg::input::InputDevice* player(int index) const;

        DeviceManager();

        bool gamepadActive() const;
        bool hasGamepad(int gamepadIndex) const;
        void connectGamepad(int gamepadIndex);
        void disconnectGamepad(int gamepadIndex);
        void clearGamepads();

        void forEachGamepad(std::function<void(int, devices::Gamepad*)> fn);

    private:

        bool m_gamepadActive;
        std::array<std::unique_ptr<devices::Gamepad>, MAX_GAMEPAD_COUNT> m_gamepads;
        std::deque<int> m_gamepadOrder;

    };
}

#endif //HAGAME2_DEVICEMANAGER_H
