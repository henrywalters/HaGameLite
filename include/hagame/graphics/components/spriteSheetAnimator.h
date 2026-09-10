//
// Created by henry on 11/7/23.
//

#ifndef HAGAME2_SPRITESHEETANIMATOR_H
#define HAGAME2_SPRITESHEETANIMATOR_H

#include "quad.h"
#include "../spriteSheet.h"
#include "hagame/graphics/spriteSheetV2.h"

namespace hg::graphics::components {
    class SpriteSheetAnimator : public Quad, public Animation {
    public:

        SpriteSheetAnimator()
        {}

        SpriteSheetAsset sprites;
        std::string activeSprite;
        int currentIndex = 0;

        AnimatedSprite* getSprite();

        void play() override;
        void update(double dt) override;
        void stop() override;
        void reset() override;

        hg::Rect getRect();

    protected:

        OBJECT_NAME(SpriteSheetAnimator)

    private:

        bool m_playing = true;
        double m_elapsedTime;

    };

    HG_COMPONENT(Graphics, SpriteSheetAnimator)
    HG_FIELD(SpriteSheetAnimator, hg::graphics::SpriteSheetAsset, sprites)
    HG_FIELD(SpriteSheetAnimator, std::string, activeSprite)
    HG_FIELD(SpriteSheetAnimator, hg::Vec2, size)
    HG_FIELD(SpriteSheetAnimator, hg::Vec2, offset)
    HG_FIELD(SpriteSheetAnimator, hg::graphics::Color, color)
    HG_FIELD(SpriteSheetAnimator, bool, centered)
}

#endif //HAGAME2_SPRITESHEETANIMATOR_H
