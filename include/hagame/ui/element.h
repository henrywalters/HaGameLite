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
#include "style.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/batchRenderer.h"
#include "../graphics/primitives/quadBorder.h"
#include "graphicsContext.h"

namespace hg::ui {

    // The Element is the base of the UI system
class Element : public structures::Tree, public hg::Object {
    public:

        Element();

        EventEmitter<utils::enum_t> events;

        Style style;

        void focus();
        void unFocus();
        void trigger(utils::enum_t triggerType);
        void render(GraphicsContext* context, Rect rootRect, double dt);
        virtual Rect getRect(Rect rootRect);
        virtual bool contains(Rect rootRect, Vec2 pos);

        bool focused() const { return m_focused; }

    protected:

        OBJECT_NAME(Element)

        hg::Vec2 mousePos(hg::graphics::Window* window);
        bool mouseClicked(hg::graphics::Window* window);
        Rect adjustedRect(Rect rect, offset_t margin, offset_t padding);

        virtual void onRender(GraphicsContext* context, Rect rootRect, double dt) {}
        virtual bool onTrigger(utils::enum_t triggerType) { return false; }
    private:

        Element* element;

        bool m_focused = false;

        graphics::primitives::QuadBorder m_quadBorder;
        graphics::MeshInstance m_border;

    };

    HG_IS_CONCEPT(Element)

}

#endif //HAGAME2_ELEMENT_H
