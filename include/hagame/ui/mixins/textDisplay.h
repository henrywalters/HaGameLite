//
// Created by henry on 8/7/24.
//

#ifndef HAGAME2_TEXTDISPLAY_H
#define HAGAME2_TEXTDISPLAY_H

#include "../../graphics/textBuffer.h"
#include "../graphicsContext.h"

namespace hg::ui::mixins {
    class TextDisplay {
    public:

        TextDisplay(Vec2 pos, graphics::Font* font, std::string text);

        void setFont(graphics::Font* font);
        void setText(std::string text);
        std::string getText();

        void renderText(GraphicsContext* context, graphics::Color color, Vec2 pos, Vec2 size, int z);

        graphics::TextBuffer* getBuffer();

    protected:

        graphics::TextBuffer m_text;

    };
}

#endif //HAGAME2_TEXTDISPLAY_H
