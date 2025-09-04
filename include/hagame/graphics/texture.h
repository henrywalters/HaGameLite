//
// Created by henry on 1/18/23.
//

#ifndef HAGAME2_TEXTURE_H
#define HAGAME2_TEXTURE_H

#include <memory>

#include "../utils/watcher.h"
#include "rawTexture.h"
#include "image.h"
#include "hagame/utils/macros.h"

namespace hg::graphics {
    class Texture : public RawTexture<GL_RGBA8, GL_UNSIGNED_BYTE> {
    public:
        std::unique_ptr<Image> image;

        Texture(std::string _path, ImageType _type = ImageType::RGBA);
        Texture(hg::Vec2i size);
    };

    // A simple type
    class TextureAsset {
    public:

        TextureAsset(std::string path = "");

        bool operator==(const TextureAsset& other) { return m_texture == other.m_texture; }
        bool operator!=(const TextureAsset& other) { return !(*this == other); }

        HG_GET_PTR(hg::graphics::Texture, texture)
        HG_GET_SET(std::string, path, reload)

    private:

        void reload();

        std::string m_path;
        hg::graphics::Texture* m_texture = nullptr;

    };
}

#endif //HAGAME2_TEXTURE_H
