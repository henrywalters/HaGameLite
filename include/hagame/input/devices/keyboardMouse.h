//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_KEYBOARDMOUSE_H
#define HAGAME2_KEYBOARDMOUSE_H

#include <iostream>

#include "../../math/aliases.h"
#include "../inputDevice.h"
#include "../../utils/clock.h"

namespace hg::input::devices {

    struct MouseState {
        int wheel;
        Vec2 position;
        Vec2 prevPosition;
        Vec2 delta;

        bool left = false;
        bool leftPressed = false;
        bool middle = false;
        bool middlePressed = false;
        bool right = false;
        bool rightPressed = false;

        Vec2 getRelativePosition(Rect viewport) {
            return position - viewport.pos;
        }
    };

    struct KeyboardState {
        std::array<bool, 10> numbers = {};
        std::array<bool, 10> numbersPressed = {};
        std::array<bool, 26> letters = {};
        std::array<bool, 26> lettersPressed = {};

        bool lCtrl, lCtrlPressed = false;
        bool rCtrl, rCtrlPressed = false;
        bool lShift, lShiftPressed = false;
        bool rShift, rShiftPressed = false;
        bool enter, enterPressed = false;

        bool esc, escPressed = false;
    };

    class KeyboardMouse : public InputDevice {
    public:

        long lastMouseMovement = utils::Clock::Now();
        Vec2 lastMousePos = Vec2::Zero();

        MouseState mouse;
        KeyboardState keyboard;

        void clearDevice() override {
            mouse.leftPressed = false;
            mouse.rightPressed = false;
            mouse.middlePressed = false;
            mouse.wheel = 0;

            for (int i = 0; i < 10; i++) {
                keyboard.numbersPressed[i] = false;
            }

            for (int i = 0; i < 26; i++) {
                keyboard.lettersPressed[i] = false;
            }
        }

        void scrollCallback(double xOffset, double yOffset) {
            mouse.wheel = yOffset;
        }

        void cursorPosCallback(double xPos, double yPos) {
            auto newMousePos = Vec2(xPos, yPos);
            auto mouseDelta = newMousePos - lastMousePos;
            auto now = utils::Clock::Now();
            auto dt = utils::Clock::ToSeconds(now - lastMouseMovement);

            lastMousePos = mouse.position;
            lastMouseMovement = now;
            mouse.position = newMousePos;

            rAxis = mouseDelta;
        }

        void mouseButtonCallback(int button, int action) {
            if (button == M_LEFT) {
                UpdateState(mouse.left, mouse.leftPressed, action != 0);
                UpdateState(rTrigger, rTriggerPressed, action != 0);
            }

            if (button == M_RIGHT) {
                UpdateState(mouse.right, mouse.rightPressed, action != 0);
                UpdateState(lTrigger, lTriggerPressed, action != 0);
            }

            if (button == M_MIDDLE) {
                UpdateState(mouse.middle, mouse.middlePressed, action != 0);
            }
        }

        void keyCallback(int key, int action) {

            if (key >= ZERO_START && key < ZERO_START + 10) {
                UpdateState(keyboard.numbers[key - ZERO_START], keyboard.numbersPressed[key - ZERO_START], action != 0);
            }

            if (key >= A_START && key < A_START + 26) {
                UpdateState(keyboard.letters[key - A_START], keyboard.lettersPressed[key - A_START], action != 0);
            }

            if (key == L_CTRL) {
                UpdateState(keyboard.lCtrl, keyboard.lCtrlPressed, action != 0);
            }

            if (key == R_CTRL) {
                UpdateState(keyboard.rCtrl, keyboard.rCtrlPressed, action != 0);
            }

            if (key == L_SHIFT) {
                UpdateState(keyboard.lShift, keyboard.lShiftPressed, action != 0);
            }

            if (key == R_SHIFT) {
                UpdateState(keyboard.rShift, keyboard.rShiftPressed, action != 0);
            }

            if (key == ESC) {
                UpdateState(keyboard.esc, keyboard.escPressed, action != 0);
            }

            if (key == ENTER) {
                UpdateState(keyboard.enter, keyboard.enterPressed, action != 0);
            }

            lAxis[0] = ((int) keyboard.letters[LetterIndex('D')]) - ((int) keyboard.letters[LetterIndex(('A'))]);
            lAxis[1] = ((int) keyboard.letters[LetterIndex('W')]) - ((int) keyboard.letters[LetterIndex(('S'))]);
        }

        static constexpr int LetterIndex(char letter) {
            return letter - 65;
        }
    private:

        static const int A_START = 65;
        static const int ZERO_START = 48;
        static const int L_CTRL = 341;
        static const int R_CTRL = 345;
        static const int L_SHIFT = 340;
        static const int R_SHIFT = 344;
        static const int ESC = 256;
        static const int ENTER = 257;

        static const int M_LEFT = 0;
        static const int M_MIDDLE = 2;
        static const int M_RIGHT = 1;



    };
}

#endif //HAGAME2_KEYBOARDMOUSE_H
