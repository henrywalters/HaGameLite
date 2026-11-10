//
// Created by henry on 12/29/23.
//

#ifndef HAGAME2_TILEMAP_COMPONENT_H
#define HAGAME2_TILEMAP_COMPONENT_H

#include <format>

#include "../../core/entity.h"
#include "../../utils/spatialHash.h"
#include "hagame/utils/tilemap.h"

namespace hg::components {

    ENUM(TileType)
    ENUM_VALUE(TileType, Color)
    ENUM_VALUE(TileType, Sprite)
    ENUM_VALUE(TileType, SpriteSheet)

    struct ColorTile {
        hg::graphics::Color color;
    };

    struct SpriteTile {
        std::string texture;
    };

    struct SpriteSheetTile {
        std::string sheet;
        hg::Vec2i index;
    };

    using TileDetails = std::variant<ColorTile, SpriteTile, SpriteSheetTile>;

    struct Tile {
        std::string name;
        bool collider;
        TileType::type type;
        TileDetails details;
    };

    struct TilemapLayer {
        Tile tile;
        utils::SpatialHash2D<int> map;
    };

    class Tilemap : public Component, public utils::Tilemap {
    public:

        std::vector<TilemapLayer> layers;

        bool hasTile(hg::Vec2i tile) {
            for (const auto& layer : layers) {
                if (layer.map.has(tile)) return true;
            }
            return false;
        }

        void removeTile(hg::Vec2i tile) {
            for (auto& layer : layers) {
                layer.map.remove(tile);
            }
        }

        // The outlines of the regions covered by collider tiles that fill their cell and by the axis aligned rects of
        // sprite sheet cell colliders, as closed loops in the tilemap's local space. Outer boundaries wind
        // counter-clockwise and holes clockwise, so the solid side is always on the left
        std::vector<std::vector<hg::Vec2>> getCollisionOutlines() const;

        // The sprite sheet cell collider shapes (see SpriteSheetV2::getCellColliders) that aren't in the outlines,
        // like slopes, one polygon per tile in the tilemap's local space
        std::vector<std::vector<hg::Vec2>> getCollisionPolygons() const;

    protected:
        OBJECT_NAME(Tilemap)
    };

    inline void to_json(nlohmann::json& j, Tile const& tile) {
        j["collider"] = tile.collider;
        j["name"] = tile.name;
        j["details"] = std::visit(utils::overloaded{
            [&](ColorTile const& t) {
                nlohmann::json details;
                details["type"] = TileType::Color.label;
                details["color"] = t.color;
                return details;
            },
            [&](SpriteTile const& t) {
                nlohmann::json details;
                details["type"] = TileType::Sprite.label;
                details["texture"] = t.texture;
                return details;
            },
            [&](SpriteSheetTile const& t) {
                nlohmann::json details;
                details["type"] = TileType::SpriteSheet.label;
                details["sheet"] = t.sheet;
                details["index"] = t.index;
                return details;
            }
        }, tile.details);
    }

    inline void from_json(nlohmann::json const& j, Tile& tile) {
        auto type = j["details"]["type"].get<std::string>();
        tile.collider = j["collider"].get<bool>();
        tile.name = j["name"].get<std::string>();
        if (type == TileType::Color.label) {
            tile.type = TileType::Color;
            tile.details = ColorTile{
                .color = j["details"]["color"].get<hg::graphics::Color>()
            };
        } else if (type == TileType::Sprite.label) {
            tile.type = TileType::Sprite;
            tile.details = SpriteTile{
                .texture = j["details"]["texture"].get<std::string>()
            };
        } else if (type == TileType::SpriteSheet.label) {
            tile.type = TileType::SpriteSheet;
            tile.details = SpriteSheetTile{
                .sheet = j["details"]["sheet"].get<std::string>(),
                .index = j["details"]["index"].get<Vec2i>()
            };
        }
        else {
            throw std::runtime_error(std::format("Unsupported Tile Type: '{}' in from_json", type));
        }
    }

    inline void to_json(nlohmann::json& j, TilemapLayer const& layer) {
        j["tile"] = layer.tile;
        j["map"] = layer.map;
    }

    inline void from_json(nlohmann::json const& j, TilemapLayer& layer) {
        layer.tile = j["tile"].get<Tile>();
        layer.map = j["map"].get<utils::SpatialHash2D<int>>();
    }

    HG_COMPONENT(Common, Tilemap)
    HG_JSON_FIELD(Tilemap, TilemapLayer, layers)
    HG_FIELD(Tilemap, hg::Vec2, tileSize)

}


#endif //HAGAME2_TILEMAP_COMPONENT_H
