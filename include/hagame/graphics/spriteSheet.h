//
// Created by henry on 11/5/23.
//

#ifndef HAGAME2_SPRITESHEET_H
#define HAGAME2_SPRITESHEET_H

#include <optional>
#include "texture.h"
#include "animation.h"
#include "../utils/config.h"

namespace hg::graphics {
    struct SpriteGroup {
        std::vector<hg::Vec2i> members;
    };

    class SpriteSheet : public Animation {
    public:

        SpriteSheet(std::string path):
            m_path(path),
            m_texture(path),
            size(hg::Vec2i(1, 1))
        {}

        SpriteSheet(utils::MultiConfig& config);

        hg::Vec2i size;
        bool animated = false;
        int fps = 10.0f;
        int frameCount = 1;
        bool looping = false;

        void play() override;
        void update(double dt) override;
        void stop() override;
        void reset() override;

        hg::Rect getRect(hg::Vec2i index);
        hg::Rect getRect();

        std::optional<std::string> activeGroup();
        void activateGroup(std::string group);
        void addGroup(std::string group);
        void addToGroup(hg::Vec2i index);
        void addToGroup(std::string group, hg::Vec2i index);
        void removeFromGroup(std::string group, hg::Vec2i index);
        void removeFromGroup(hg::Vec2i index);
        std::vector<hg::Vec2i> members();
        SpriteGroup* getGroup(std::string group);
        std::unordered_map<std::string, SpriteGroup>* groups();

        hg::graphics::Texture* texture();

        void save(utils::MultiConfig& config);
        void load(utils::MultiConfig& config);

    private:

        std::string m_path;
        hg::graphics::Texture m_texture;
        std::unordered_map<std::string, SpriteGroup> m_groups;
        std::optional<std::string> m_selectedGroup = std::nullopt;

        double m_elapsedTime = 0;
        int m_currentIndex = 0;
        bool m_playing = true;
    };
}



#endif //HAGAME2_SPRITESHEET_H
