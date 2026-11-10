//
// Created by henry on 9/28/26.
//
#include <hagame/common/textEditor.h>

#include "hagame/core/assets.h"

using namespace hg;
using namespace hg::graphics;

hg::TextEditor::TextEditor(Font *font, Vec2 size, Vec2 pos):
    m_editorQuad(size, pos),
    m_editor(&m_editorQuad),
    m_cursorQuad(Vec2(10, 24)),
    m_cursor(&m_cursorQuad),
    m_scrollBarQuad(Vec2(settings.scrollWidth, size[1]), Vec2(size[0] - settings.scrollWidth, -size[1])),
    m_scrollBar(&m_scrollBarQuad),
    m_scrollQuad(Vec2(settings.scrollWidth, 20), Vec2(size[0] - settings.scrollWidth, -size[1])),
    m_scroll(&m_scrollQuad)
{
    m_editorQuad.centered(false);
    m_cursorQuad.centered(false);
    m_scrollQuad.centered(false);

    m_editor.update(&m_editorQuad);
    m_cursor.update(&m_cursorQuad);
    m_scroll.update(&m_scrollQuad);
}

void TextEditor::render(Vec2 windowSize) {

    if (!open) return;

    Mat4 proj = Mat4::Orthographic(0, windowSize[0], 0, windowSize[1], -100, 100);

    auto shader = hg::getShader("color");
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", proj);
    shader->setMat4("model", Mat4::Identity());
    shader->setVec4("color", settings.bgColor);

    m_editor.render();

    // TODO: Get cursor working
    shader->setVec4("color", settings.fgColor);
    // m_cursor->render();

    shader->setVec4("color", settings.scrollBarColor);
    m_scrollBar.render();
    shader->setVec4("color", settings.scrollColor);
    m_scroll.render();

    shader = hg::getShader("text_buffer");
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", proj);
    shader->setMat4("model", Mat4::Identity());
    shader->setVec4("textColor", settings.fgColor);

    m_buffer.render();

}