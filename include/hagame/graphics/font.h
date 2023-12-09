//
// Created by henry on 12/30/22.
//

#ifndef HAGAME2_FONT_H
#define HAGAME2_FONT_H

#include <memory>

#include "../core/object.h"
#include "../utils/macros.h"
#include "../math/aliases.h"
#include "../utils/file.h"
#include "rawTexture.h"

struct stbtt_fontinfo;
struct stbtt_pack_context;

namespace hg::graphics {

    struct FontCharacter {
    public:
        unsigned int id;
        Vec2i size;
        Vec2i offset;
        Vec2i bearing;
        Vec2i ascent;
        int advance;
        Rect texCoords;

        ~FontCharacter();
    };


    using FontMap = std::unordered_map<char, std::unique_ptr<FontCharacter>>;

    using FontTexture = graphics::RawTexture<GL_R8, GL_UNSIGNED_BYTE, GL_RED>;

    class Font : public Object {
    public:

        Font(std::string ttf);

        FontCharacter* getChar(char character);

        float calcLineSpacing();
        float calcTabSpacing();
        Vec2i calcMessageSize(std::string message);

        HG_GET_SET(float, scale, updateFont);
        HG_GET_SET(float, lineSpacing, updateFont);
        HG_GET_SET(int, fontSize, updateFont);
        HG_GET_SET(int, ascent, updateFont);
        HG_GET_SET(int, descent, updateFont);
        HG_GET_SET(int, lineGap, updateFont);

        std::unique_ptr<FontTexture> m_atlas;

    protected:

        OBJECT_NAME(Font)

    private:

        float m_scale = 1.0f;
        float m_lineSpacing = 1.0f;
        int m_fontSize = 36;
        int m_ascent, m_descent, m_lineGap;
        FontMap m_fontMap;
        stbtt_fontinfo* m_font;
        stbtt_pack_context* m_packContext;
        void* m_bakedChar;
        void* m_packedChar;
        hg::utils::FileBuffer m_fileBuffer;


        void updateFont();

        void loadChar(char character, FontCharacter* fontChar);

    };
}

#endif //HAGAME2_FONT_H
