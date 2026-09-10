//
// Created by henry on 8/29/26.
//
#include <hagame/common/components/tilemap.h>

using namespace hg;

std::vector<hg::Recti> components::Tilemap::getCollisionRects() const {
    hg::utils::SpatialHash2D<int> remaining;

    for (const auto& layer : layers) {
        if (layer.tile.collider) {
            layer.map.forEach([&](Vec2i const& idx) {
                remaining.add(idx);
            });
        }
    }

    if (remaining.empty()) return {};

    std::vector<Recti> out;

    Vec2i min = Vec2i(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    Vec2i max = Vec2i(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());

    remaining.forEach([&](Vec2i const& idx) {
        for (int i = 0; i < 2; i++) {
            if (idx[i] < min[i]) min[i] = idx[i];
            if (idx[i] > max[i]) max[i] = idx[i];
        }
    });

    for (int y = min.y(); y <= max.y(); y++) {
        for (int x = min.x(); x <= max.x(); x++) {
            hg::Vec2i start(x, y);
            if (!remaining.has(start)) continue;

            int width = 1;
            while (remaining.has(hg::Vec2i(x + width, y))) {
                width++;
            }

            int height = 1;
            bool canGrow = true;
            while (canGrow) {
                for (int dx = 0; dx < width; dx++) {
                    if (!remaining.has(hg::Vec2i(x + dx, y + height))) {
                        canGrow = false;
                        break;
                    }
                }
                if (canGrow) height++;
            }

            for (int dy = 0; dy < height; dy++) {
                for (int dx = 0; dx < width; dx++) {
                    remaining.remove(Vec2i(x + dx, y + dy));
                }
            }

            out.push_back(Recti{start, Vec2i(width, height)});
        }
    }

    return out;
}
