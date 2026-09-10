//
// Created by henry on 8/23/26.
//

#ifndef HAGAME2_TILESET_H
#define HAGAME2_TILESET_H

#include <string>
#include <variant>

#include "color.h"
#include "spriteSheetV2.h"
#include "texture.h"
#include "hagame/core/asset.h"

namespace hg::graphics {

    ENUM(TileType)
    ENUM_VALUE(TileType, Color)
    ENUM_VALUE(TileType, Sprite)
    ENUM_VALUE(TileType, SpriteSheet)

    struct ColorTile {
        Color color;
    };

    struct SpriteTile {
        TextureAsset texture;
    };

    struct SpriteSheetTile {
        SpriteSheetAsset* sheet;
        Vec2i index;
    };

    using TileDisplay = std::variant<ColorTile, SpriteTile, SpriteSheetTile>;

    struct Tile {
        utils::UUID id;
        std::string name;
        bool collider;
        TileType::type type;
        TileDisplay details;
    };

    class Tileset {
    public:

        // void save(std::string const& path);
        // void load(std::string const& path);
        //
        // Tile* addTile();
        // void removeTile(Tile* tile);


    private:

        std::unordered_map<std::string, SpriteSheetAsset> m_spriteSheets;
        std::vector<Tile> m_tiles;

    };

    class TilesetAsset : public Asset<Tileset> {
    protected:
        //void reload() override;
    };

}

#endif //HAGAME2_TILESET_H
