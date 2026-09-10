//
// Created by henry on 1/18/23.
//

#ifndef HAGAME2_TEXTURE_H
#define HAGAME2_TEXTURE_H

#include <memory>

#include "rawTexture.h"
#include "image.h"
#include "../core/asset.h"

namespace hg::graphics {
    class Texture : public RawTexture<GL_RGBA8, GL_UNSIGNED_BYTE> {
    public:
        std::unique_ptr<Image> image;

        Texture(std::string _path, ImageType _type = ImageType::RGBA);
        Texture(hg::Vec2i size);
    };

    // A simple type
    class TextureAsset : public Asset<Texture> {
    protected:

        void reload() override;

    };
}

#endif //HAGAME2_TEXTURE_H
