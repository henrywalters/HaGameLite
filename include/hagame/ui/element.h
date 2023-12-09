//
// Created by henry on 12/2/23.
//

#ifndef HAGAME2_ELEMENT_H
#define HAGAME2_ELEMENT_H

#include "../math/aliases.h"
#include "../graphics/window.h"

namespace hg::ui {

    // The Element is the base of the UI system
    class Element {
    public:

        virtual Rect getRect() { return Rect::Zero(); }
        virtual void update(hg::graphics::Window* window, double dt) {};
        virtual void render(double dt) {};

    protected:

        hg::Vec2 mousePos(hg::graphics::Window* window) {
            hg::Vec2 pos = window->input.keyboardMouse.mouse.position;
            pos[1] = window->size()[1] - pos[1];
            return pos;
        }

        bool mouseClicked(hg::graphics::Window* window) {
            return window->input.keyboardMouse.mouse.leftPressed;
        }

    };

}

#endif //HAGAME2_ELEMENT_H
