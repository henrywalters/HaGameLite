//
// Created by henry on 12/6/23.
//

#ifndef HAGAME2_BUTTON_H
#define HAGAME2_BUTTON_H

#include "../../input/devices/keyboardMouse.h"
#include "../element.h"
#include "../../utils/macros.h"
#include "../../utils/pubsub.h"

namespace hg::ui {

    enum class ButtonEvents {
        MouseEnter,
        MouseLeave,
        MouseClick,
    };

    class Button : public Element {
    public:

        EventEmitter<ButtonEvents> events;

        void update(hg::graphics::Window* window, double dt) override;

    protected:

        bool m_mouseInButton;

    };
}

#endif //HAGAME2_BUTTON_H
