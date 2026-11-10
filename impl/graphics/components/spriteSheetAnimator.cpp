//
// Created by henry on 9/8/26.
//
#include <hagame/graphics/components/spriteSheetAnimator.h>

using namespace hg::graphics;
using namespace hg::graphics::components;

AnimatedSprite* SpriteSheetAnimator::getSprite() {
    auto sheet = sprites.asset();
    if (!sheet) return nullptr;
    if (!sheet->sprites->hasSprite(activeSprite)) return nullptr;
    return sheet->sprites->getSprite(activeSprite);
}

void SpriteSheetAnimator::play() {
    m_playing = true;
}

void SpriteSheetAnimator::stop() {
    m_playing = false;
}

void SpriteSheetAnimator::reset() {
    currentIndex = 0;
    m_elapsedTime = 0;

    auto sprite = getSprite();

    if (!sprite || !sprite->looping) {
        m_playing = false;
    }
}

void SpriteSheetAnimator::update(double dt) {
    auto sprite = getSprite();
    if (!sprite) return;

    auto& group = sprite->group();

    if (!m_playing || group.size() < 1) {
        return;
    }

    double frameRate = 1.0 / (double) sprite->fps;
    m_elapsedTime += dt;
    int frame = m_elapsedTime / frameRate;

    if (sprite->looping) {
        currentIndex = frame % group.size();
    } else {
        currentIndex = hg::math::clamp<int>(frame, 0, group.size() - 1);
    }


    if (frame > 0 && currentIndex == 0) {
        currentIndex = currentIndex % group.size();
        onFinish.emit(this);
    }
}

hg::Rect SpriteSheetAnimator::getRect() {
    auto sheet = sprites.asset();
    if (!sheet) return Rect();
    auto sprite = getSprite();
    if (!sprite) return Rect();

    auto& group = sprite->group();

    if (group.size() == 0) {
        return Rect();
    }
    return sheet->atlas.getRect(group[currentIndex], sheet->texture()->image->size);
}

void SpriteSheetAnimator::setSprite(const std::string& name) {
    if (name == activeSprite) {
        return;
    }
    activeSprite = name;
    reset();
    play(); // reset() stops non-looping sprites, but a newly chosen one should play once
}

hg::Rect SpriteSheetAnimator::drawRect() {
    auto rect = getRect();
    if (flipX) {
        // Sample the frame right-to-left
        rect.pos[0] += rect.size[0];
        rect.size[0] = -rect.size[0];
    }
    return rect;
}

hg::Vec2 SpriteSheetAnimator::drawOffset() const {
    return flipX ? hg::Vec2(-offset[0], offset[1]) : offset;
}
