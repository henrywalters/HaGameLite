//
// Created by henry on 4/21/23.
//

#ifndef HAGAME2_TILEMAP_H
#define HAGAME2_TILEMAP_H

#include <optional>

#include "color.h"
#include "texture.h"
#include "shaderProgram.h"
#include "mesh.h"
#include "primitives/quad.h"

#include "../core/component.h"
#include "../math/aliases.h"
#include "../math/polygon.h"
#include "../math/collisions.h"
#include "../utils/spatialMap.h"
#include "../utils/store.h"
#include "../utils/config.h"

namespace hg::graphics {

    enum class TileMode {
        Color,
        Sprite,
        SpawnPoint
    };

    const std::vector<std::string> TILE_MODE_NAMES = {
        "Color",
        "Sprite",
        "SpawnPoint",
    };

    class Tile {
    public:

        TileMode mode;
        Color color;
        std::string texture;

    };

    class Tilemap {
    public:

        Tilemap(Vec2 tileSize);

        HG_GET(Vec2, tileSize);
        HG_GET(Vec2i, min);
        HG_GET(Vec2i, max);
        HG_GET_SET_SIMPLE(int, zIndex);

        size_t layers() const { return m_layers.size(); }
        utils::SpatialMap2D<Tile, int>* getLayer(int layerIdx);

        size_t tileCount(int layer) const {
            return m_layers[layer].size();
        }

        utils::Store<std::shared_ptr<Texture>>* textures() {
            return &m_textures;
        }

        hg::Vec2i getIndex(hg::Vec2 pos);
        hg::Vec2 getPos(hg::Vec2i idx);

        void clear();

        void addTexture(std::string name, std::string path);
        void addLayer();

        void setVisibility(int layer, bool visible);
        bool getVisibility(int layer);

        void addTile(int layer, hg::Vec2i idx, Tile tile);
        void removeTile(int layer, hg::Vec2i idx);

        utils::MultiConfig save();
        void load(utils::MultiConfig config);

        void render(TileMode mode, ShaderProgram* shader, std::optional<Texture*> texture = std::nullopt);

        hg::Vec2i getSpawnPoint();

        std::vector<math::polygon_t> decompose(int layer);


        Vec3 resolveCollisions(int layer, Rect rect, Vec3 vel, float dt);

    private:

        utils::Store<std::shared_ptr<Texture>> m_textures;
        std::vector<utils::SpatialMap2D<Tile, int>> m_layers;
        std::vector<bool> m_layerVisible;
        Vec2 m_tileSize;

        bool m_hasBounds = false;
        Vec2i m_min, m_max;

        std::unique_ptr<primitives::Quad> m_quad;
        std::unique_ptr<MeshInstance> m_mesh;

        int m_zIndex = 0;

    };
}

#endif //HAGAME2_TILEMAP_H