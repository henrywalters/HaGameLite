//
// Created by henry on 3/5/23.
//

#ifndef HAGAME2_SPRITE_H
#define HAGAME2_SPRITE_H

#include "quad.h"

namespace hg::graphics {
    class Sprite : public Quad {
    public:

        Sprite(Vec2 size, Vec2 offset = Vec2::Zero()):
            Quad(size, offset)
        {}

        std::string texture;

    protected:
        std::string toString() const override {
            return "Sprite<" + std::to_string(id()) + ">";
        }

    };
}

#endif //HAGAME2_SPRITE_H
