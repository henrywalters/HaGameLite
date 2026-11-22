//
// Created by henry on 8/3/24.
//

#ifndef HAGAME2_FRAME_H
#define HAGAME2_FRAME_H

#include <set>
#include "element.h"
#include "elements/container.h"

namespace hg::ui {

    // The UI Frame is a light wrapper around a root Container element
    class Frame {
    public:

        Frame() = default;

        Rect rect;

        template <IsElement El, class... Args>
        El* addElement(Args &&... args) {
            auto el = new El(std::forward<Args>(args)...);
            m_elements.push_back(el);
            m_root.addChild(el);
            return el;
        }

        template <IsElement El, class... Args>
        El* addElementTo(Element* parent, Args &&... args) {
            auto el = new El(std::forward<Args>(args)...);
            m_elements.push_back(static_cast<Element*>(el));
            parent->addChild(el);
            return el;
        }

        Container* root();

        void render(double dt, bool debug = false);

        // Sends MouseEnter / MouseLeave, and with mouseDown (the left button held), presses and drags to the element
        // under the mouse that takes them (see Element::onPointerDown)
        void update(Vec2 mousePos, double dt, bool mouseDown = false);

        bool contains(Element* element, hg::Vec2 pos) const;

        void resize();

        // Removes and deletes every element, leaving the root (and its style) to build the frame again
        void clear();

    private:

        GraphicsContext m_context;
        std::vector<Element*> m_elements;
        std::set<hg::utils::uuid_t> m_hovered;
        bool m_mouseDown = false;
        Element* m_captured = nullptr; // Took the current press, and gets its drags until release
        Container m_root;

        void initializeShader(graphics::ShaderProgram* shader);

    };
}

#endif //HAGAME2_FRAME_H
