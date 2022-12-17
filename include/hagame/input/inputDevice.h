//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_INPUTDEVICE_H
#define HAGAME2_INPUTDEVICE_H

#include "../math/aliases.h"

namespace hg::input {
    class InputDevice {
    public:
        Vec2 rAxis;
        Vec2 lAxis;
        Vec2 dPad;

        bool select, selectPressed;
        bool home, homePressed;
        bool start, startPressed;

        bool a, aPressed;
        bool b, bPressed;
        bool x, xPressed;
        bool y, yPressed;

        float rTriggerRaw, lTriggerRaw;
        bool lTrigger, lTriggerPressed;
        bool rTrigger, rTriggerPressed;

        bool lShoulder, lShoulderPressed;
        bool rShoulder, rShoulderPressed;

        void clear() {
            rAxis.zero();
            lAxis.zero();
            dPad.zero();
        }

    protected:
        static void UpdateState(bool& btn, bool& btnPressed, bool state) {
            btnPressed = false;
            if (state) {
                if (!btn) btnPressed = true;
                btn = true;
            }
            else {
                btn = false;
            }
        }
    };
}

#endif //HAGAME2_INPUTDEVICE_H
