//
// Created by henry on 12/29/23.
//

#ifndef HAGAME2_TILEMAP_COMPONENT_H
#define HAGAME2_TILEMAP_COMPONENT_H

#include "../../core/entity.h"
#include "../../core/component.h"
#include "../../utils/spatialMap.h"

namespace hg::graphics::components {
    class Tilemap : public hg::Component {
    public:

        enum class TileType {
            Color,
            Texture,
        };

        struct Tile {
            TileType type;
            hg::Vec2i index;
            Color color;
            std::string texture;
        };

        hg::Vec2 tileSize = hg::Vec2::Identity();
        hg::utils::SpatialMap2D<Tile, int> tiles;

        void load(utils::Config* config, std::string section) override;
        void save(utils::Config* config, std::string section) override;

    protected:

        OBJECT_NAME(Tilemap)

    };

    HG_COMPONENT(Graphics, Tilemap)
    HG_FIELD(Tilemap, hg::Vec2, tileSize)
}

#endif //HAGAME2_TILEMAP_COMPONENT_H
