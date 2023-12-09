//
// Created by henry on 12/30/22.
//



#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include "stb_rect_pack.h"
#include "stb_truetype.h"

#include "../../include/hagame/graphics/font.h"

#include <iostream>

#include "../../include/hagame/graphics/glfw.h"



hg::graphics::FontCharacter::~FontCharacter() {
    glDeleteTextures(1, &id);
}

hg::graphics::Font::Font(std::string ttf):
    m_font(new stbtt_fontinfo()),
    m_packContext(new stbtt_pack_context()),
    m_packedChar(malloc(sizeof(stbtt_packedchar) * 96)),
    m_bakedChar(malloc(sizeof(stbtt_bakedchar) * 96))
{

    m_fileBuffer = utils::f_readToBuffer(ttf);
    if (!stbtt_InitFont(m_font, m_fileBuffer.data, 0)) {
        throw std::runtime_error("Failed to load Font");
    }

    unsigned char pixels[512*512];
    void* context;

    //stbtt_BakeFontBitmap(m_fileBuffer.data, 0, fontSize(), pixels, 512, 512, 32, 96, (stbtt_bakedchar *)m_bakedChar);
    //m_atlas = std::make_unique<FontTexture>(hg::Vec2i(512, 512), pixels);

    if (!stbtt_PackBegin(m_packContext, pixels, 512, 512, 0, 1, context)) {
        throw std::runtime_error("Failed to initalize packing context");
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_atlas = std::make_unique<FontTexture>(hg::Vec2i(512, 512), pixels);


    updateFont();
}

hg::graphics::FontCharacter *hg::graphics::Font::getChar(char character) {
    return m_fontMap[character].get();
}

void hg::graphics::Font::updateFont() {
    m_scale = stbtt_ScaleForPixelHeight(m_font, m_fontSize);
    stbtt_GetFontVMetrics(m_font, &m_ascent, &m_descent, &m_lineGap);

    //stbtt_PackSetOversampling(m_packContext, 16, 16);
    stbtt_PackFontRange(m_packContext, m_fileBuffer.data, 0, fontSize(), 32, 96, (stbtt_packedchar*) m_packedChar);

    m_atlas->update(hg::Vec2i(512, 512), m_packContext->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    m_fontMap.clear();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int i = 0; i < 128; i++)
    {
        m_fontMap.insert(std::make_pair((char) i, std::make_unique<FontCharacter>()));
        loadChar((char) i, m_fontMap[(char)i].get());
    }
}

void hg::graphics::Font::loadChar(char character, FontCharacter* fontChar) {
    Vec2i a, b;
    auto buffer = stbtt_GetCodepointBitmap(m_font, 0, m_scale, character, &fontChar->size[0], &fontChar->size[1], &fontChar->offset[0], &fontChar->offset[1]);
    stbtt_GetCodepointHMetrics(m_font, character, &fontChar->advance, &fontChar->bearing[0]);
    stbtt_GetCodepointBitmapBox(m_font, character, 0, m_scale, 0, &fontChar->ascent[0], 0, &fontChar->ascent[1]);

    float posX, posY;

    stbtt_aligned_quad q;

    //stbtt_GetBakedQuad((stbtt_bakedchar*)m_bakedChar, 512, 512, character -  32, )

    stbtt_GetPackedQuad((stbtt_packedchar *)m_packedChar, 512, 512, character - 32, &posX, &posY, &q, 0);

    fontChar->texCoords = Rect(Vec2(q.s0, q.t0), Vec2(q.s1 - q.s0, q.t1 - q.t0));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &fontChar->id);
    glBindTexture(GL_TEXTURE_2D, fontChar->id);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_R8,
            fontChar->size[0],
            fontChar->size[1],
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            buffer
            );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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