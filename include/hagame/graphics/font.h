//
// Created by henry on 12/30/22.
//

#ifndef HAGAME2_FONT_H
#define HAGAME2_FONT_H

#include <memory>

#include "../core/object.h"
#include "../utils/macros.h"
#include "../math/aliases.h"

struct stbtt_fontinfo;

namespace hg::graphics {

    struct FontCharacter {
    public:
        unsigned int id;
        Vec2i size;
        Vec2i offset;
        Vec2i bearing;
        Vec2i ascent;
        int advance;

        ~FontCharacter();
    };


    using FontMap = std::unordered_map<char, std::unique_ptr<FontCharacter>>;

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

    protected:

        std::string toString() const override;

    private:

        float m_scale = 1.0f;
        float m_lineSpacing = 1.0f;
        int m_fontSize = 24;
        int m_ascent, m_descent, m_lineGap;
        FontMap m_fontMap;
        stbtt_fontinfo* m_font;

        void updateFont();

        void loadChar(char character, FontCharacter* fontChar);

    };
}

#endif //HAGAME2_FONT_H
