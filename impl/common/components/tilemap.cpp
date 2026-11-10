//
// Created by henry on 8/29/26.
//
#include <hagame/common/components/tilemap.h>
#include <hagame/core/assets.h>
#include <hagame/graphics/spriteSheetV2.h>

#include <cmath>
#include <map>
#include <numeric>
#include <set>

using namespace hg;

// The shapes the tile's sprite sheet gives its cell, or nullopt when it fills the whole cell
static std::optional<std::vector<std::vector<Vec2>>> cellShapes(components::Tile const& tile) {
    auto sheetTile = std::get_if<components::SpriteSheetTile>(&tile.details);
    if (!sheetTile || !hasSpriteSheet(sheetTile->sheet)) {
        return std::nullopt;
    }
    return getSpriteSheet(sheetTile->sheet)->getCellColliders(sheetTile->index);
}

// The most sub-cells per tile side that collider rects get traced at
static constexpr int MAX_COLLIDER_RESOLUTION = 16;

// The smallest number of sub-cells per tile side that the shape's corners land on, when it is an axis aligned rect
static std::optional<int> rectResolution(std::vector<Vec2> const& shape) {
    if (shape.size() != 4) {
        return std::nullopt;
    }
    std::set<float> xs, ys;
    for (const auto& point : shape) {
        xs.insert(point[0]);
        ys.insert(point[1]);
    }
    if (xs.size() != 2 || ys.size() != 2) {
        return std::nullopt;
    }
    for (int resolution = 1; resolution <= MAX_COLLIDER_RESOLUTION; resolution++) {
        bool onGrid = true;
        for (const auto& point : shape) {
            for (int i = 0; i < 2; i++) {
                float scaled = point[i] * resolution;
                onGrid = onGrid && std::abs(scaled - std::round(scaled)) < 1e-4f;
            }
        }
        if (onGrid) {
            return resolution;
        }
    }
    return std::nullopt;
}

// The sub-cells per tile side that the tilemap's collider rects are traced at: one that every rect's own resolution
// divides, as long as that stays within MAX_COLLIDER_RESOLUTION
static int colliderResolution(components::Tilemap const& tilemap) {
    int resolution = 1;
    for (const auto& layer : tilemap.layers) {
        if (!layer.tile.collider) {
            continue;
        }
        auto shapes = cellShapes(layer.tile);
        if (!shapes.has_value()) {
            continue;
        }
        for (const auto& shape : shapes.value()) {
            auto rectRes = rectResolution(shape);
            if (rectRes.has_value() && std::lcm(resolution, rectRes.value()) <= MAX_COLLIDER_RESOLUTION) {
                resolution = std::lcm(resolution, rectRes.value());
            }
        }
    }
    return resolution;
}

// The shape's span in sub-cells, as [min, max), when it is an axis aligned rect on the resolution's grid
static std::optional<std::pair<Vec2i, Vec2i>> gridRect(std::vector<Vec2> const& shape, int resolution) {
    auto rectRes = rectResolution(shape);
    if (!rectRes.has_value() || resolution % rectRes.value() != 0) {
        return std::nullopt;
    }
    Vec2i min(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    Vec2i max(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
    for (const auto& point : shape) {
        for (int i = 0; i < 2; i++) {
            int scaled = std::round(point[i] * resolution);
            min[i] = std::min(min[i], scaled);
            max[i] = std::max(max[i], scaled);
        }
    }
    return std::make_pair(min, max);
}

std::vector<std::vector<Vec2>> components::Tilemap::getCollisionPolygons() const {
    std::vector<std::vector<Vec2>> out;
    int resolution = colliderResolution(*this);

    for (const auto& layer : layers) {
        if (!layer.tile.collider) {
            continue;
        }
        auto shapes = cellShapes(layer.tile);
        if (!shapes.has_value()) {
            continue;
        }
        layer.map.forEach([&](Vec2i const& idx) {
            for (const auto& shape : shapes.value()) {
                // Rects go into the outlines instead
                if (shape.size() < 3 || gridRect(shape, resolution).has_value()) {
                    continue;
                }
                std::vector<Vec2> polygon;
                for (const auto& point : shape) {
                    polygon.push_back((idx.cast<float>() + point).prod(tileSize));
                }
                out.push_back(polygon);
            }
        });
    }

    return out;
}

std::vector<std::vector<Vec2>> components::Tilemap::getCollisionOutlines() const {
    int resolution = colliderResolution(*this);

    // Traced on a grid of sub-cells, resolution to a tile side, so half tile colliders join up with their neighbours
    hg::utils::SpatialHash2D<int> solid;

    for (const auto& layer : layers) {
        if (!layer.tile.collider) {
            continue;
        }
        auto shapes = cellShapes(layer.tile);
        layer.map.forEach([&](Vec2i const& idx) {
            Vec2i base = idx * resolution;
            if (!shapes.has_value()) {
                for (int y = 0; y < resolution; y++) {
                    for (int x = 0; x < resolution; x++) {
                        solid.add(base + Vec2i(x, y));
                    }
                }
                return;
            }
            for (const auto& shape : shapes.value()) {
                auto rect = gridRect(shape, resolution);
                if (!rect.has_value()) {
                    continue;
                }
                for (int y = rect->first.y(); y < rect->second.y(); y++) {
                    for (int x = rect->first.x(); x < rect->second.x(); x++) {
                        solid.add(base + Vec2i(x, y));
                    }
                }
            }
        });
    }

    if (solid.empty()) return {};

    // Every cell side facing an empty cell, directed so the solid cell is on its left. Walking these edges traces
    // outer boundaries counter-clockwise and holes clockwise, the winding Box2D's one-sided chains want
    struct Edge {
        Vec2i start;
        Vec2i dir;
    };
    std::vector<Edge> edges;
    std::map<std::pair<int, int>, std::vector<int>> edgesFrom;

    auto addEdge = [&](Vec2i start, Vec2i dir) {
        edgesFrom[{start.x(), start.y()}].push_back(edges.size());
        edges.push_back(Edge{start, dir});
    };

    solid.forEach([&](Vec2i const& idx) {
        int x = idx.x(), y = idx.y();
        if (!solid.has(Vec2i(x, y - 1))) addEdge(Vec2i(x, y), Vec2i(1, 0));
        if (!solid.has(Vec2i(x + 1, y))) addEdge(Vec2i(x + 1, y), Vec2i(0, 1));
        if (!solid.has(Vec2i(x, y + 1))) addEdge(Vec2i(x + 1, y + 1), Vec2i(-1, 0));
        if (!solid.has(Vec2i(x - 1, y))) addEdge(Vec2i(x, y + 1), Vec2i(0, -1));
    });

    std::vector<bool> used(edges.size(), false);
    std::vector<std::vector<Vec2>> out;

    for (int first = 0; first < edges.size(); first++) {
        if (used[first]) continue;

        std::vector<Vec2i> loop;
        int current = first;
        while (!used[current]) {
            used[current] = true;
            auto edge = edges[current];
            loop.push_back(edge.start);

            Vec2i end = edge.start + edge.dir;
            Vec2i left(-edge.dir.y(), edge.dir.x());
            Vec2i right(edge.dir.y(), -edge.dir.x());

            // Two cells touching only at a corner leave two ways on; turning left keeps each loop from crossing itself
            int next = -1;
            for (auto dir : {left, edge.dir, right}) {
                for (int candidate : edgesFrom[{end.x(), end.y()}]) {
                    if (!used[candidate] && edges[candidate].dir == dir) {
                        next = candidate;
                        break;
                    }
                }
                if (next != -1) break;
            }
            if (next == -1) break;
            current = next;
        }

        // Drop the points in the middle of straight runs
        std::vector<Vec2> corners;
        for (int i = 0; i < loop.size(); i++) {
            Vec2i prev = loop[(i + loop.size() - 1) % loop.size()];
            Vec2i next = loop[(i + 1) % loop.size()];
            Vec2i a = loop[i] - prev;
            Vec2i b = next - loop[i];
            if (a.x() * b.y() - a.y() * b.x() != 0) {
                corners.push_back((loop[i].cast<float>() / resolution).prod(tileSize));
            }
        }

        out.push_back(corners);
    }

    return out;
}
