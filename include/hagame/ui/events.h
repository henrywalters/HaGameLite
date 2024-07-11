//
// Created by henry on 2/27/24.
//

#ifndef HAGAME2_EVENTS_H
#define HAGAME2_EVENTS_H

#include "../utils/enum.h"

namespace hg::ui {
    ENUM(UITriggers)
    ENUM_VALUE(UITriggers, Select)
    ENUM_VALUE(UITriggers, Back)
    ENUM_VALUE(UITriggers, Next)
    ENUM_VALUE(UITriggers, Previous)
    ENUM_VALUE(UITriggers, Focus)

    ENUM(UIEvents)
    ENUM_VALUE(UIEvents, Focused)
    ENUM_VALUE(UIEvents, UnFocused)
    ENUM_VALUE(UIEvents, Selected)
}

#endif //HAGAME2_EVENTS_H
