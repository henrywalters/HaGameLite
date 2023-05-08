//
// Created by henry on 1/18/23.
//
#include "../../include/hagame/graphics/texture.h"

hg::graphics::Texture::Texture(std::string _path, hg::graphics::ImageType _type):
        image(std::make_unique<Image>(_path, _type)) {
    initialize(image->size, image->data);

    setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);
}

hg::graphics::Texture::Texture(hg::Vec2i size):
    image(std::make_unique<Image>()),
    RawTexture<GL_RGBA8, GL_UNSIGNED_BYTE>(size)
{

}