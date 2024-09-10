//
// Created by henry on 8/7/24.
//
#include "../../../../include/hagame/ui/mixins/textDisplay.h"

using namespace hg;
using namespace hg::ui::mixins;
using namespace hg::graphics;

TextDisplay::TextDisplay(Vec2 pos, graphics::Font *font, std::string text):
    m_text(font, text, pos.resize<3>(), Vec3::Zero(), TextHAlignment::Center, TextVAlignment::Center)
{}

void TextDisplay::setFont(Font *font) {
    m_text.font(font);
}

void TextDisplay::setText(std::string text) {
    m_text.text(text);
}

std::string TextDisplay::getText() {
    return m_text.text();
}

TextBuffer* TextDisplay::getBuffer() {
    return &m_text;
}

void TextDisplay::renderText(ui::GraphicsContext *context, graphics::Color color, Vec2 pos, Vec2 size, int z) {
    context->textBufferShader.use();
    context->textBufferShader.setVec4("textColor", color);
    m_text.limitSize(size.resize<3>());
    m_text.pos(Vec3(pos[0], pos[1], z));
    m_text.render();
}
