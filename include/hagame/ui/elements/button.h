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

    class Button : public Element {
    protected:

        OBJECT_NAME(Button)

        bool onTrigger(utils::enum_t event) override;

    };
}

#endif //HAGAME2_BUTTON_H
