//
// Created by henry on 1/18/23.
//

#ifndef HAGAME2_RAWTEXTURE_H
#define HAGAME2_RAWTEXTURE_H

#include "glfw.h"
#include "../math/aliases.h"

namespace hg::graphics {
    // The RawTexture class provides a thin wrapper around OpenGL textures.
    template <GLuint TextureType, GLuint DataType = GL_FLOAT, GLuint DataFormat = GL_RGBA>
    class RawTexture {
    public:

        GLuint id;

        RawTexture(hg::Vec2i size) {
            initialize(size);
        }

        RawTexture(hg::Vec2i size, float* data) {
            initialize(size, data);
        }

        RawTexture(hg::Vec2i size, unsigned char* data) {
            initialize(size, data);
        }

        void setParameter(GLuint param, GLuint value) {
            glTexParameteri(GL_TEXTURE_2D, param, value);
        }

        void bind() {
            glBindTexture(GL_TEXTURE_2D, id);
        }

    protected:

        // Protected so only extending classes are able provide custom initialization
        RawTexture() {}

        template <class T>
        void initialize(hg::Vec2i size, T* data) {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, TextureType, size[0], size[1], 0, DataFormat, DataType, data);
            setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        void initialize(hg::Vec2i size) {
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, TextureType, size[0], size[1], 0, DataFormat, DataType, NULL);
            setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    };
}

#endif //HAGAME2_RAWTEXTURE_H