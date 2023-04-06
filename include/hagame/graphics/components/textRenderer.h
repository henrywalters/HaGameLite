//
// Created by henry on 3/27/23.
//

#ifndef HAGAME2_TEXTRENDERER_H
#define HAGAME2_TEXTRENDERER_H

#include "../../core/component.h"
#include "../../math/aliases.h"
#include "../text.h"
#include "../color.h"

namespace hg::graphics {
    class TextRenderer : public Component {
    public:
        bool fixedWidth = false;
        hg::Vec3 pos;
        hg::Vec3 size;
        Color color = Color::black();
        std::string message;
        std::string font;
        TextHAlignment hAlignment = TextHAlignment::Left;
        TextVAlignment vAlignment = TextVAlignment::Top;
    };
}



#endif //HAGAME2_TEXTRENDERER_H
