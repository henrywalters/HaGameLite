//
// Created by henry on 12/6/23.
//
#include "../../../../include/hagame/ui/elements/textButton.h"
#include "../../../../include/hagame/graphics/debug.h"

using namespace hg::graphics;
using namespace hg::ui;

TextButton::TextButton(Vec2 pos, graphics::Font *font, std::string text):
    mixins::TextDisplay(pos, font, text)
{}

void TextButton::render(GraphicsContext* context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    renderText(context, focused() ? style.focusForegroundColor : style.foregroundColor, rect.pos, rect.size, depth());
}
