//
// Created by henry on 6/15/24.
//

#ifndef HAGAME2_ANIMATEDSPRITE_H
#define HAGAME2_ANIMATEDSPRITE_H

#include "animation.h"
#include "textureAtlas.h"
#include "texture.h"

namespace hg::graphics {
    class AnimatedSprite : public hg::graphics::Animation {
    public:

        AnimatedSprite(Texture* texture, TextureAtlas* atlas, TextureAtlasGroup group);

        bool animated = false;
        int fps = 10;
        bool looping = false;

        void play() override;
        void update(double dt) override;
        void stop() override;
        void reset() override;

        hg::Rect getRect() const;
        TextureAtlasGroup& group();

    private:

        Texture* m_texture;
        TextureAtlas* m_atlas;
        TextureAtlasGroup m_atlasGroup;

        double m_elapsedTime = 0;
        int m_currentIndex = 0;
        bool m_playing = true;

    };

    class AnimatedSpriteStore {
    public:

        const std::string IDENTITY = "identity";

        AnimatedSpriteStore(Texture* texture, TextureAtlas* atlas);

        void setTexture(Texture* texture);
        void setAtlas(TextureAtlas* atlas);

        void addSprite(std::string name, TextureAtlasGroup group);
        AnimatedSprite* active();
        AnimatedSprite* getSprite(std::string name);
        std::unordered_map<std::string, AnimatedSprite>& sprites();
        void removeSprite(std::string name);
        std::string activeSprite();
        void setSprite(std::string name);
        void forEach(std::function<void(std::string, AnimatedSprite*)> fn);
        size_t size() const;

    private:

        std::string m_activeSprite;
        std::unordered_map<std::string, AnimatedSprite> m_sprites;
        Texture* m_texture;
        TextureAtlas* m_atlas;

    };
}

#endif //HAGAME2_ANIMATEDSPRITE_H
