//
// Created by henry on 11/14/26.
//
#include "../../../../include/hagame/ui/elements/slider.h"

#include <algorithm>
#include <cmath>

using namespace hg;
using namespace hg::ui;

Slider::Slider(float value, float min, float max, float step):
    m_value(value),
    m_min(min),
    m_max(max > min ? max : min + 1.0f),
    m_step(std::max(step, 0.0f)),
    m_trackQuad(Vec2::Zero(), Vec2::Zero()),
    m_fillQuad(Vec2::Zero(), Vec2::Zero()),
    m_handleQuad(Vec2::Zero(), Vec2::Zero()),
    m_track(&m_trackQuad),
    m_fill(&m_fillQuad),
    m_handle(&m_handleQuad)
{
    m_trackQuad.centered(true);
    m_fillQuad.centered(true);
    m_handleQuad.centered(true);
    // Through the setter, so a starting value outside the range is clamped too, without calling onChange (unset)
    this->value(value);
}

void Slider::value(float value) {
    value = std::clamp(value, m_min, m_max);
    if (m_step > 0.0f) {
        value = m_min + std::round((value - m_min) / m_step) * m_step;
        value = std::clamp(value, m_min, m_max);
    }
    if (value == m_value) {
        return;
    }
    m_value = value;
    if (onChange) {
        onChange(m_value);
    }
}

float Slider::fraction() const {
    return (m_value - m_min) / (m_max - m_min);
}

float Slider::travelStart(Rect rect) const {
    return rect.pos[0] + handleSize * 0.5f;
}

float Slider::travelLength(Rect rect) const {
    return std::max(rect.size[0] - handleSize, 1.0f);
}

void Slider::setFromPosition(Rect rootRect, Vec2 pos) {
    auto rect = getRect(rootRect);
    float t = std::clamp((pos[0] - travelStart(rect)) / travelLength(rect), 0.0f, 1.0f);
    value(m_min + t * (m_max - m_min));
}

bool Slider::onPointerDown(Rect rootRect, Vec2 pos) {
    m_dragging = true;
    focus();
    // Clicking along the track jumps there, and a drag carries on from it
    setFromPosition(rootRect, pos);
    return true;
}

void Slider::onPointerDrag(Rect rootRect, Vec2 pos) {
    setFromPosition(rootRect, pos);
}

void Slider::onPointerUp(Rect rootRect, Vec2 pos) {
    m_dragging = false;
    if (onRelease) {
        onRelease(m_value);
    }
}

bool Slider::onTrigger(utils::enum_t trigger) {
    float step = m_step > 0.0f ? m_step : (m_max - m_min) / 20.0f;
    if (trigger == UITriggers::Left) {
        value(m_value - step);
    } else if (trigger == UITriggers::Right) {
        value(m_value + step);
    } else if (trigger == UITriggers::Select) {
        // Nothing to select, but taken, so it doesn't reach a parent button
    } else {
        return false;
    }
    if (trigger != UITriggers::Select && onRelease) {
        onRelease(m_value);
    }
    return true;
}

void Slider::onRender(GraphicsContext* context, Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    float centerY = rect.pos[1] + rect.size[1] * 0.5f;
    float start = travelStart(rect);
    float length = travelLength(rect);
    float handleX = start + fraction() * length;

    // Whole pixels, so a pixel-art menu stays crisp
    auto snap = [](float v) { return std::round(v); };

    m_trackQuad.setSizeAndOffset(Vec2(snap(length), snap(trackThickness)), Vec2(snap(start + length * 0.5f), snap(centerY)));
    m_track.update(&m_trackQuad);

    float fillLength = handleX - start;
    m_fillQuad.setSizeAndOffset(Vec2(snap(fillLength), snap(trackThickness)), Vec2(snap(start + fillLength * 0.5f), snap(centerY)));
    m_fill.update(&m_fillQuad);

    m_handleQuad.setSizeAndOffset(Vec2(snap(handleSize), snap(handleSize)), Vec2(snap(handleX), snap(centerY)));
    m_handle.update(&m_handleQuad);

    bool lit = focused() || m_dragging;
    auto background = lit ? style.focusBackgroundColor : style.backgroundColor;
    auto foreground = lit ? style.focusForegroundColor : style.foregroundColor;

    context->colorShader.use();
    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth())));
    context->colorShader.setVec4("color", background);
    m_track.render();

    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth() + 1)));
    context->colorShader.setVec4("color", foreground);
    if (fillLength >= 1.0f) {
        m_fill.render();
    }

    context->colorShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth() + 2)));
    m_handle.render();
}
