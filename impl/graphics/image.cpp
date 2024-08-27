//
// Created by henry on 1/18/23.
//
#include <iostream>
#include "../../include/hagame/graphics/image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../thirdparty/stb/stb_image.h"
#include "../../include/hagame/ui/elements/image.h"


hg::graphics::Image::Image(std::string _path, hg::graphics::ImageType _type): path(_path), type(_type) {
    data = stbi_load(path.c_str(), &size[0], &size[1], &channels, getStbiEnum(type));

    if (data == nullptr) {
        throw std::runtime_error("Failed to load texture: " + path + " Check console for more details.");
    }
}

hg::graphics::Image::~Image() {
    if (size.magnitude() > 0) {
        stbi_image_free(data);
    }
}

int hg::graphics::Image::getStbiEnum(hg::graphics::ImageType type) {
    switch (type) {
        case ImageType::RGBA:
            return STBI_rgb_alpha;
        case ImageType::RGB:
            return STBI_rgb;
        case ImageType::G:
            return STBI_grey;
        case ImageType::GA:
            return STBI_grey_alpha;
        default:
            throw std::runtime_error("Unknown image type");
    }
}

hg::graphics::Color hg::graphics::Image::getColor(hg::Vec2i idx) {
    unsigned bytePerPixel = channels;
    unsigned char* p = data + ((idx[0] + size[0] * idx[1]) * bytePerPixel);
    int r = static_cast<int>(p[0]);
    int g = static_cast<int>(p[1]);
    int b = static_cast<int>(p[2]);

    if (channels == 3) {
        return Color(r, g, b);
    }
    else {
        return Color(r, g, b, static_cast<int>(p[3]));
    }
}