//
// Created by henry on 9/25/23.
//

#ifndef HAGAME2_TEXTBUFFER_H
#define HAGAME2_TEXTBUFFER_H

#include "text.h"

namespace hg::graphics {
    class TextBuffer {
    public:

    private:

        struct

        Font* m_font;
        std::vector<std::string> m_text;
        Vec3 m_pos;
        Vec3 m_size;

        void updateBuffer();

    };
}

#endif //HAGAME2_TEXTBUFFER_H
