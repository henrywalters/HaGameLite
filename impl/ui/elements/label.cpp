//
// Created by henry on 8/7/24.
//
#include "../../../../include/hagame/ui/elements/label.h"

using namespace hg;
using namespace hg::ui;
using namespace hg::graphics;

Label::Label(Vec2 pos, graphics::Font *font, std::string text):
        mixins::TextDisplay(pos, font, text)
{}

void Label::render(GraphicsContext* context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    renderText(context, focused() ? style.focusForegroundColor : style.foregroundColor, rect.pos, rect.size, depth());
}
