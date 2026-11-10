//
// Created by henry on 6/16/24.
//

#ifndef HAGAME2_SPRITESHEETV2_H
#define HAGAME2_SPRITESHEETV2_H

#include <map>
#include <optional>

#include "textureAtlas.h"
#include "animatedSprite.h"
#include "../utils/config.h"
#include "nlohmann/json.hpp"

namespace hg::graphics {
    class SpriteSheetV2 : public utils::Configurable, public utils::JsonConfigurable {
    public:

        SpriteSheetV2(nlohmann::json const& json);
        SpriteSheetV2(std::string const& texturePath);
        SpriteSheetV2(const utils::Config& config);

        TextureAtlas atlas;
        std::unique_ptr<AnimatedSpriteStore> sprites;

        Texture* texture();

        // The texture's asset name, e.g. "textures/tiles"
        std::string texturePath() const { return m_texturePath; }

        // Collision shapes for cells, for sheets used as tilesets. Each shape is a convex polygon of up to 8 points in
        // the cell's normalized space, (0, 0) bottom-left to (1, 1) top-right; several make concave shapes like an L.
        // A cell without any collides as a full tile, and an empty list means it doesn't collide at all.
        std::optional<std::vector<std::vector<hg::Vec2>>> getCellColliders(hg::Vec2i cell) const;
        void setCellColliders(hg::Vec2i cell, std::vector<std::vector<hg::Vec2>> shapes);
        // Back to colliding as a full tile
        void clearCellColliders(hg::Vec2i cell);

        void save(utils::Config& config) override;
        void load(const utils::Config& config) override;

        nlohmann::json save() const override;
        void load(nlohmann::json const& json) override;

    private:

        const std::string META = "meta";
        const std::string SPRITE = "sprite_";

        std::string m_activeSprite;
        std::string m_texturePath;

        std::map<std::pair<int, int>, std::vector<std::vector<hg::Vec2>>> m_cellColliders;
    };

    class SpriteSheetAsset : public Asset<SpriteSheetV2> {
    public:
        using Asset::Asset;
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_SPRITESHEETV2_H
