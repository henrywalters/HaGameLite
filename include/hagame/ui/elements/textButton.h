//
// Created by henry on 12/6/23.
//

#ifndef HAGAME2_TEXTBUTTON_H
#define HAGAME2_TEXTBUTTON_H

#include "button.h"
#include "../mixins/textDisplay.h"
#include "../../graphics/textBuffer.h"

namespace hg::ui {
    class TextButton : public Button, public mixins::TextDisplay {
    public:

        TextButton(Vec2 pos, graphics::Font* font, std::string text);

    protected:

        void onRender(GraphicsContext* context, Rect rootRect, double dt) override;

        OBJECT_NAME(TextButton)

    };
}

#endif //HAGAME2_TEXTBUTTON_H
