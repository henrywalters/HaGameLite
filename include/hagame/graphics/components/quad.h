//
// Created by henry on 3/25/23.
//

#ifndef HAGAME2_QUAD_H
#define HAGAME2_QUAD_H

#include "../../core/component.h"
#include "../../core/entity.h"
#include "../primitives/quad.h"
#include "../color.h"
#include "../mesh.h"
#include "../../math/aliases.h"

namespace hg::graphics {
    class Quad : public Component {
    public:

        Vec2 size = Vec2(1.0);
        Vec2 offset;
        Color color = Color::white();

    protected:

        OBJECT_NAME(Quad)

    };

    HG_COMPONENT(Graphics, Quad)
    HG_FIELD(Quad, hg::Vec2, size)
    HG_FIELD(Quad, hg::Vec2, offset)
    HG_FIELD(Quad, hg::graphics::Color, color)

}

#endif //HAGAME2_QUAD_H
