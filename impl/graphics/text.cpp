//
// Created by henry on 12/30/22.
//

#include "../../include/hagame/graphics/text.h"
#include "../../include/hagame/graphics/glfw.h"
#include "../../include/hagame/utils/string.h"
#include "../../include/hagame/graphics/debug.h"

using namespace hg::graphics;

hg::graphics::Text::Text() {
    m_quad = std::make_unique<primitives::Quad>(Vec2::Zero());
    m_mesh = std::make_unique<MeshInstance>(m_quad.get());
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::string message, hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {

    auto characters = Construct(font, message, pos, alignmentH, alignmentV);
    render(font, characters);
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::string message, hg::Vec3 size, hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    auto characters = Construct(font, message, pos, size, alignmentH, alignmentV);
    render(font, characters);
}

void hg::graphics::Text::draw(hg::graphics::Font *font, std::vector<std::string> lines,
                              hg::Vec3 pos,
                              hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    auto characters = Construct(font, lines, pos, Vec3::Zero(), alignmentH, alignmentV);
    render(font, characters);
}

std::vector<Text::Character> hg::graphics::Text::Construct(hg::graphics::Font *font, std::vector<std::string> lines, hg::Vec3 pos,
                                   hg::Vec3 size,
                                   hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    std::vector<Text::Character> characters;

    Vec2 messageSize;

    for (const auto& line: lines) {
        auto lineSize = font->calcMessageSize(line);
        if (lineSize.x() > messageSize.x()) {
            messageSize[0] = lineSize[0];
        }
        messageSize[1] += lineSize[1];
    }

    hg::Vec2 rectOffset;

    if (messageSize[0] < size[0]) {
        switch (alignmentH) {
            case TextHAlignment::Left:
                break;
            case TextHAlignment::Center:
                rectOffset[0] = (size[0] - messageSize[0]) * 0.5;
                break;
            case TextHAlignment::Right:
                rectOffset[0] = size[0] - messageSize[0];
                break;
        }
    }

    if (messageSize[1] < size[1]) {
        switch (alignmentV) {
            case TextVAlignment::Top:
                rectOffset[1] = size[1] - messageSize[1];
                break;
            case TextVAlignment::Center:
                rectOffset[1] = (size[1] - messageSize[1]) * 0.5;
                break;
            case TextVAlignment::Bottom:
                break;
        }
    }

    hg::Vec2 cursorPos;

    cursorPos[0] = pos[0];
    cursorPos[1] = pos[1] - (font->descent() * font->scale());

    cursorPos[1] += font->calcLineSpacing() * (lines.size() - 1);

    // Centres the font's ascent-to-descent box in each line, the same for any text
    float baselineOffset = (font->calcLineSpacing() - (font->ascent() - font->descent()) * font->scale()) * 0.5f;

    int lineIndex = 0;
    for (const auto& line : lines) {

        auto lineSize = font->calcMessageSize(line);

        std::unordered_map<char, FontCharacter*> chars;
        for (const auto& c : line) {
            chars.insert(std::make_pair(c, font->getChar(c)));
        }

        for (const auto& c : line) {
            if (c == '\t') {
                cursorPos[0] += 4 * font->getChar(' ')->advance * font->scale();
                continue;
            }
            auto charSize = chars[c]->size.cast<float>();

            auto charPos = Vec2(
                    cursorPos[0] + chars[c]->bearing[0] * font->scale() + rectOffset.x(),
                    cursorPos[1] - chars[c]->ascent[1] + rectOffset.y()
            );

            switch (alignmentH) {
                case TextHAlignment::Left:
                    break;
                case TextHAlignment::Center:
                    charPos[0] += (messageSize[0] - lineSize[0]) * 0.5f;
                    break;
                case TextHAlignment::Right:
                    charPos[0] += messageSize[0] - lineSize[0];
                    break;
            }

            // Every line shares one baseline from the font's metrics. It used to be nudged by the tallest glyph in the
            // line, so a 'p' or 'g' moved the whole line
            charPos[1] += baselineOffset;

            Character newChar;
            newChar.ch = c;
            newChar.size = charSize;
            newChar.texCoords = chars[c]->texCoords;
            newChar.position = charPos + charSize * 0.5;
            newChar.line = lineIndex;

            characters.push_back(newChar);

            cursorPos[0] += chars[c]->advance * font->scale();

        }

        cursorPos[0] = pos[0];
        cursorPos[1] -= font->calcLineSpacing();
        lineIndex++;
    }

    return characters;
}

std::vector<Text::Character>
hg::graphics::Text::Construct(hg::graphics::Font *font, std::string message, hg::Vec3 size, hg::Vec3 pos,
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

    return Construct(font, lines, pos, size, alignmentH, alignmentV);
}

std::vector<Text::Character> hg::graphics::Text::Construct(hg::graphics::Font *font, std::string message, hg::Vec3 pos,
                                   hg::graphics::TextHAlignment alignmentH, hg::graphics::TextVAlignment alignmentV) {
    return Construct(font, hg::utils::s_split(message, '\n'), pos, Vec3::Zero(), alignmentH, alignmentV);
}

void Text::render(Font *font, std::vector<Text::Character>& characters) {
    font->m_atlas->bind();

    for (const auto& ch : characters) {
        m_quad->size(ch.size);
        m_quad->texOffset(ch.texCoords.pos);
        m_quad->texSize(ch.texCoords.size);
        m_quad->offset(ch.position);
        m_mesh->update(m_quad.get());

        m_mesh->render();
    }
}
