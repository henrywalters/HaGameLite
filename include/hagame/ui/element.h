//
// Created by henry on 12/2/23.
//

#ifndef HAGAME2_ELEMENT_H
#define HAGAME2_ELEMENT_H

#include "../math/aliases.h"
#include "../graphics/window.h"
#include "../utils/pubsub.h"
#include "../structures/tree.h"
#include "events.h"

namespace hg::ui {

    // The Element is the base of the UI system
    class Element : public structures::Tree {
    public:

        EventEmitter<utils::enum_t> events;

        void focus();
        void trigger(utils::enum_t triggerType);

        virtual Rect getRect() { return Rect::Zero(); }
        virtual void render(double dt) {};
        virtual bool onTrigger(utils::enum_t triggerType) { return false; }

        bool focused() const { return m_focused; }

    protected:

        hg::Vec2 mousePos(hg::graphics::Window* window);
        bool mouseClicked(hg::graphics::Window* window);

    private:

        Element* element;

        bool m_focused = true;

        void unFocus();

    };

}

#endif //HAGAME2_ELEMENT_H
