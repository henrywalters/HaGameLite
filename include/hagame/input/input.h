//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_INPUT_H
#define HAGAME2_INPUT_H

#include "memory"
#include "inputDevice.h"
#include "devices/keyboardMouse.h"

namespace hg::input {
    class Input {
    public:

        static devices::KeyboardMouse& KeyboardMouse() {
            static devices::KeyboardMouse keyboardMouse;
            return keyboardMouse;
        }

    };


}

#endif //HAGAME2_INPUT_H
