//
// Created by henry on 12/30/22.
//

#ifndef HAGAME2_TEXT_H
#define HAGAME2_TEXT_H

#include "primitives/quad.h"
#include "font.h"

namespace hg::graphics {
    enum class TextHAlignment {
        Left,
        Center,
        Right
    };

    enum class TextVAlignment {
        Top,
        Center,
        Bottom
    };

    class TextBuffer;

    // Text is a good way to render short text bits, especially ones that will change frequently.
    // For larger texts, or static ones, use TextBuffer
    class Text {
    public:

        struct Character {
            char ch;
            Vec2 size;
            Rect texCoords;
            Vec2 position;
        };

        Text();

        void draw(Font* font, std::string message, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
        void draw(Font* font, std::string message, Vec3 size, Vec3 pos, TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
        void draw(Font* font, std::vector<std::string> lines, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);

    private:

        friend class TextBuffer;

        std::unique_ptr<primitives::Quad> m_quad;
        std::unique_ptr<MeshInstance> m_mesh;

        void render(Font* font, std::vector<Character>& characters);

        static std::vector<Character> Construct(Font* font, std::string message, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
        static std::vector<Character> Construct(Font* font, std::string message, Vec3 size, Vec3 pos, TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
        static std::vector<Character> Construct(Font* font, std::vector<std::string> lines, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
    };
}

#endif //HAGAME2_TEXT_H
