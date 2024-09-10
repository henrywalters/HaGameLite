//
// Created by henry on 2/23/24.
//
#include "../../../include/hagame/input/inputDevice.h"

using namespace hg::input;

void hg::input::InputDevice::clear()
{
    clearDevice();
}

void hg::input::InputDevice::UpdateState(utils::Watcher<bool>& btn, utils::Watcher<bool>& btnPressed, bool state)
{
    btnPressed = false;
    if (state) {
        if (!btn) btnPressed = true;
        btn = true;
    }
    else {
        btn = false;
    }
}


