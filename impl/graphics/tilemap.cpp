//
// Created by henry on 4/22/23.
//
#include "../../include/hagame/graphics/tilemap.h"
#include "../../include/hagame/math/functions.h"
#include "../../include/hagame/graphics/debug.h"

hg::graphics::Tilemap::Tilemap(hg::Vec2 tileSize):
        m_tileSize(tileSize)
{
    m_quad = std::make_unique<primitives::Quad>(m_tileSize, m_tileSize * 0.5);
    m_mesh = std::make_unique<MeshInstance>(m_quad.get());
}

void hg::graphics::Tilemap::addTexture(std::string name, std::string path) {
    m_textures.set(name, std::make_shared<Texture>(path));
}

void hg::graphics::Tilemap::addLayer() {
    m_layers.push_back(hg::utils::SpatialMap2D<Tile, int>());
    m_layerVisible.push_back(true);
}

void hg::graphics::Tilemap::addTile(int layer, hg::Vec2i idx, hg::graphics::Tile tile) {
    if (m_layers[layer].has(idx)) {
        return;
    }

    m_layers[layer].insert(idx, tile);

    if (!m_hasBounds) {
        m_hasBounds = true;
        m_min = idx;
        m_max = idx;
    } else {
        for (int i = 0; i < 2; i++) {
            if (idx[i] < m_min[i]) {
                m_min[i] = idx[i];
            }

            if (idx[i] > m_max[i]) {
                m_max[i] = idx[i];
            }
        }
    }
}

void hg::graphics::Tilemap::removeTile(int layer, hg::Vec2i idx) {
    m_layers[layer].clear(idx);
}

hg::utils::MultiConfig hg::graphics::Tilemap::save() {
    hg::utils::MultiConfig config;

    config.addPage("project");
    auto project = config.getPage("project");

    project->addSection("meta");
    project->set("meta", "layer_count", m_layers.size());

    config.addPage("textures");
    auto textures = config.getPage("textures");

    m_textures.forEach([&](auto key, auto texture) {
        textures->addSection(key);
        textures->setRaw(key, "path", texture->image->path);
    });

    config.addPage("tiles");
    auto tiles = config.getPage("tiles");

    int tileIndex = 0;

    for (int i = 0; i < m_layers.size(); i++) {
        m_layers[i].forEach([&](hg::Vec2i idx, Tile tile) {
            std::string id = std::to_string(tileIndex++);
            tiles->set(id, "layer", i);
            tiles->set(id, "mode", (int) tile.mode);
            tiles->setArray<int, 2>(id, "index", idx.vector);
            tiles->setArray<float, 4>(id, "color", tile.color.vector);

            if (tile.mode == TileMode::Sprite) {
                tiles->setRaw(id, "texture", tile.texture);
            }
        });
    }

    return config;
}

void hg::graphics::Tilemap::load(hg::utils::MultiConfig config) {
    auto project = config.getPage("project");
    auto textures = config.getPage("textures");
    auto tiles = config.getPage("tiles");

    int layerCount = project->get<int>("meta", "layer_count");

    for (int i = 0; i < layerCount; i++) {
        addLayer();
    }

    for (const auto& texture : textures->sections()) {
        m_textures.set(texture, std::make_shared<Texture>(textures->getRaw(texture, "path")));
    }

    for (const auto& id : tiles->sections()) {

        Tile tile;
        tiles->getArray<float, 4>(id, "color", tile.color.vector);
        tile.mode = (TileMode) tiles->get<int>(id, "mode");
        if (tile.mode == TileMode::Sprite) {
            tile.texture = tiles->getRaw(id, "texture");
        }

        int layer = tiles->get<int>(id, "layer");
        hg::Vec2i idx;
        tiles->getArray<int, 2>(id, "index", idx.vector);

        addTile(layer, idx, tile);
    }
}

void hg::graphics::Tilemap::render(TileMode mode, ShaderProgram* shader, std::optional<Texture*> texture) {
    for (int i = 0; i < m_layers.size(); i++) {

        if (!m_layerVisible[i]) {
            continue;
        }

        m_layers[i].forEach([&](hg::Vec2i idx, Tile tile) {
            if (tile.mode == mode) {
                auto pos = getPos(idx).resize<3>();
                pos[2] = m_zIndex + i;
                shader->setMat4("model", Mat4::Translation(pos));
                shader->setVec4("color", tile.color);
                if (mode == TileMode::Sprite) {
                    m_textures.get(tile.texture)->bind();
                } else if (texture.has_value()) {
                    texture.value()->bind();
                }
                m_mesh->render();
            }
        });
    }
}

hg::Vec2i hg::graphics::Tilemap::getIndex(hg::Vec2 pos) {
    return pos.div(m_tileSize).floor().cast<int>();
}

hg::Vec2 hg::graphics::Tilemap::getPos(hg::Vec2i idx) {
    return idx.cast<float>().prod(m_tileSize);
}

void hg::graphics::Tilemap::setVisibility(int layer, bool visible) {
    m_layerVisible[layer] = visible;
}

bool hg::graphics::Tilemap::getVisibility(int layer) {
    return m_layerVisible[layer];
}

void hg::graphics::Tilemap::clear() {
    m_layers.clear();
    m_layerVisible.clear();
    m_textures.clear();

    m_hasBounds = false;
    m_min = Vec2i::Zero();
    m_max = Vec2i::Zero();
}

std::vector<hg::math::polygon_t> hg::graphics::Tilemap::decompose(int layer) {
    std::vector<hg::math::polygon_t> polygons;

    std::vector<std::vector<hg::Vec2i>> islands;

    return polygons;
}

hg::Vec2i hg::graphics::Tilemap::getSpawnPoint() {

    bool hasSpawnPoint = false;
    hg::Vec2i spawnPoint;

    for (int i = 0; i < m_layers.size(); i++) {
        m_layers[i].forEach([&](auto idx, auto tile) {
            if (tile.mode == TileMode::SpawnPoint) {
                spawnPoint = idx;
                hasSpawnPoint = true;
            }
        });
    }

    if (hasSpawnPoint) {
        return spawnPoint;
    }

    throw std::runtime_error("Tilemap does not have a spawn point");
}

hg::utils::SpatialMap2D<hg::graphics::Tile, int> *hg::graphics::Tilemap::getLayer(int layerIdx) {
    return &m_layers[layerIdx];
}

hg::Vec3 hg::graphics::Tilemap::resolveCollisions(int layer, hg::Rect rect, hg::Vec3 vel, double dt) {

    m_layers[layer].forEach([&](Vec2i idx, Tile tile) {
        float t;
        Rect tileRect(getPos(idx) - rect.size * 0.5, m_tileSize + rect.size);

        auto hit = hg::math::collisions::checkRayAgainstRect(
                hg::math::Ray(rect.getCenter().resize<3>(), vel * dt),
                tileRect,
                t
        );

        if (hit.has_value() && t < 1.0f) {
            vel += hit.value().normal.prod(vel.abs()) * (1 - t);
        }
    });

    return vel ;
}

std::optional<hg::math::collisions::Hit>  hg::graphics::Tilemap::isColliding(int layer, hg::Vec2i tileIdx, hg::Rect rect, hg::Vec3 vel, double dt)
{

    if (!m_layers[layer].has(tileIdx) || m_layers[layer].isEmpty(m_layers[layer].get(tileIdx))) {
        return std::nullopt;
    }

    float t;
    Rect tileRect(getPos(tileIdx) - rect.size * 0.5, m_tileSize + rect.size);

    auto hit = hg::math::collisions::checkRayAgainstRect(
            hg::math::Ray(rect.getCenter().resize<3>(), vel * dt),
            tileRect,
            t
    );

    if (hit.has_value() && t < 1.0f) {
        return hit;
    }

    return std::nullopt;
}

std::optional<hg::math::collisions::Hit>  hg::graphics::Tilemap::isColliding(int layer, hg::Rect rect, hg::Vec3 vel, double dt) {

    bool colliding = false;

    std::optional<hg::math::collisions::Hit> closestHit;

    auto velSize = Vec2(vel.magnitude());

    Rect velRect(rect.pos + velSize * -0.5, rect.size + velSize);

    hg::Vec2i minIdx = getIndex(velRect.min());
    hg::Vec2i maxIdx = getIndex(velRect.max());

    for (int i = minIdx[0]; i <= maxIdx[0]; i++) {
        if (colliding) {
            continue;
        }
        for (int j = minIdx[1]; j <= maxIdx[1]; j++) {

            if (colliding) {
                continue;
            }

            auto hit = isColliding(layer, hg::Vec2i(i, j), rect, vel, dt);

            if (hit.has_value()) {
                colliding = true;
                closestHit = hit;
            }
        }
    }

    return closestHit;
}
