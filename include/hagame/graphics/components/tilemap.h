//
// Created by henry on 12/29/23.
//

#ifndef HAGAME2_TILEMAP_COMPONENT_H
#define HAGAME2_TILEMAP_COMPONENT_H

#include "../../core/entity.h"
#include "../../core/component.h"
#include "../../utils/spatialMap.h"
#include "../../math/polygon.h"

namespace hg::graphics::components {

    enum class TileType {
        Color,
        Texture,
    };

    const std::vector<std::string> TILE_TYPE_NAMES = {
            "Color",
            "Texture",
    };

    struct Tile {
        TileType type;
        hg::Vec2i index;
        Color color;
        std::string texture;
    };

    class Tilemap : public hg::Component {
    public:

        hg::Vec2 tileSize = hg::Vec2::Identity();
        bool collide = true;
        bool dynamic = false;
        hg::utils::SpatialMap2D<Tile, int> tiles;

        void load(utils::Config* config, std::string section) override;
        void save(utils::Config* config, std::string section) override;

        bool isBaked() const { return m_isBaked; }
        void bake();
        std::vector<hg::math::Polygon> geometry() const { return m_geometry; }

        hg::Vec2i getIndex(hg::Vec2 pos);
        hg::Vec2 getPos(hg::Vec2i index);

    protected:

        OBJECT_NAME(Tilemap)

    private:

        std::vector<hg::math::Polygon> m_geometry;
        bool m_isBaked;

    };

    HG_COMPONENT(Graphics, Tilemap)
    HG_FIELD(Tilemap, hg::Vec2, tileSize)
    HG_FIELD(Tilemap, bool, collide)
    HG_FIELD(Tilemap, bool, dynamic)
}

#endif //HAGAME2_TILEMAP_COMPONENT_H
