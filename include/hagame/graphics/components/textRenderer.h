//
// Created by henry on 3/27/23.
//

#ifndef HAGAME2_TEXTRENDERER_H
#define HAGAME2_TEXTRENDERER_H

#include <algorithm>
#include <limits>
#include "../../core/component.h"
#include "../../core/assets.h"
#include "../../math/aliases.h"
#include "../../utils/file.h"
#include "../text.h"
#include "../font.h"
#include "../fontAsset.h"
#include "../color.h"
#include "../textBuffer.h"

namespace hg::graphics {
    // Text drawn at its entity. Set its fields, as the editor does, or construct it with a font and set the buffer
    // up directly. With the font field set, the renderer owns the buffer's layout: changing a field rebuilds it the next
    // time the buffer is used. Without it, a buffer set up directly is left alone
    class TextRenderer : public Component {
    public:

        // Where the text sits around the entity, and how its lines line up with each other
        static constexpr const char* LEFT = "Left";
        static constexpr const char* CENTER = "Center";
        static constexpr const char* RIGHT = "Right";
        static constexpr const char* TOP = "Top";
        static constexpr const char* BOTTOM = "Bottom";
        static std::vector<std::string> HAlignments() { return {LEFT, CENTER, RIGHT}; }
        static std::vector<std::string> VAlignments() { return {TOP, CENTER, BOTTOM}; }

        TextRenderer():
            m_buffer(std::make_unique<TextBuffer>(nullptr))
        {}

        TextRenderer(Font* font):
            m_buffer(std::make_unique<TextBuffer>(font))
        {}

        FontAsset font;
        std::string text;
        Color color = Color::black();
        int fontSize = 0; // Pixels tall. 0 uses the font at the size it was loaded
        std::string hAlign = CENTER; // Left puts the text's left edge on the entity, Center its middle, Right its right
        std::string vAlign = CENTER; // The same up and down: Top hangs the text below the entity
        float wrapWidth = 0.0f; // Font pixels to wrap lines at, between words. 0 doesn't wrap

        // The buffer, brought up to date with the fields
        TextBuffer* buffer() {
            sync();
            return m_buffer.get();
        }

    protected:

        OBJECT_NAME(TextRenderer)

    private:

        std::unique_ptr<TextBuffer> m_buffer;

        // What the buffer was last laid out with, so it's only rebuilt when a field changes
        std::string m_syncedFont;
        std::string m_syncedText;
        int m_syncedSize = -1;
        std::string m_syncedHAlign;
        std::string m_syncedVAlign;
        float m_syncedWrap = -1.0f;

        static TextHAlignment parseH(std::string const& value) {
            return value == LEFT ? TextHAlignment::Left : value == RIGHT ? TextHAlignment::Right : TextHAlignment::Center;
        }

        static TextVAlignment parseV(std::string const& value) {
            return value == TOP ? TextVAlignment::Top : value == BOTTOM ? TextVAlignment::Bottom : TextVAlignment::Center;
        }

        // The font at fontSize. Sizes other than the loaded one are their own copy of the font, loaded once and shared
        // by every renderer asking for that font at that size
        Font* sizedFont() {
            if (fontSize <= 0 || !font.asset() || font.asset()->fontSize() == fontSize) {
                return font.asset();
            }
            std::string key = font.path() + "@" + std::to_string(fontSize);
            if (!hasFont(key)) {
                // Fonts are loaded from the fonts folder, by path (fonts/vt323) or, for the loading screen's, by name
                std::string relative = font.path().find('/') == std::string::npos ? "fonts/" + font.path() : font.path();
                std::string file = ASSET_DIR + relative + ".ttf";
                if (!utils::f_exists(file)) {
                    return font.asset();
                }
                loadFont(key, file)->fontSize(fontSize);
            }
            return getFont(key);
        }

        void sync() {
            // A font set before fonts were loaded (e.g. from a prefab) didn't find it, so look again
            if (!font.asset() && !font.path().empty()) {
                FontAsset retry;
                retry.path(font.path());
                if (retry.asset()) {
                    font = retry;
                }
            }
            if (!font.asset()) {
                if (text != m_syncedText) {
                    m_syncedText = text;
                    m_buffer->text(text);
                }
                return;
            }

            bool changed = font.path() != m_syncedFont || text != m_syncedText || fontSize != m_syncedSize ||
                           hAlign != m_syncedHAlign || vAlign != m_syncedVAlign || wrapWidth != m_syncedWrap;
            if (!changed) {
                return;
            }
            m_syncedFont = font.path();
            m_syncedText = text;
            m_syncedSize = fontSize;
            m_syncedHAlign = hAlign;
            m_syncedVAlign = vAlign;
            m_syncedWrap = wrapWidth;

            Font* sized = sizedFont();
            auto h = parseH(hAlign);
            auto v = parseV(vAlign);
            Vec3 box = wrapWidth > 0.0f ? Vec3(wrapWidth, 0.0f, 0.0f) : Vec3::Zero();

            // Lay it out once at the origin to measure it, then move it so the alignment decides where it sits around
            // the entity. TextBuffer's alignment only lines lines up with each other, inside a box
            auto characters = wrapWidth > 0.0f ?
                Text::Construct(sized, text, box, Vec3::Zero(), h, v) :
                Text::Construct(sized, text, Vec3::Zero(), h, v);
            Vec2 min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::lowest());
            for (const auto& ch : characters) {
                for (int i = 0; i < 2; i++) {
                    min[i] = std::min(min[i], ch.position[i] - ch.size[i] * 0.5f);
                    max[i] = std::max(max[i], ch.position[i] + ch.size[i] * 0.5f);
                }
            }
            Vec2 pivot(h == TextHAlignment::Left ? 0.0f : h == TextHAlignment::Right ? 1.0f : 0.5f,
                       v == TextVAlignment::Bottom ? 0.0f : v == TextVAlignment::Top ? 1.0f : 0.5f);
            Vec2 offset = characters.empty() ? Vec2::Zero() : (min + (max - min).prod(pivot)) * -1.0f;

            m_buffer->font(sized);
            m_buffer->hAlignment(h);
            m_buffer->vAlignment(v);
            if (wrapWidth > 0.0f) {
                m_buffer->limitSize(box);
            } else {
                m_buffer->uncapSize();
            }
            m_buffer->pos(Vec3(std::round(offset[0]), std::round(offset[1]), 0.0f));
            m_buffer->text(text);
        }
    };

    HG_COMPONENT(Graphics, TextRenderer)
    HG_FIELD(TextRenderer, hg::graphics::FontAsset, font)
    HG_FIELD(TextRenderer, std::string, text)
    HG_FIELD(TextRenderer, hg::graphics::Color, color)
    HG_NUMBER_FIELD(TextRenderer, int, fontSize, 0, 256, 1)
    HG_FIELD(TextRenderer, std::string, hAlign)
    HG_FIELD(TextRenderer, std::string, vAlign)
    HG_NUMBER_FIELD(TextRenderer, float, wrapWidth, 0, 4096, 1)
}

#endif //HAGAME2_TEXTRENDERER_H
