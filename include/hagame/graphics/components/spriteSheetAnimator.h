//
// Created by henry on 11/7/23.
//

#ifndef HAGAME2_SPRITESHEETANIMATOR_H
#define HAGAME2_SPRITESHEETANIMATOR_H

#include "quad.h"
#include "../spriteSheet.h"

namespace hg::graphics::components {
    class SpriteSheetAnimator : public Quad {
    public:

        SpriteSheetAnimator():
            player(std::make_shared<AnimationPlayer<std::string>>())
        {}

        std::shared_ptr<AnimationPlayer<std::string>> player;

        void update(double dt) {
            player->update(dt);
        }

        void render();

    };
}

#endif //HAGAME2_SPRITESHEETANIMATOR_H
