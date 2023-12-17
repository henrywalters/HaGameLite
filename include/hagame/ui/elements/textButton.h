//
// Created by henry on 12/6/23.
//

#ifndef HAGAME2_TEXTBUTTON_H
#define HAGAME2_TEXTBUTTON_H

#include "button.h"
#include "../../graphics/textBuffer.h"

namespace hg::ui {
    class TextButton : public Button {
    public:

        TextButton(Vec2 pos, graphics::Font* font, std::string text);
        void render(double dt) override;

        void setFont(graphics::Font* font);
        void setText(std::string text);
        std::string getText();
        void setPos(Vec2 pos);
        Rect getRect() override;

    private:
        
        graphics::TextBuffer m_text;

    };
}

#endif //HAGAME2_TEXTBUTTON_H
