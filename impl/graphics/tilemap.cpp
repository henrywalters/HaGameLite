//
// Created by henry on 4/22/23.
//
#include "../../include/hagame/graphics/tilemap.h"
#include "../../include/hagame/math/functions.h"
#include "../../include/hagame/graphics/debug.h"

using namespace hg::math;
using namespace hg::graphics;

Tilemap::Tilemap(hg::Vec2 tileSize):
        m_tileSize(tileSize)
{
    m_quad = std::make_unique<primitives::Quad>(m_tileSize, m_tileSize * 0.5);
    m_mesh = std::make_unique<MeshInstance>(m_quad.get());
}

void Tilemap::addTexture(std::string name, std::string path) {
    m_textures.set(name, std::make_shared<Texture>(path));
}

void Tilemap::addLayer() {
    m_layers.push_back(hg::utils::SpatialMap2D<Tile, int>());
    m_layerVisible.push_back(true);
}

void Tilemap::addTile(int layer, hg::Vec2i idx, Tile tile) {
    if (m_layers[layer].has(idx)) {
        return;
    }

    m_layers[layer].set(idx, tile);

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

void Tilemap::removeTile(int layer, hg::Vec2i idx) {
    m_layers[layer].remove(idx);
}

hg::utils::MultiConfig Tilemap::save() {
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

void Tilemap::load(hg::utils::MultiConfig config) {
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

void Tilemap::render(TileMode mode, ShaderProgram* shader, std::optional<Texture*> texture) {
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

hg::Vec2i Tilemap::getIndex(hg::Vec2 pos) {
    return pos.div(m_tileSize).floor().cast<int>();
}

hg::Vec2 Tilemap::getPos(hg::Vec2i idx) {
    return idx.cast<float>().prod(m_tileSize);
}

void Tilemap::setVisibility(int layer, bool visible) {
    m_layerVisible[layer] = visible;
}

bool Tilemap::getVisibility(int layer) {
    return m_layerVisible[layer];
}

void Tilemap::clear() {
    m_layers.clear();
    m_layerVisible.clear();
    m_textures.clear();

    m_hasBounds = false;
    m_min = Vec2i::Zero();
    m_max = Vec2i::Zero();
}

std::vector<hg::math::Polygon> Tilemap::decompose(int layer)  {
    std::vector<Polygon> polygons;

    hg::Vec2i size = m_max - m_min;

    //std::vector<std::vector<bool>> visited(size[0], std::vector<bool>(size[1], false));

    hg::utils::SpatialMap2D<bool, int> visited;

    auto areCollinear = [&](const hg::Vec2& p1, const Vec2& p2, const Vec2& p3) -> bool {
        return (p1[0] * (p2[1] - p3[1]) + p2[0] * (p3[1] - p1[1]) + p3[0] * (p1[1] - p2[1])) == 0;
    };

    auto floodFill = [&](int startX, int startY) -> Polygon {
        Polygon polygon;
        std::deque<hg::Vec2i> toVisit;
        toVisit.push_back(Vec2i(startX, startY));

        while (!toVisit.empty()) {
            hg::Vec2i current = toVisit.front();
            toVisit.pop_front();

            if (
                    current[0] >= m_min[0] && current[0] <= m_max[0] && current[1] >= m_min[1] && current[1] <= m_max[1] &&
                    m_layers[layer].has(current) &&
                    !visited.has(current)
                    ) {
                visited.set(current, true);

                std::array<Vec2i, 4> neighbors = {
                        Vec2i(current[0] - 1, current[1]),
                        Vec2i(current[0], current[1] + 1),
                        Vec2i(current[0] + 1, current[1]),
                        Vec2i(current[0], current[1] - 1),
                };

                for (int i = 0; i < 4; i++) {
                    if (!m_layers[layer].has(neighbors[i])) {
                        hg::Vec2 aOffset, bOffset;

                        if (i % 2 == 0) {
                            aOffset = Vec2(m_tileSize[0] * 0.5 * (i == 0 ? -1 : 1), m_tileSize[1] * 0.5 * (i == 0 ? 1 : -1));
                            bOffset = Vec2(m_tileSize[0] * 0.5 * (i == 0 ? -1 : 1), m_tileSize[1] * 0.5 * (i == 0 ? -1 : 1));
                        } else {
                            aOffset = Vec2(m_tileSize[0] * 0.5 * (i == 0 ? 1 : -1), m_tileSize[1] * 0.5 * (i == 1 ? 1 : -1));
                            bOffset = Vec2(m_tileSize[0] * 0.5 * (i == 0 ? -1 : 1), m_tileSize[1] * 0.5 * (i == 1 ? 1 : -1));
                        }

                        Vec2 a = getPos(current) + aOffset + m_tileSize * 0.5;
                        Vec2 b = getPos(current) + bOffset + m_tileSize * 0.5;

                        bool inPolygon = false;

                        for (auto& edge : polygon) {

                            if ((edge.a == a && edge.b == b) || (edge.b == a && edge.a == b)) continue;

                            if (a == edge.a && areCollinear(edge.a, edge.b, b)) {
                                edge.a = b;
                                inPolygon = true;
                                break;
                            }

                            if (b == edge.a && areCollinear(edge.a, edge.b, a)) {
                                edge.a = a;
                                inPolygon = true;
                                break;
                            }

                            if (a == edge.b && areCollinear(edge.a, edge.b, b)) {
                                edge.b = b;
                                inPolygon = true;
                                break;
                            }

                            if (b == edge.b && areCollinear(edge.a, edge.b, a)) {
                                edge.b = a;
                                inPolygon = true;
                                break;
                            }
                        }

                        if (!inPolygon) {
                            polygon.push_back(Edge{a, b});
                        }

                    } else if (!visited.has(neighbors[i])) {
                        toVisit.push_back(neighbors[i]);
                    }
                }
            }
        }

        return polygon;
    };

    for (int y = m_min[1]; y <= m_max[1]; y++) {
        for (int x = m_min[0]; x <= m_max[0]; x++) {
            hg::Vec2i index(x, y);
            if (m_layers[layer].has(index) && !visited.has(index)) {
                polygons.push_back(floodFill(x, y));
            }
        }
    }


    return polygons;
}

hg::Vec2i Tilemap::getSpawnPoint() {

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

hg::utils::SpatialMap2D<Tile, int> *Tilemap::getLayer(int layerIdx) {
    return &m_layers[layerIdx];
}

hg::Vec3 Tilemap::resolveCollisions(int layer, hg::Rect rect, hg::Vec3 vel, double dt) {

    float speed = vel.magnitude() * dt * 2;

    hg::Rect checkRect(rect.pos - Vec2(speed * 0.5), rect.size + Vec2(speed));

    hg::Vec2i min = getIndex(checkRect.min());
    hg::Vec2i max = getIndex(checkRect.max());

    Ray ray(rect.getCenter().resize<3>(), vel * dt);

    float t;

    std::vector<hg::Vec2> positions;

    for (int i = min.x(); i <= max.x(); i++) {
        for (int j = min.y(); j <= max.y(); j++) {
            hg::Vec2i idx(i, j);
            if (getLayer(layer)->has(idx)) {
                positions.push_back(getPos(idx));
            }
        }
    }

    std::sort(positions.begin(), positions.end(), [&](hg::Vec2 a, hg::Vec2 b){
        return (a - ray.origin.resize<2>()).magnitude() < (b - ray.origin.resize<2>()).magnitude();
    });

    for (const auto& position : positions) {
        Rect tileRect(position - rect.size * 0.5, m_tileSize + rect.size);
        auto hit = collisions::checkRayAgainstRect(ray, tileRect, t);
        if (hit.has_value() && t < 1.0f) {
            vel += hit.value().normal.prod(vel.abs()) * (1 - t);
        }
    }

    return vel ;
}

std::optional<collisions::Hit>  Tilemap::isColliding(int layer, hg::Vec2i tileIdx, hg::Rect rect, hg::Vec3 vel, double dt)
{

    if (!m_layers[layer].has(tileIdx)) {
        return std::nullopt;
    }

    float t;
    Rect tileRect(getPos(tileIdx) - rect.size * 0.5, m_tileSize + rect.size);

    auto hit = collisions::checkRayAgainstRect(
            Ray(rect.getCenter().resize<3>(), vel * dt),
            tileRect,
            t
    );

    if (hit.has_value() && t < 1.0f) {
        return hit;
    }

    return std::nullopt;
}

std::optional<collisions::Hit> Tilemap::raycast(int layer, math::Ray ray, float& t, hg::Vec2 padTiles) {
    auto indices = hg::bresenham(getIndex(ray.origin.resize<2>()), getIndex((ray.origin + ray.direction).resize<2>()));

    hg::Vec2 origin = ray.origin.resize<2>();

    std::sort(indices.begin(), indices.end(), [&](auto a, auto b) {
        return (getPos(a) - origin).magnitude() > (getPos(b) - origin).magnitude();
    });

    std::optional<collisions::Hit> closestHit;
    float tmpT;

    t = std::numeric_limits<float>::infinity();

    m_layers[layer].forEach([&](auto index, auto tile) {

        Rect tileRect(getPos(index) - padTiles * 0.5, m_tileSize + padTiles);

        auto hit = collisions::checkRayAgainstRect(ray, tileRect, tmpT);

        if (hit.has_value() && tmpT <= 1.0f) {
            if (!closestHit.has_value() || tmpT < t) {
                t = tmpT;
                closestHit = hit;
            }
        }
    });

    return closestHit;
}

std::optional<collisions::Hit>  Tilemap::isColliding(int layer, hg::Rect rect, hg::Vec3 vel, double dt) {

    bool colliding = false;

    std::optional<collisions::Hit> closestHit;

    auto velSize = Vec2(vel.magnitude());

    Rect velRect(rect.pos + velSize * -0.5, rect.size + velSize);

    hg::Vec2i minIdx = getIndex(velRect.min());
    hg::Vec2i maxIdx = getIndex(velRect.max());

    for (int i = minIdx[0]; i <= maxIdx[0]; i++) {
        if (colliding) {
            break;
        }
        for (int j = minIdx[1]; j <= maxIdx[1]; j++) {

            if (colliding) {
                break;
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

bool Tilemap::isColliding(int layer, hg::Rect rect) {

    hg::Vec2i minIdx = getIndex(rect.min());
    hg::Vec2i maxIdx = getIndex(rect.max());

    for (int i = minIdx[0]; i <= maxIdx[0]; i++) {
        for (int j = minIdx[1]; j <= maxIdx[1]; j++) {
            hg::Vec2i idx = hg::Vec2i(i, j);
            if (!m_layers[layer].has(idx)) {
                continue;
            }

            Rect tileRect(getPos(idx) - m_tileSize * 0.5, m_tileSize);
            if (collisions::checkRectAgainstRect(rect, tileRect)) {
                Debug::DrawRect(tileRect, Color::red());
                return true;
            }
        }
    }

    return false;
}


std::optional<std::vector<hg::Vec2i>> Tilemap::search(int layer, hg::Vec2i startIdx, hg::Vec2i goalIdx) {
    utils::PathFinding pathfinding([&](utils::PathFinding::Node node) {
        return findNeighbors(layer, node);
    });

    auto path = pathfinding.search(startIdx, goalIdx);

    return path;
}

std::vector<hg::utils::PathFinding::Node> Tilemap::findNeighbors(int layer, hg::utils::PathFinding::Node node) {
    std::vector<utils::PathFinding::Node> nodes;

    bool hasNeighbor = false;

    for (const auto& position : getLayer(layer)->getNeighbors(node.position)) {
        if (!getLayer(layer)->has(position)) {
            hasNeighbor = true;
            break;
        }
    }

    for (const auto& position : getLayer(layer)->getNeighbors(node.position, !hasNeighbor)) {

        if (getLayer(layer)->has(position)) {
            continue;
        }

        utils::PathFinding::Node neighbor;
        neighbor.position = position;
        neighbor.cost = 0;

        nodes.push_back(neighbor);
    }

    return nodes;
}

