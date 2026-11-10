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
        bool flipX = false; // Mirror horizontally, e.g. to face left

        // Switch to another sprite in the sheet and play it from the start. Does nothing if it's already active.
        void setSprite(const std::string& name);

        AnimatedSprite* getSprite();

        void play() override;
        void update(double dt) override;
        void stop() override;
        void reset() override;

        hg::Rect getRect();

        // What renderers should draw: the current frame's texture rect and the quad offset, mirrored when flipX is set
        hg::Rect drawRect();
        hg::Vec2 drawOffset() const;

    protected:

        OBJECT_NAME(SpriteSheetAnimator)

    private:

        bool m_playing = true;
        double m_elapsedTime = 0;

    };

    HG_COMPONENT(Graphics, SpriteSheetAnimator)
    HG_FIELD(SpriteSheetAnimator, hg::graphics::SpriteSheetAsset, sprites)
    HG_FIELD(SpriteSheetAnimator, std::string, activeSprite)
    HG_FIELD(SpriteSheetAnimator, hg::Vec2, size)
    HG_FIELD(SpriteSheetAnimator, hg::Vec2, offset)
    HG_FIELD(SpriteSheetAnimator, hg::graphics::Color, color)
    HG_FIELD(SpriteSheetAnimator, bool, centered)
    HG_FIELD(SpriteSheetAnimator, bool, flipX)
}

#endif //HAGAME2_SPRITESHEETANIMATOR_H
