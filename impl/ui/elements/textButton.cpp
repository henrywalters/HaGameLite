//
// Created by henry on 12/6/23.
//
#include "../../../../include/hagame/ui/elements/textButton.h"
#include "../../../../include/hagame/graphics/debug.h"

using namespace hg::graphics;
using namespace hg::ui;

TextButton::TextButton(Vec2 pos, graphics::Font *font, std::string text):
    m_text(font, text, pos.resize<3>(), Vec3::Zero(), TextHAlignment::Center, TextVAlignment::Center)
{}

void TextButton::setFont(Font *font) {
    m_text.font(font);
}

void TextButton::setText(std::string text) {
    m_text.text(text);
}

std::string TextButton::getText() {
    return m_text.text();
}

void TextButton::render(graphics::BatchRenderer* renderer, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    m_text.limitSize(rect.size.resize<3>());
    m_text.pos(Vec3(rect.pos[0], rect.pos[1], depth()));
    //m_text.size(rect.size.resize<3>());
    m_text.render();
    // Debug::DrawRect(rect, Color::green(), 2);
}
