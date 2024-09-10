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

        ~FrameBuffer() {
            glDeleteFramebuffers(1, &m_id);
        }

        void initialize() {
            glGenFramebuffers(1, &m_id);
        }

        void initializeRenderBufferObject(hg::Vec2i size) {
            glGenRenderbuffers(1, &m_rbo);

            glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
            // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size[0], size[1]);
            // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
        }

        void bind() {
            glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        }

        void unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        bool isComplete() {
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }

        void attachTexture(hg::graphics::Texture *texture) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
        }

        template<GLuint TextureType>
        void attachRawTexture(hg::graphics::RawTexture<TextureType> *texture, int attachmentIdx) {
            texture->bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIdx, GL_TEXTURE_2D, texture->id, 0);
        }

        template<GLuint TextureType>
        void attachRawTexture(hg::graphics::RawTexture<TextureType> *texture) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
        }

        template<GLuint TextureType>
        void attachDepthTexture(hg::graphics::RawTexture<TextureType> *texture) {
            glGenRenderbuffers(1, &m_rboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texture->size[0], texture->size[1]);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);
            texture->bind();
            // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->id, 0);
        }
    private:
        unsigned int m_id, m_rbo, m_rboDepth;
    };
}






#endif //HAGAME2_FRAMEBUFFER_H
