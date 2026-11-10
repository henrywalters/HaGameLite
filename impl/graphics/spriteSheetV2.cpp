//
// Created by henry on 6/17/24.
//
#include <hagame/graphics/spriteSheetV2.h>
#include <hagame/core/assets.h>
#include <hagame/utils/serialization.h>

using namespace hg::graphics;

SpriteSheetV2::SpriteSheetV2(std::string const& texturePath) :
    m_texturePath(texturePath)
{
    sprites = std::make_unique<AnimatedSpriteStore>(getTexture(m_texturePath), &atlas);
}

SpriteSheetV2::SpriteSheetV2(nlohmann::json const &json)
{
    sprites = std::make_unique<AnimatedSpriteStore>(nullptr, &atlas);
    load(json);
    auto texture = getTexture(m_texturePath);
    sprites->setTexture(texture);
}

SpriteSheetV2::SpriteSheetV2(const hg::utils::Config &config)
{
    sprites = std::make_unique<AnimatedSpriteStore>(nullptr, &atlas);
    load(config);
    auto texture = getTexture(m_texturePath);
    sprites->setTexture(texture);
}

Texture *SpriteSheetV2::texture() {
    return sprites->texture();
}

void SpriteSheetV2::save(hg::utils::Config &config) {
    config.addSection(META);
    config.setRaw(META, "texturePath", m_texturePath);
    config.setArray<int, 2>(META, "size", atlas.size.vector);
    config.set(META, "sprites", sprites->size());

    int index = 0;
    sprites->forEach([&](std::string key, AnimatedSprite* sprite) {
        auto section = SPRITE + std::to_string(index++);
        config.addSection(section);
        config.set(section, "animated", sprite->animated);
        config.set(section, "fps", sprite->fps);
        config.set(section, "looping", sprite->looping);
        config.setRaw(section, "key", key);
        config.setListOfVectors(section, "members", sprite->group());
    });
}

nlohmann::json SpriteSheetV2::save() const {
    nlohmann::json json;
    json["texturePath"] = m_texturePath;
    json["size"] = atlas.size;

    nlohmann::json sJson;

    sprites->forEach([&](std::string const& key, AnimatedSprite* sprite) {
        sJson[key] = sprite->save();
    });

    json["sprites"] = sJson;

    if (!m_cellColliders.empty()) {
        nlohmann::json colliders = nlohmann::json::array();
        for (const auto& [cell, shapes] : m_cellColliders) {
            nlohmann::json collider;
            collider["cell"] = hg::Vec2i(cell.first, cell.second);
            collider["shapes"] = shapes;
            colliders.push_back(collider);
        }
        json["cellColliders"] = colliders;
    }

    return json;
}

std::optional<std::vector<std::vector<hg::Vec2>>> SpriteSheetV2::getCellColliders(hg::Vec2i cell) const {
    auto it = m_cellColliders.find({cell[0], cell[1]});
    if (it == m_cellColliders.end()) {
        return std::nullopt;
    }
    return it->second;
}

void SpriteSheetV2::setCellColliders(hg::Vec2i cell, std::vector<std::vector<hg::Vec2>> shapes) {
    m_cellColliders[{cell[0], cell[1]}] = shapes;
}

void SpriteSheetV2::clearCellColliders(hg::Vec2i cell) {
    m_cellColliders.erase({cell[0], cell[1]});
}

void SpriteSheetV2::load(const hg::utils::Config &config) {
    m_texturePath = config.getRaw(META, "texturePath");
    config.getArray<int, 2>(META, "size", atlas.size.vector);
    auto count = config.get<int>(META, "sprites");

    for (int i = 0; i < count; i++) {
        auto section = SPRITE + std::to_string(i);
        auto key = config.getRaw(section, "key");
        auto members = config.getListOfVectors<int, 2>(section, "members");
        sprites->addSprite(key, members);
        sprites->getSprite(key)->animated = config.get<int>(section, "animated");
        sprites->getSprite(key)->looping = config.get<bool>(section, "looping");
        sprites->getSprite(key)->fps = config.get<int>(section, "fps");
    }
}

void SpriteSheetV2::load(nlohmann::json const &json) {
    m_texturePath = json["texturePath"].get<std::string>();
    atlas.size = json["size"].get<hg::Vec2i>();

    sprites->clear();

    for (const auto& [key, sprite]: json["sprites"].items()) {
        sprites->addSprite(key, TextureAtlasGroup{});
        sprites->getSprite(key)->load(sprite);
    }

    m_cellColliders.clear();

    if (json.contains("cellColliders")) {
        for (const auto& collider : json["cellColliders"]) {
            auto cell = collider["cell"].get<hg::Vec2i>();
            if (collider.contains("shapes")) {
                setCellColliders(cell, collider["shapes"].get<std::vector<std::vector<hg::Vec2>>>());
            } else {
                // Saved when a cell had a single shape. No points meant no collision
                auto points = collider["points"].get<std::vector<hg::Vec2>>();
                setCellColliders(cell, points.empty() ? std::vector<std::vector<hg::Vec2>>{} : std::vector<std::vector<hg::Vec2>>{points});
            }
        }
    }
}

void SpriteSheetAsset::reload() {
    m_asset = hasSpriteSheet(m_path) ? getSpriteSheet(m_path) : nullptr;
}
