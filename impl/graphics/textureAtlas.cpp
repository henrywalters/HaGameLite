//
// Created by henry on 6/15/24.
//
#include "../../../include/hagame/graphics/textureAtlas.h"

hg::graphics::TextureAtlasGroup hg::graphics::TextureAtlas::identity() {
    TextureAtlasGroup group;

    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            group.emplace_back(i, j);
        }
    }

    return group;
}
