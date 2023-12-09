//
// Created by henry on 12/6/23.
//
#include "../../../../include/hagame/ui/elements/textButton.h"

using namespace hg::graphics;
using namespace hg::ui;

TextButton::TextButton(Vec2 pos, graphics::Font *font, std::string text):
    m_text(font, text, pos.resize<3>())
{}

void TextButton::render(double dt) {
    m_text.render();
}

void TextButton::setFont(Font *font) {
    m_text.font(font);
}

void TextButton::setText(std::string text) {
    m_text.text(text);
}

std::string TextButton::getText() {
    return m_text.text();
}

hg::Rect TextButton::getRect() {
    Vec2 size = m_text.font()->calcMessageSize(m_text.text()).cast<float>();
    return Rect(m_text.pos().resize<2>() - size * 0.5, size);
}
