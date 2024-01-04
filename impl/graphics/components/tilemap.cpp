//
// Created by henry on 12/29/23.
//
#include <deque>
#include "../../../include/hagame/graphics/components/tilemap.h"

using namespace hg::graphics;
using namespace hg::math;

void hg::graphics::components::Tilemap::load(hg::utils::Config *config, std::string section) {
    tiles.clear();
    std::string serialized = config->getRaw(section, "tiles");
    std::vector<std::string> serializedTiles = hg::utils::s_partition(serialized, '{', '}');
    for (const auto& serializedTile : serializedTiles) {
        std::vector<std::string> parts = hg::utils::s_split(serializedTile, ';');
        auto indexParts = hg::utils::s_split(parts[1].substr(1, parts[1].size() - 2), ',');
        auto colorParts = hg::utils::s_split(parts[2].substr(1, parts[2].size() - 2), ',');
        Tile tile;
        tile.type = (TileType) std::stoi(parts[0]);

        for (int i = 0; i < 2; i++) {
            tile.index[i] = std::stoi(indexParts[i]);
        }

        for (int i = 0; i < 4; i++) {
            tile.color[i] = std::stof(colorParts[i]);
        }

        tile.texture = parts[3];

        tiles.set(tile.index, tile);
    }
}

void hg::graphics::components::Tilemap::save(hg::utils::Config *config, std::string section) {
    std::string serialized;
    tiles.forEach([&](hg::Vec2i index, Tile tile) {
        std::string serializedTile = "{" + std::to_string((int) tile.type) + ";" +
                                     tile.index.toString() + ";" +
                                     tile.color.toString() + ";" +
                                     tile.texture + "}";
        serialized += serializedTile;
    });
    config->setRaw(section, "tiles", serialized);
}

void hg::graphics::components::Tilemap::bake() {
    m_geometry.clear();

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
                    current[0] >= tiles.min()[0] && current[0] <= tiles.max()[0] && current[1] >= tiles.min()[1] && current[1] <= tiles.max()[1] &&
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
                    if (!tiles.has(neighbors[i])) {
                        hg::Vec2 aOffset, bOffset;

                        if (i % 2 == 0) {
                            aOffset = Vec2(tileSize[0] * 0.5 * (i == 0 ? -1 : 1), tileSize[1] * 0.5 * (i == 0 ? 1 : -1));
                            bOffset = Vec2(tileSize[0] * 0.5 * (i == 0 ? -1 : 1), tileSize[1] * 0.5 * (i == 0 ? -1 : 1));
                        } else {
                            aOffset = Vec2(tileSize[0] * 0.5 * (i == 0 ? 1 : -1), tileSize[1] * 0.5 * (i == 1 ? 1 : -1));
                            bOffset = Vec2(tileSize[0] * 0.5 * (i == 0 ? -1 : 1), tileSize[1] * 0.5 * (i == 1 ? 1 : -1));
                        }

                        Vec2 a = getPos(current) + aOffset + tileSize * 0.5;
                        Vec2 b = getPos(current) + bOffset + tileSize * 0.5;

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

    for (int y = tiles.min()[1]; y <= tiles.max()[1]; y++) {
        for (int x = tiles.min()[0]; x <= tiles.max()[0]; x++) {
            hg::Vec2i index(x, y);
            if (tiles.has(index) && !visited.has(index)) {
                m_geometry.push_back(floodFill(x, y));
            }
        }
    }

    m_isBaked = true;
}

hg::Vec2i components::Tilemap::getIndex(hg::Vec2 pos) {
    return pos.div(tileSize).floor().cast<int>();
}

hg::Vec2 components::Tilemap::getPos(hg::Vec2i index) {
    return index.cast<float>().prod(tileSize);
}
