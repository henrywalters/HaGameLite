//
// Created by henry on 12/2/23.
//

#ifndef HAGAME2_ELEMENT_H
#define HAGAME2_ELEMENT_H

#include "../math/aliases.h"

namespace hg::ui {

    // The Element is the base of the UI system
    class Element {
    public:

        virtual Vec2 getSize() { return Vec2::Zero(); }
        virtual void render(double dt) {};

    };
}

#endif //HAGAME2_ELEMENT_H
