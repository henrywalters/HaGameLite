//
// Created by henry on 9/25/23.
//
#include "../../include/hagame/graphics/textBuffer.h"

hg::graphics::TextBuffer::TextBuffer()
{

}

hg::graphics::TextBuffer::TextBuffer(hg::graphics::Font *font, std::string text, hg::Vec3 pos, TextHAlignment hAlignment, TextVAlignment vAlignment):
    m_font(font),
    m_text(text),
    m_pos(pos),
    m_limitSize(false),
    m_hAlignment(hAlignment),
    m_vAlignment(vAlignment)
{
    updateBuffers();
}

hg::graphics::TextBuffer::TextBuffer(hg::graphics::Font *font, std::string text, hg::Vec3 pos, hg::Vec3 size, TextHAlignment hAlignment, TextVAlignment vAlignment):
    m_font(font),
    m_text(text),
    m_pos(pos),
    m_limitSize(true),
    m_size(size),
    m_hAlignment(hAlignment),
    m_vAlignment(vAlignment)
{
    updateBuffers();
}

void hg::graphics::TextBuffer::updateBuffers() {

    if (!m_font) {
        return;
    }

    std::vector<Text::Character> characters = m_limitSize ?
        Text::Construct(m_font, m_text, m_size, m_pos, m_hAlignment, m_vAlignment) :
        Text::Construct(m_font, m_text, m_pos, m_hAlignment, m_vAlignment);

    std::unordered_map<char, std::vector<Vec3>> charData;

    m_characters.clear();

    for (const auto& ch : characters) {
        if (charData.find(ch.ch) == charData.end()) {
            charData.insert(std::make_pair(ch.ch, std::vector<Vec3>()));
        }
        charData[ch.ch].push_back(ch.position.resize<3>());
    }

    for (const auto& [ch, data] : charData) {

        auto character = m_font->getChar(ch);
        primitives::Quad quad(character->size.cast<float>());
        quad.texOffset(character->texCoords.pos);
        quad.texSize(character->texCoords.size);
        m_characters.insert(std::make_pair(ch, std::make_shared<Character>(quad, data)));

        auto vao = m_characters[ch]->mesh.getVAO();
        m_characters[ch]->buffer->bind();
        vao->bind();
        vao->defineAttribute(m_characters[ch]->buffer.get(), DataType::Float, 3, 3);
        vao->setInstanced(3);
    }
}

void hg::graphics::TextBuffer::render() {
    m_font->m_atlas->bind();
    for (const auto& [ch, character] : m_characters) {
        character->buffer->bind();
        character->mesh.getVAO()->bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, character->buffer->max);
    }
}

