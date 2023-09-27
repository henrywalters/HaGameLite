//
// Created by henry on 12/30/22.
//

#include "../../include/hagame/graphics/text.h"
#include "../../include/hagame/graphics/glfw.h"
#include "../../include/hagame/utils/string.h"

hg::graphics::Text::Text() {
    m_quad = std::make_unique<primitives::Quad>(Vec2::Zero());
    m_mesh = std::make_unique<MeshInstance>(m_quad.get());
/*
    auto vao = m_mesh->getVAO();
    m_buffer = VertexBuffer<Vertex>::Dynamic(0);
    m_buffer->bind();
    vao->bind();
    vao->defineAttribute(m_buffer.get(), DataType::Float, 0, 3, offsetof(Vertex, position));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 1, 3, offsetof(Vertex, normal));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 2, 2, offsetof(Vertex, texCoords));
    vao->setInstanced(0, 2);
    */
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::string message, hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    draw(font, utils::s_split(message, '\n'), pos, alignmentH, alignmentV);
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::string message, hg::Vec3 size, hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    auto originalLines = utils::s_split(message, '\n');
    std::vector<std::string> lines;

    for (const auto& originalLine : originalLines) {
        std::vector<std::string> subLines;
        auto words = utils::s_split(originalLine, ' ');
        int x = 0;
        std::string line;
        for (int i = 0; i < words.size(); i++) {
            auto word = words[i];
            auto wordSize = font->calcMessageSize(word);
            bool newLine = x + wordSize[0] > size[0];

            if (newLine) {
                subLines.push_back(line);
                line = "";
                x = 0;
            }

            if (i >= 1 && !newLine) {
                line += " ";
                x += font->calcMessageSize(" ")[0];
            }

            line += word;

            x += wordSize[0];
        }
        subLines.push_back(line);

        lines.insert(lines.end(), subLines.begin(), subLines.end());
    }

    draw(font, originalLines, pos, alignmentH, alignmentV);
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::vector<std::string> lines,
                              hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {

    float lineWidth = 0;

    int charLength = 0;

    for (const auto& line: lines) {
        auto lineSize = font->calcMessageSize(line);
        if (lineSize.x() > lineWidth) {
            lineWidth = lineSize.x();
        }
        charLength += line.length();
    }

   // m_buffer->resize(charLength);
  //  m_buffer->clear();

    float x = pos[0];
    float y = pos[1] - (font->descent() * font->scale());

    if (lines.size() > 1) {
        y += font->calcLineSpacing();
    }

    int index = 0;

    for (const auto& line : lines) {
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
                    charPos[0] += (lineWidth - lineSize[0]) * 0.5f;
                    break;
                case TextHAlignment::Right:
                    charPos[1] += lineWidth - lineSize[0];
                    break;
            }

            m_quad->size(charSize);

            m_quad->texOffset(character->texCoords.pos);
            m_quad->texSize(character->texCoords.size);
            m_quad->offset(pos.resize<2>() + charPos + charSize * 0.5);
            m_mesh->update(m_quad.get());

            font->m_atlas->bind();
            m_mesh->render();

            x += character->advance * font->scale();
        }

        x = pos[0];
        y -= font->calcLineSpacing();
    }
}
