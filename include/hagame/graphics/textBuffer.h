//
// Created by henry on 9/25/23.
//

#ifndef HAGAME2_TEXTBUFFER_H
#define HAGAME2_TEXTBUFFER_H

#include "text.h"
#include "../utils/watcher.h"

namespace hg::graphics {
    class TextBuffer {
    public:

        TextBuffer();
        TextBuffer(Font* font, std::string text, Vec3 pos, TextHAlignment hAlignment = TextHAlignment::Center, TextVAlignment vAlignment = TextVAlignment::Center);
        TextBuffer(Font* font, std::string text, Vec3 pos, Vec3 size, TextHAlignment hAlignment = TextHAlignment::Center, TextVAlignment vAlignment = TextVAlignment::Center);

        HG_GET_SET_PTR(Font, font, updateBuffers);
        HG_GET_SET(std::string, text, updateBuffers);
        HG_GET_SET(TextHAlignment, hAlignment, updateBuffers);
        HG_GET_SET(TextVAlignment, vAlignment, updateBuffers);
        HG_GET(Vec3, size);

        void limitSize(const Vec3& size) {
            m_size = size;
            m_limitSize = true;
            updateBuffers();
        }

        void uncapSize() {
            m_limitSize = false;
            m_size = Vec3::Zero();
            updateBuffers();
        }

        void render();

    private:

        struct Character {
            primitives::Quad quad;
            MeshInstance mesh;
            std::unique_ptr<VertexBuffer<Vec3>> buffer;

            Character(primitives::Quad _quad, const std::vector<Vec3> data):
                quad(_quad),
                mesh(MeshInstance(&quad)),
                buffer(VertexBuffer<Vec3>::Dynamic(data))
            {}
        };

        std::unordered_map<char, std::shared_ptr<Character>> m_characters;

        Font* m_font;
        std::string m_text;
        Vec3 m_pos;
        bool m_limitSize;
        Vec3 m_size;
        TextHAlignment m_hAlignment;
        TextVAlignment m_vAlignment;

        void updateBuffers();

    };
}

#endif //HAGAME2_TEXTBUFFER_H
