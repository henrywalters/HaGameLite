//
// Created by henry on 3/5/23.
//

#ifndef HAGAME2_SPRITE_H
#define HAGAME2_SPRITE_H

#include "quad.h"
#include "../../core/entity.h"

namespace hg::graphics {
    class Sprite : public Quad {
    public:

        std::string texture;
        bool visible = true;

    protected:

        OBJECT_NAME(Sprite)

    };

    HG_COMPONENT(Graphics, Sprite)
    HG_FIELD(Sprite, std::string, texture)
    HG_FIELD(Sprite, bool, visible)
    HG_FIELD(Sprite, hg::Vec2, size)
    HG_FIELD(Sprite, hg::Vec2, offset)
    HG_FIELD(Sprite, hg::graphics::Color, color)
}

#endif //HAGAME2_SPRITE_H
