//
// Created by henry on 6/15/24.
//

#ifndef HAGAME2_ANIMATEDSPRITE_H
#define HAGAME2_ANIMATEDSPRITE_H

#include "animation.h"
#include "textureAtlas.h"
#include "texture.h"
#include "hagame/utils/config.h"

namespace hg::graphics {
    class AnimatedSprite : public utils::JsonConfigurable {
    public:

        AnimatedSprite(TextureAtlasGroup group);

        bool animated = false;
        int fps = 10;
        bool looping = false;

        TextureAtlasGroup& group();

        nlohmann::json save() const override;
        void load(nlohmann::json const& json) override;

    private:

        TextureAtlasGroup m_atlasGroup;

        bool m_playing = true;

    };

    class AnimatedSpriteStore {
    public:

        const std::string IDENTITY = "identity";

        AnimatedSpriteStore(Texture* texture, TextureAtlas* atlas);

        void setTexture(Texture* texture);
        void setAtlas(TextureAtlas* atlas);

        bool hasSprite(std::string const& name);
        void addSprite(std::string name, TextureAtlasGroup group);
        AnimatedSprite* getSprite(std::string name);
        std::unordered_map<std::string, AnimatedSprite>& sprites();
        void removeSprite(std::string name);
        void renameSprite(std::string name, std::string newName);
        void forEach(std::function<void(std::string, AnimatedSprite*)> fn);
        size_t size() const;
        void clear();

        HG_GET_PTR(Texture, texture);

    private:

        std::unordered_map<std::string, AnimatedSprite> m_sprites;
        Texture* m_texture;
        TextureAtlas* m_atlas;

    };
}

#endif //HAGAME2_ANIMATEDSPRITE_H
