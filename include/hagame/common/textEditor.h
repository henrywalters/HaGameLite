//
// Created by henry on 9/28/26.
//

#ifndef HAGAME2_TEXTEDITOR_H
#define HAGAME2_TEXTEDITOR_H
#include "hagame/graphics/font.h"
#include "hagame/graphics/textBuffer.h"
#include "hagame/graphics/primitives/quad.h"

namespace hg {
    class TextEditor {
    public:

        struct Settings {
            std::string historyFile = ".history";
            std::string prefix = "> ";

            int lineHeight = 26.0f;

            float scrollWidth = 15;

            hg::graphics::Color bgColor = hg::graphics::Color(4, 30, 135, 150);
            hg::graphics::Color scrollBarColor = hg::graphics::Color(4, 30, 120, 150);
            hg::graphics::Color scrollColor = hg::graphics::Color(4, 30, 200, 150);
            hg::graphics::Color fgColor = hg::graphics::Color::white();
        } settings;

        bool open = false;

        TextEditor(hg::graphics::Font* font, Vec2 size, Vec2 pos);

        void render(Vec2 windowSize);

    private:


        Vec2 m_size;

        float m_scrollHeight;

        int m_scrollIdx = 0;

        std::string m_text;

        hg::graphics::primitives::Quad m_editorQuad;
        hg::graphics::primitives::Quad m_scrollQuad;
        hg::graphics::primitives::Quad m_scrollBarQuad;
        hg::graphics::primitives::Quad m_cursorQuad;
        hg::graphics::MeshInstance m_editor;
        hg::graphics::MeshInstance m_scroll;
        hg::graphics::MeshInstance m_scrollBar;
        hg::graphics::MeshInstance m_cursor;

        hg::graphics::TextBuffer m_buffer;

    };
}

#endif //HAGAME2_TEXTEDITOR_H
