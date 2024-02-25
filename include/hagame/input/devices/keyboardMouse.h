//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_KEYBOARDMOUSE_H
#define HAGAME2_KEYBOARDMOUSE_H

#include "../../math/aliases.h"
#include "../inputDevice.h"
#include "../../utils/clock.h"
#include "../../utils/pubsub.h"

namespace hg::input::devices {

    constexpr size_t MOUSE_BUTTON_COUNT = 3;
    constexpr size_t MOUSE_AXES_COUNT = 8;

    ENUM(MouseButtons)
    ENUM_VALUE(MouseButtons, Left)
    ENUM_VALUE(MouseButtons, Middle)
    ENUM_VALUE(MouseButtons, Right)

    ENUM(MouseAxes)
    ENUM_VALUE(MouseAxes, PosX)
    ENUM_VALUE(MouseAxes, PosY)
    ENUM_VALUE(MouseAxes, PrevPosX)
    ENUM_VALUE(MouseAxes, PrevPosY)
    ENUM_VALUE(MouseAxes, RAxisX)
    ENUM_VALUE(MouseAxes, RAxisY)
    ENUM_VALUE(MouseAxes, WheelX)
    ENUM_VALUE(MouseAxes, WheelY)

    struct MouseState {

        hg::Notifier onInput;

        long lastMouseMovement = utils::Clock::Now();

        std::array<utils::Watcher<bool>, MOUSE_BUTTON_COUNT> buttons;
        std::array<utils::Watcher<bool>, MOUSE_BUTTON_COUNT> buttonsPressed;
        std::array<utils::Watcher<float>, MOUSE_AXES_COUNT> axes;

        MouseState();

        void setPosition(float x, float y) {
            lastMouseMovement = utils::Clock::Now();
            axes[MouseAxes::PrevPosX] = axes[MouseAxes::PosX];
            axes[MouseAxes::PrevPosY] = axes[MouseAxes::PosY];
            axes[MouseAxes::PosX] = x;
            axes[MouseAxes::PosY] = y;
            auto d = delta();
            axes[MouseAxes::RAxisX] = d[0];
            axes[MouseAxes::RAxisY] = d[1];
        }

        Vec2 position() {
            return Vec2(axes[MouseAxes::PosX], axes[MouseAxes::PosY]);
        }

        Vec2 previousPosition() {
            return Vec2(axes[MouseAxes::PrevPosX], axes[MouseAxes::PrevPosY]);
        }

        Vec2 delta() {
            return position() - previousPosition();
        }

        Vec2 getRelativePosition(Rect viewport) {
            return position() - viewport.pos;
        }
    };

    constexpr size_t KEYBOARD_BUTTON_COUNT = 10 + 26 + 11;
    constexpr size_t KEYBOARD_AXES_COUNT = 2;

    ENUM(KeyboardButtons)
    ENUM_VALUE_OFFSET(KeyboardButtons, RCtrl, MOUSE_BUTTON_COUNT)
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

    ENUM(KeyboardAxes)
    ENUM_VALUE_OFFSET(KeyboardAxes, LAxisX, MOUSE_AXES_COUNT)
    ENUM_VALUE(KeyboardAxes, LAxisY)

    namespace KeyboardButtons {
        const hg::utils::enum_t NumberStart = KeyboardButtons::Escape + 1;
        const hg::utils::enum_t LetterStart = KeyboardButtons::Escape + 2 + 26;
    }

   struct KeyboardState {

        hg::Notifier onInput;

        std::array<utils::Watcher<bool>, KEYBOARD_BUTTON_COUNT> buttons;
        std::array<utils::Watcher<bool>, KEYBOARD_BUTTON_COUNT> buttonsPressed;
        std::array<utils::Watcher<float>, KEYBOARD_AXES_COUNT> axes;

        KeyboardState();
    };

    struct KeyPress {
        unsigned int key;
        bool shiftPressed;
        bool ctrlPressed;
    };

    enum class KeyboardEvent {
        TextInput,
        KeyPressed,
        KeyReleased,
    };

    class KeyboardMouse : public InputDevice {
    public:

        Publisher<KeyboardEvent, KeyPress> events;

        MouseState mouse;
        KeyboardState keyboard;

        KeyboardMouse();

        void clearDevice() override;

        void scrollCallback(double xOffset, double yOffset) {
            mouse.axes[MouseAxes::WheelX] = xOffset;
            mouse.axes[MouseAxes::WheelY] = yOffset;
        }

        void cursorPosCallback(double xPos, double yPos);

        void mouseButtonCallback(int button, int action);

        void keyCallback(int key, int action);

        void charCallback(unsigned int codepoint);

        static int LetterIndex(char letter) {
            return letter - KeyboardButtons::LetterStart;
        }

        static int NumberIndex(char number) {
            return number - KeyboardButtons::NumberStart;
        }

        std::unordered_map<hg::utils::enum_t, bool> getButtonState() const override;
        std::unordered_map<hg::utils::enum_t, bool> getButtonPressedState() const override;
        std::unordered_map<hg::utils::enum_t, float> getAxesState() const override;

    };
}

#endif //HAGAME2_KEYBOARDMOUSE_H
