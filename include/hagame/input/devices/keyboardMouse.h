//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_KEYBOARD_MOUSE_H
#define HAGAME2_KEYBOARD_MOUSE_H

#include "../../math/aliases.h"
#include "../inputDevice.h"
#include "../../utils/clock.h"
#include "../../utils/pubsub.h"
#include "keyboardMouseEnums.h"

namespace hg::graphics {
    class Windows;
}

namespace hg::input::devices {

    namespace KeyboardButtons {
        const hg::utils::enum_t LetterStart = _MOUSE_BUTTON_COUNT + _KEYBOARD_SPECIAL_COUNT;
        const hg::utils::enum_t NumberStart = _MOUSE_BUTTON_COUNT + _KEYBOARD_SPECIAL_COUNT + _KEYBOARD_LETTER_COUNT;
    }

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

        KeyboardMouse();

        friend class hg::graphics::Windows;

        Publisher<KeyboardEvent, KeyPress> events;

        std::array<utils::Watcher<bool>, _KEYBOARD_BUTTON_COUNT + _MOUSE_BUTTON_COUNT> buttons;
        std::array<utils::Watcher<bool>, _KEYBOARD_BUTTON_COUNT + _MOUSE_BUTTON_COUNT> buttonsPressed;
        std::array<utils::Watcher<float>, _MOUSE_AXES_COUNT + _KEYBOARD_AXES_COUNT> axes;

        Vec2 mousePosition() const;
        Vec2 mouseDelta() const;
        Vec2 wasd() const;

        std::unordered_map<hg::utils::enum_t, bool> getButtonState() const override;
        std::unordered_map<hg::utils::enum_t, bool> getButtonPressedState() const override;
        std::unordered_map<hg::utils::enum_t, float> getAxesState() const override;

    private:

        void clearDevice() override;

        void scrollCallback(double xOffset, double yOffset);

        void cursorPosCallback(double xPos, double yPos);

        void mouseButtonCallback(int button, int action);

        void keyCallback(int key, int action);

        void charCallback(unsigned int codepoint);

    };
}

#endif //HAGAME2_KEYBOARD_MOUSE_H
