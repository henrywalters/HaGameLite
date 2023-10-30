//
// Created by henry on 3/27/23.
//

#ifndef HAGAME2_TEXTRENDERER_H
#define HAGAME2_TEXTRENDERER_H

#include "../../core/component.h"
#include "../../math/aliases.h"
#include "../text.h"
#include "../color.h"
#include "../textBuffer.h"

namespace hg::graphics {
    class TextRenderer : public Component {
    public:

        TextRenderer(Font* font):
            m_buffer(std::make_unique<TextBuffer>(font))
        {}

        Color color = Color::black();

        TextBuffer* buffer() { return m_buffer.get(); }

    private:
        std::unique_ptr<TextBuffer> m_buffer;
    };
}



#endif //HAGAME2_TEXTRENDERER_H
