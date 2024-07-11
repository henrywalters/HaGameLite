//
// Created by henry on 6/16/24.
//

#ifndef HAGAME2_SPRITESHEETV2_H
#define HAGAME2_SPRITESHEETV2_H

#include "textureAtlas.h"
#include "animatedSprite.h"
#include "../utils/store.h"
#include "../utils/config.h"

namespace hg::graphics {
    class SpriteSheetV2 : public Animation, public utils::Configurable {
    public:

        SpriteSheetV2(std::string path);
        SpriteSheetV2(const utils::Config& config);

        TextureAtlas atlas;
        AnimatedSpriteStore sprites;

        void play() override;
        void update(double dt) override;
        void stop() override;
        void reset() override;

        Texture* texture();

        void save(utils::Config& config) override;
        void load(const utils::Config& config) override;

    private:

        const std::string META = "meta";
        const std::string SPRITE = "sprite_";

        std::string m_activeSprite;
        std::string m_path;
        Texture m_texture;
    };
}

#endif //HAGAME2_SPRITESHEETV2_H
