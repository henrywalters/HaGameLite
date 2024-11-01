//
// Created by henry on 1/18/23.
//
#include "../../include/hagame/graphics/texture.h"

hg::graphics::Texture::Texture(std::string _path, hg::graphics::ImageType _type):
        image(std::make_unique<Image>(_path, _type)) {
    initialize(image->size, image->data);

    setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenerateMipmap(GL_TEXTURE_2D);
}

hg::graphics::Texture::Texture(hg::Vec2i size):
    image(std::make_unique<Image>()),
    RawTexture<GL_RGBA8, GL_UNSIGNED_BYTE>(size) {

}

