//
// Created by henry on 6/16/24.
//

#ifndef HAGAME2_SPRITESHEETV2_H
#define HAGAME2_SPRITESHEETV2_H

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

        void save(utils::Config& config) override;
        void load(const utils::Config& config) override;

        nlohmann::json save() const override;
        void load(nlohmann::json const& json) override;

    private:

        const std::string META = "meta";
        const std::string SPRITE = "sprite_";

        std::string m_activeSprite;
        std::string m_texturePath;
    };

    class SpriteSheetAsset : public Asset<SpriteSheetV2> {
    public:
        using Asset::Asset;
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_SPRITESHEETV2_H
