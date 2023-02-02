//
// Created by henry on 1/18/23.
//

#ifndef HAGAME2_IMAGE_H
#define HAGAME2_IMAGE_H

#include <string>
#include "../math/aliases.h"
#include "color.h"

namespace hg::graphics {
    enum ImageType {
        RGBA,
        RGB,
        GA,
        G
    };

    class Image {
    private:

        int getStbiEnum(ImageType type);

    public:
        std::string path;
        hg::Vec2i size;
        int channels;
        unsigned char* data;
        ImageType type;

        Image(): path(""), type(ImageType::RGBA) {}

        Image(std::string _path, ImageType _type);

        ~Image();

        Color getColor(hg::Vec2i idx);
    };
}

#endif //HAGAME2_IMAGE_H
