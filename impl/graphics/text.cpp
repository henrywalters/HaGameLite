//
// Created by henry on 12/30/22.
//

#include "../../include/hagame/graphics/text.h"
#include "../../include/hagame/graphics/glfw.h"
#include "../../include/hagame/utils/string.h"

hg::graphics::Text::Text() {
    m_quad = std::make_unique<primitives::Quad>(Vec2::Zero());
    m_mesh = std::make_unique<MeshInstance>(m_quad.get());
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::string message, hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    auto messageSize = font->calcMessageSize(message);
    auto messageParts = utils::s_split(message, '\n');

    float x = pos[0];
    float y = pos[1] - (font->descent() * font->scale());

    if (messageParts.size() > 1) {
        y += font->calcLineSpacing();
    }

    for (const auto& line : messageParts) {
        auto lineSize = font->calcMessageSize(line);

        for (const auto& c : line) {
            auto character = font->getChar(c);

            if (c == '\t') {
                x += 4 * font->getChar(' ')->advance * font->scale();
                continue;
            }
            auto charSize = character->size.cast<float>();
            auto charPos = Vec2(
                    x + character->bearing[0] * font->scale(),
                    y - character->ascent[1]
                    );

            switch (alignmentH) {
                case TextHAlignment::Left:
                    break;
                case TextHAlignment::Center:
                    charPos[0] += (messageSize[0] - lineSize[0]) * 0.5f;
                    break;
                case TextHAlignment::Right:
                    charPos[1] += messageSize[0] - lineSize[0];
                    break;
            }


            m_quad->size(charSize);
            m_quad->offset(pos.resize<2>() + charPos + charSize * 0.5);
            m_mesh->update();

            glBindTexture(GL_TEXTURE_2D, character->id);
            m_mesh->render();

            x += character->advance * font->scale();
        }

        x = pos[0];
        y -= font->calcLineSpacing();
    }
}
