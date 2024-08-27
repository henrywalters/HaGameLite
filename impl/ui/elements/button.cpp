//
// Created by henry on 12/6/23.
//
#include "../../../../include/hagame/ui/elements/button.h"

bool hg::ui::Button::onTrigger(hg::utils::enum_t event) {
    if (event == UITriggers::Select) {
        std::cout << "SELECTED\n";
        events.emit(UIEvents::Selected);
        return true;
    }
    return false;
}
