//
// Created by henry on 1/11/23.
//

#ifndef HAGAME2_FRAMEBUFFER_H
#define HAGAME2_FRAMEBUFFER_H

#include "../math/aliases.h"
#include "texture.h"

namespace hg::graphics {
    class FrameBuffer {
    public:

        FrameBuffer() {}
        ~FrameBuffer();

        void initialize();

        void initializeRenderBufferObject(hg::Vec2i size);

        void bind();

        void unbind();

        bool isComplete();

        void attachTexture(Texture* texture);

        template <GLuint TextureType>
        void attachRawTexture(RawTexture<TextureType>* texture);

        template <GLuint TextureType>
        void attachRawTexture(RawTexture<TextureType>* texture, int attachmentIdx);

    private:
        unsigned int m_id, m_rbo;
    };
}

hg::graphics::FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_id);
}

void hg::graphics::FrameBuffer::initialize() {
    glGenFramebuffers(1, &m_id);
}

void hg::graphics::FrameBuffer::initializeRenderBufferObject(hg::Vec2i size) {
    glGenRenderbuffers(1, &m_rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size[0], size[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
}

void hg::graphics::FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void hg::graphics::FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool hg::graphics::FrameBuffer::isComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void hg::graphics::FrameBuffer::attachTexture(hg::graphics::Texture *texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
}

template<GLuint TextureType>
void hg::graphics::FrameBuffer::attachRawTexture(hg::graphics::RawTexture<TextureType> *texture, int attachmentIdx) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIdx, GL_TEXTURE_2D, texture->id, 0);
}

template<GLuint TextureType>
void hg::graphics::FrameBuffer::attachRawTexture(hg::graphics::RawTexture<TextureType> *texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
}




#endif //HAGAME2_FRAMEBUFFER_H
