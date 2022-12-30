//
// Created by henry on 12/30/22.
//

#define STB_TRUETYPE_IMPLEMENTATION

#include <iostream>
#include "stb_truetype.h"

#include "../../include/hagame/graphics/glfw.h"

#include "../../include/hagame/graphics/font.h"

hg::graphics::FontCharacter::~FontCharacter() {
    glDeleteTextures(1, &id);
}

hg::graphics::Font::Font(std::string ttf) {
    m_font = new stbtt_fontinfo();
    if (!stbtt_InitFont(m_font, (unsigned char*) ttf.c_str(), 0)) {
        std::cout << "FAILED TO INITIALIZE FONT\n";
        throw std::runtime_error("Failed to load Font");
    }
    updateFont();
}

hg::graphics::FontCharacter *hg::graphics::Font::getChar(char character) {
    return m_fontMap[character].get();
}

void hg::graphics::Font::updateFont() {
    m_scale = stbtt_ScaleForPixelHeight(m_font, m_fontSize);
    stbtt_GetFontVMetrics(m_font, &m_ascent, &m_descent, &m_lineGap);

    m_fontMap.clear();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int i = 0; i < 128; i++) {
        m_fontMap.insert(std::make_pair((char) i, std::make_unique<FontCharacter>()));
        loadChar((char) i, m_fontMap[(char)i].get());
    }
}

void hg::graphics::Font::loadChar(char character, FontCharacter* fontChar) {
    Vec2i a, b;
    auto buffer = stbtt_GetCodepointBitmap(m_font, 0, m_scale, character, &fontChar->size[0], &fontChar->size[1], &fontChar->offset[0], &fontChar->offset[1]);
    stbtt_GetCodepointHMetrics(m_font, character, &fontChar->advance, &fontChar->bearing[0]);
    stbtt_GetCodepointBitmapBox(m_font, character, 0, m_scale, 0, &fontChar->ascent[0], 0, &fontChar->ascent[1]);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &fontChar->id);
    glBindTexture(GL_TEXTURE_2D, fontChar->id);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            fontChar->size[0],
            fontChar->size[1],
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            buffer
            );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

float hg::graphics::Font::calcLineSpacing() {
    return (m_ascent - m_descent + m_lineGap) * m_lineSpacing * m_scale;
}

float hg::graphics::Font::calcTabSpacing() {
    return 4 * getChar(' ')->advance * m_scale;
}

hg::Vec2i hg::graphics::Font::calcMessageSize(std::string message) {
    Vec2i size;
    float x = 0;
    float y = calcLineSpacing();

    for (const auto& c: message) {
        if (c == '\n') {
            size[0] = x;
            x = 0;
            y += calcLineSpacing();
            continue;
        }

        if (c == '\t') {
            x += calcTabSpacing();
        }

        auto ch = getChar(c);

        x += ch->advance * m_scale;
    }

    if (x != 0) {
        size[0] = x;
    }

    size[1] = y;

    return size;
}

std::string hg::graphics::Font::toString() const {
    return "Font<" + std::to_string(id()) + "> = \n" +
        "\tSize = " + std::to_string(m_fontSize) + "\n" +
        "\tScale = " + std::to_string(m_scale) + "\n" +
        "\tAscent = " + std::to_string(m_ascent) + "\n" +
        "\tDescent = " + std::to_string(m_descent) + "\n" +
        "\tLine gap = " + std::to_string(m_lineGap) + "\n";
}
