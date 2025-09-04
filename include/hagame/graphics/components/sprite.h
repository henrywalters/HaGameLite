//
// Created by henry on 3/5/23.
//

#ifndef HAGAME2_SPRITE_H
#define HAGAME2_SPRITE_H

#include "quad.h"
#include "../../core/entity.h"
#include "hagame/graphics/texture.h"

namespace hg::graphics {
    class Sprite : public Quad {
    public:

        TextureAsset texture;
        bool visible = true;
        bool batched = false;

    protected:

        OBJECT_NAME(Sprite)

    };

    HG_COMPONENT(Graphics, Sprite)
    HG_FIELD(Sprite, hg::graphics::TextureAsset, texture)
    HG_FIELD(Sprite, bool, visible)
    HG_FIELD(Sprite, bool, batched)
    HG_FIELD(Sprite, hg::Vec2, size)
    HG_FIELD(Sprite, hg::Vec2, offset)
    HG_FIELD(Sprite, hg::graphics::Color, color)
}

#endif //HAGAME2_SPRITE_H
