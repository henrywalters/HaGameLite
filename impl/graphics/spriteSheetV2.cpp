//
// Created by henry on 6/17/24.
//
#include "../../../include/hagame/graphics/spriteSheetV2.h"

using namespace hg::graphics;

SpriteSheetV2::SpriteSheetV2(std::string path) :
    m_path(path),
    m_texture(path),
    sprites(&m_texture, &atlas)
{

}

SpriteSheetV2::SpriteSheetV2(const hg::utils::Config &config):
    m_texture(hg::Vec2i::Zero()),
    sprites(&m_texture, &atlas)
{
    load(config);
    m_texture = Texture(m_path);
    sprites.setTexture(&m_texture);
}


void SpriteSheetV2::play() {
    sprites.active()->play();
}

void SpriteSheetV2::update(double dt) {
    sprites.active()->update(dt);
}

void SpriteSheetV2::stop() {
    sprites.active()->stop();
}

void SpriteSheetV2::reset() {
    sprites.active()->reset();
}

Texture *SpriteSheetV2::texture() {
    return &m_texture;
}

void SpriteSheetV2::save(hg::utils::Config &config) {
    config.addSection(META);
    config.setRaw(META, "path", m_path);
    config.setArray<int, 2>(META, "size", atlas.size.vector);
    config.set(META, "sprites", sprites.size());

    int index = 0;
    sprites.forEach([&](std::string key, AnimatedSprite* sprite) {
        auto section = SPRITE + std::to_string(index++);
        config.addSection(section);
        config.set(section, "animated", sprite->animated);
        config.set(section, "fps", sprite->fps);
        config.set(section, "looping", sprite->looping);
        config.setRaw(section, "key", key);
        config.setListOfVectors(section, "members", sprite->group());
    });
}

void SpriteSheetV2::load(const hg::utils::Config &config) {
    m_path = config.getRaw(META, "path");
    config.getArray<int, 2>(META, "size", atlas.size.vector);
    auto count = config.get<int>(META, "sprites");

    for (int i = 0; i < count; i++) {
        auto section = SPRITE + std::to_string(i);
        auto key = config.getRaw(section, "key");
        auto members = config.getListOfVectors<int, 2>(section, "members");
        sprites.addSprite(key, members);
        sprites.getSprite(key)->animated = config.get<int>(section, "animated");
        sprites.getSprite(key)->looping = config.get<bool>(section, "looping");
        sprites.getSprite(key)->fps = config.get<int>(section, "fps");
    }
}


