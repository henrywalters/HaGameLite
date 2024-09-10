//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_INPUTDEVICE_H
#define HAGAME2_INPUTDEVICE_H

#include "../math/aliases.h"
#include "../utils/pubsub.h"
#include "../utils/watcher.h"
#include "../utils/enum.h"

namespace hg::input {

    using ButtonState = std::unordered_map<hg::utils::enum_t, bool>;
    using AxesState = std::unordered_map<hg::utils::enum_t, float>;

    struct InputDeviceState {
        ButtonState buttons;
        AxesState axes;
    };

    class InputDevice {
    public:

        Notifier onInput;

        void clear();

        virtual ButtonState getButtonState() const = 0;
        virtual ButtonState getButtonPressedState() const = 0;
        virtual AxesState getAxesState() const = 0;

    protected:

        static void UpdateState(utils::Watcher<bool>& btn, utils::Watcher<bool>& btnPressed, bool state);

        virtual void clearDevice() {};
    };
}

#endif //HAGAME2_INPUTDEVICE_H
