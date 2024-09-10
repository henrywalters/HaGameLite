//
// Created by henry on 6/16/24.
//
#include "../../../include/hagame/graphics/animatedSprite.h"

using namespace hg::graphics;

AnimatedSprite::AnimatedSprite(Texture* texture, TextureAtlas* atlas, TextureAtlasGroup group):
    m_texture(texture),
    m_atlas(atlas),
    m_atlasGroup(group)
{

}

void AnimatedSprite::update(double dt) {

    if (!m_playing || !animated || m_atlasGroup.size() < 1) {
        return;
    }

    double frameRate = 1.0 / (double) fps;
    m_elapsedTime += dt;
    int frame = m_elapsedTime / frameRate;

    if (looping) {
        m_currentIndex = frame % m_atlasGroup.size();
    } else {
        m_currentIndex = hg::math::clamp<int>(frame, 0, m_atlasGroup.size());
    }


    if (frame > 0 && m_currentIndex == 0) {
        m_currentIndex = m_currentIndex % m_atlasGroup.size();
        onFinish.emit(this);
    }
}

void AnimatedSprite::play() {
    m_playing = true;
}

void AnimatedSprite::stop() {
    m_playing = false;
}

void AnimatedSprite::reset() {
    m_currentIndex = 0;
    m_elapsedTime = 0;
    if (!looping) {
        m_playing = false;
    }
}

hg::Rect AnimatedSprite::getRect() const {
    if (m_atlasGroup.size() == 0) {
        return Rect();
    }
    return m_atlas->getRect(m_atlasGroup[m_currentIndex], m_texture->image->size);
    Vec2 imageSize = m_texture->image->size.cast<float>();
    Vec2 cellSize = m_texture->image->size.div(m_atlas->size).cast<float>();
    Vec2 cellPos = cellSize.prod(m_atlasGroup[m_currentIndex].cast<float>()).div(imageSize);
    return Rect(cellPos, cellSize.div(imageSize));
}

TextureAtlasGroup &AnimatedSprite::group() {
    return m_atlasGroup;
}

AnimatedSpriteStore::AnimatedSpriteStore(Texture *texture, TextureAtlas *atlas) :
    m_texture(texture),
    m_atlas(atlas)
{
    addSprite(IDENTITY, m_atlas->identity());
}

void AnimatedSpriteStore::addSprite(std::string name, TextureAtlasGroup group) {
    m_sprites.insert(std::make_pair(name, AnimatedSprite(m_texture, m_atlas, group)));
    if (m_sprites.size() == 1) {
        m_activeSprite = name;
    }
}

void AnimatedSpriteStore::removeSprite(std::string name) {
    m_sprites.erase(name);
}

std::string AnimatedSpriteStore::activeSprite() {
    return m_activeSprite;
}

void AnimatedSpriteStore::setSprite(std::string name) {
    m_activeSprite = name;
}

AnimatedSprite* AnimatedSpriteStore::getSprite(std::string name) {
    if (m_sprites.find(name) == m_sprites.end()) {
        throw std::runtime_error("Sprite: " + name + " does not exist in store");
    }
    return &m_sprites.at(name);
}

AnimatedSprite* AnimatedSpriteStore::active() {
    return getSprite(m_activeSprite);
}

void AnimatedSpriteStore::forEach(std::function<void(std::string, AnimatedSprite *)> fn) {
    for (auto& [key, sprite] : m_sprites) {
        fn(key, &sprite);
    }
}

std::unordered_map<std::string, AnimatedSprite> &AnimatedSpriteStore::sprites() {
    return m_sprites;
}

size_t AnimatedSpriteStore::size() const {
    return m_sprites.size();
}

void AnimatedSpriteStore::setTexture(Texture *texture) {
    m_texture = texture;
}

void AnimatedSpriteStore::setAtlas(TextureAtlas *atlas) {
    m_atlas = atlas;
}
