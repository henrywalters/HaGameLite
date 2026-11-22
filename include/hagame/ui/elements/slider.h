//
// Created by henry on 11/14/26.
//

#ifndef HAGAME2_SLIDER_H
#define HAGAME2_SLIDER_H

#include <functional>
#include "../element.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"

namespace hg::ui {

    // A horizontal slider for a number between min and max. Drag it, or click anywhere along it, with the mouse (the
    // Frame needs mouseDown passed to update), or step it with the Left / Right triggers.
    //
    // Drawn as a track across the middle of its rect, the part up to the value filled in, and a square handle:
    //  track: style.backgroundColor (focusBackgroundColor while focused)
    //  fill and handle: style.foregroundColor (focusForegroundColor while focused)
    class Slider : public Element {
    public:

        // Called with the new value whenever it changes, by dragging, clicking or stepping
        std::function<void(float)> onChange;

        // Called once a drag (or click) lets go, with the final value. For work too slow to do on every change, e.g.
        // saving to disk
        std::function<void(float)> onRelease;

        float trackThickness = 4.0f; // Pixels
        float handleSize = 16.0f; // Pixels, square

        Slider(float value, float min = 0.0f, float max = 1.0f, float step = 0.0f);

        float value() const { return m_value; }

        // Clamped to min and max and snapped to step. Calls onChange if that changes the value
        void value(float value);

        float min() const { return m_min; }
        float max() const { return m_max; }

        // What the Left / Right triggers move by, and what the value snaps to. 0 is smooth, and steps by a twentieth
        float step() const { return m_step; }

        // Where the value sits between min and max, 0 to 1
        float fraction() const;

        bool dragging() const { return m_dragging; }

        bool onPointerDown(Rect rootRect, Vec2 pos) override;
        void onPointerDrag(Rect rootRect, Vec2 pos) override;
        void onPointerUp(Rect rootRect, Vec2 pos) override;

    protected:

        OBJECT_NAME(Slider)

        void onRender(GraphicsContext* context, Rect rootRect, double dt) override;
        bool onTrigger(utils::enum_t trigger) override;

    private:

        float m_value;
        float m_min;
        float m_max;
        float m_step;
        bool m_dragging = false;

        graphics::primitives::Quad m_trackQuad, m_fillQuad, m_handleQuad;
        graphics::MeshInstance m_track, m_fill, m_handle;

        // The part of the rect the handle's centre travels along, inset so the handle stays inside at either end
        float travelStart(Rect rect) const;
        float travelLength(Rect rect) const;

        void setFromPosition(Rect rootRect, Vec2 pos);
    };
}

#endif //HAGAME2_SLIDER_H
