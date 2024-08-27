
//
// Created by henry on 6/15/24.
//

#ifndef HAGAME2_TEXTUREATLAS_H
#define HAGAME2_TEXTUREATLAS_H

#include "../math/aliases.h"

namespace hg::graphics {

    using TextureAtlasGroup = std::vector<hg::Vec2i>;

    struct TextureAtlas {
        hg::Vec2i size = hg::Vec2i::Identity();
        TextureAtlasGroup identity();

        Rect getRect(hg::Vec2i index, hg::Vec2i imageSize) const {
            Vec2 cellSize = imageSize.cast<float>().div(size.cast<float>());
            Vec2 cellPos = cellSize.prod(index.cast<float>()).div(imageSize.cast<float>());
            return Rect(cellPos, cellSize.div(imageSize.cast<float>()));
        }

        Vec2 getCellSize(hg::Vec2i imageSize, bool normalize = false) const {
            auto cellSize = imageSize.div(size).cast<float>();
            if (normalize) {
                auto max = std::max(cellSize[0], cellSize[1]);
                cellSize.update([max](int i, float value) {
                    return value / max;
                });
            }
            return cellSize;
        }
    };
}

#endif //HAGAME2_TEXTUREATLAS_H
