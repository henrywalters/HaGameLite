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
    // btnPressed is only ever set here; clearDevice() resets it once the frame has been processed. Resetting it on
    // every event meant a press and release arriving in the same poll (a quick click, or a touchpad tap) cancelled out
    // and the game never saw the press.
    if (state) {
        if (!btn) btnPressed = true;
        btn = true;
    }
    else {
        btn = false;
    }
}


