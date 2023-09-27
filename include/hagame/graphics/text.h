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

    class Text {
    public:

        Text();

        void draw(Font* font, std::string message, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
        void draw(Font* font, std::string message, Vec3 size, Vec3 pos, TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);
        void draw(Font* font, std::vector<std::string> lines, Vec3 pos = Vec3::Zero(), TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);

    private:
        std::unique_ptr<primitives::Quad> m_quad;
        std::unique_ptr<MeshInstance> m_mesh;
        std::unique_ptr<VertexBuffer<Vertex>> m_buffer;
    };
}

#endif //HAGAME2_TEXT_H
