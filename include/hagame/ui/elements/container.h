//
// Created by henry on 2/22/24.
//

#ifndef HAGAME2_CONTAINER_H
#define HAGAME2_CONTAINER_H

#include "../element.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"

namespace hg::ui {
    class Container : public Element {
    public:

        Container();

        bool wrapIndex = false;

    protected:

        void render(graphics::BatchRenderer* renderer, Rect rootRect, double dt) override;

        bool onTrigger(utils::enum_t event) override;

    private:

        int m_index = 0;
    };
}

#endif //HAGAME2_CONTAINER_H
