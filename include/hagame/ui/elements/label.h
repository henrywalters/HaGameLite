//
// Created by henry on 8/7/24.
//

#ifndef HAGAME2_LABEL_H
#define HAGAME2_LABEL_H

#include "../element.h"
#include "../mixins/textDisplay.h"
#include "../../graphics/textBuffer.h"

namespace hg::ui {
    class Label : public Element, public mixins::TextDisplay {
    public:
        Label(Vec2 pos, graphics::Font* font, std::string text);
        void render(GraphicsContext* context, Rect rootRect, double dt) override;
    protected:

        OBJECT_NAME(Label)

    };
}

#endif //HAGAME2_LABEL_H
