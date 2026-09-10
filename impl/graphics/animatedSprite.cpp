//
// Created by henry on 6/16/24.
//
#include "hagame/graphics/animatedSprite.h"
#include "hagame/utils/serialization.h"

#include "nlohmann/json.hpp"

using namespace hg::graphics;

AnimatedSprite::AnimatedSprite(TextureAtlasGroup group):
    m_atlasGroup(group)
{

}
//
// void AnimatedSprite::update(double dt) {
//
//     if (!m_playing || !animated || m_atlasGroup.size() < 1) {
//         return;
//     }
//
//     double frameRate = 1.0 / (double) fps;
//     m_elapsedTime += dt;
//     int frame = m_elapsedTime / frameRate;
//
//     if (looping) {
//         currentIndex = frame % m_atlasGroup.size();
//     } else {
//         currentIndex = hg::math::clamp<int>(frame, 0, m_atlasGroup.size() - 1);
//     }
//
//
//     if (frame > 0 && currentIndex == 0) {
//         currentIndex = currentIndex % m_atlasGroup.size();
//         onFinish.emit(this);
//     }
// }
//
// void AnimatedSprite::play() {
//     m_playing = true;
// }
//
// void AnimatedSprite::stop() {
//     m_playing = false;
// }
//
// void AnimatedSprite::reset() {
//     currentIndex = 0;
//     m_elapsedTime = 0;
//     if (!looping) {
//         m_playing = false;
//     }
// }

// hg::Rect AnimatedSprite::getRect() const {
//     if (m_atlasGroup.size() == 0) {
//         return Rect();
//     }
//     return m_atlas->getRect(m_atlasGroup[currentIndex], m_texture->image->size);
//     Vec2 imageSize = m_texture->image->size.cast<float>();
//     Vec2 cellSize = m_texture->image->size.div(m_atlas->size).cast<float>();
//     Vec2 cellPos = cellSize.prod(m_atlasGroup[currentIndex].cast<float>()).div(imageSize);
//     return Rect(cellPos, cellSize.div(imageSize));
// }

nlohmann::json AnimatedSprite::save() const {
    nlohmann::json json;
    json["animated"] = animated;
    json["fps"] = fps;
    json["looping"] = looping;
    json["members"] = m_atlasGroup;
    return json;
}

void AnimatedSprite::load(nlohmann::json const& json) {
    animated = json["animated"].get<bool>();
    fps = json["fps"].get<int>();
    looping = json["looping"].get<bool>();
    m_atlasGroup = json["members"].get<std::vector<hg::Vec2i>>();
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
    m_sprites.insert(std::make_pair(name, AnimatedSprite(group)));
}

void AnimatedSpriteStore::renameSprite(std::string name, std::string newName) {
    auto sprite = m_sprites.extract(name);
    if (!sprite.empty()) {
        sprite.key() = newName;
        m_sprites.insert(std::move(sprite));
    }
}

void AnimatedSpriteStore::removeSprite(std::string name) {
    m_sprites.erase(name);
}

bool AnimatedSpriteStore::hasSprite(std::string const& name) {
    return m_sprites.find(name) != m_sprites.end();
}

AnimatedSprite* AnimatedSpriteStore::getSprite(std::string name) {
    if (m_sprites.find(name) == m_sprites.end()) {
        throw std::runtime_error("Sprite: " + name + " does not exist in store");
    }
    return &m_sprites.at(name);
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

void AnimatedSpriteStore::clear() {
    m_sprites.clear();
}
