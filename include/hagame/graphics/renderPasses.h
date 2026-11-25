//
// Created by henry on 1/24/23.
//

#ifndef HAGAME2_RENDERPASSES_H
#define HAGAME2_RENDERPASSES_H

#include <vector>
#include "./frameBuffer.h"

namespace hg::graphics {

    // A texture a render pass draws into, in whatever format suits what's stored in it. Smaller formats are much
    // cheaper to draw into and read back: RGBA32F is 16 bytes a pixel, RGBA16F 8 and RGBA8 4
    struct RenderTarget {
        GLuint id = 0;
        hg::Vec2i size;
        GLenum format;

        RenderTarget(hg::Vec2i _size, GLenum _format):
            size(_size),
            format(_format)
        {
            glGenTextures(1, &id);
            bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            GLenum layout = GL_RGBA;
            GLenum type = GL_UNSIGNED_BYTE;
            switch (format) {
                case GL_RGBA32F: type = GL_FLOAT; break;
                case GL_RGBA16F: type = GL_HALF_FLOAT; break;
                case GL_RG32F: layout = GL_RG; type = GL_FLOAT; break;
                case GL_RG16F: layout = GL_RG; type = GL_HALF_FLOAT; break;
                case GL_R32F: layout = GL_RED; type = GL_FLOAT; break;
                case GL_R16F: layout = GL_RED; type = GL_HALF_FLOAT; break;
                default: break; // GL_RGBA8
            }
            glTexImage2D(GL_TEXTURE_2D, 0, format, size[0], size[1], 0, layout, type, nullptr);
        }

        ~RenderTarget() {
            glDeleteTextures(1, &id);
        }

        RenderTarget(RenderTarget const&) = delete;
        RenderTarget& operator=(RenderTarget const&) = delete;

        void bind() {
            glBindTexture(GL_TEXTURE_2D, id);
        }
    };

    struct RenderPass {
        std::vector<std::unique_ptr<RenderTarget>> textures;
        std::unique_ptr<FrameBuffer> buffer;
        // What it was made with, to make it again at another size
        std::vector<GLenum> formats;
        bool hasDepth = false;
    };

    template <class KeyType>
    class RenderPasses {
    public:

        // A pass drawing into textures (all RGBA32F unless given a format), and a depth buffer if attachDepth
        hg::graphics::RenderPass *create(KeyType key, Vec2i size, int textures = 1, bool attachDepth = false,
                                         GLenum format = GL_RGBA32F) {
            return create(key, size, std::vector<GLenum>(textures, format), attachDepth);
        }

        // A pass drawing into a texture of each format, in order, and a depth buffer if attachDepth
        hg::graphics::RenderPass *create(KeyType key, Vec2i size, std::vector<GLenum> const& formats,
                                         bool attachDepth = false) {
            m_passes.erase(key);
            m_passes.insert(std::make_pair(key, std::make_unique<RenderPass>()));
            auto pass = get(key);
            pass->formats = formats;
            pass->hasDepth = attachDepth;
            for (auto format : formats) {
                pass->textures.push_back(std::make_unique<RenderTarget>(size, format));
            }
            pass->buffer = std::make_unique<FrameBuffer>();
            pass->buffer->initialize();
            pass->buffer->bind();
            pass->buffer->initializeRenderBufferObject(size);

            for (int i = 0; i < (int) formats.size(); i++) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, pass->textures[i]->id, 0);
            }

            if (attachDepth) {
                pass->buffer->attachDepthBuffer(size);
            }

            return pass;
        }

        void validate() {
            for (const auto& [key, pass] : m_passes) {
                if (!pass->buffer->isComplete()) {
                    throw std::runtime_error("Incomplete Frame Buffer in Renderer!");
                }
            }
        }

        void removeAll() {
            m_passes.clear();
        }

        void activateTextures() {
            int idx = 0;
            for (auto& [key, pass] : m_passes) {
                glActiveTexture(GL_TEXTURE0 + idx++);
                pass->texture->bind();
            }
        }

        void clear(KeyType key, Color color) {
            m_passes[key]->buffer->bind();
            glClearColor(color[0], color[1], color[2], color[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // glDisable(GL_DEPTH_TEST);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            //glEnable(GL_CULL_FACE);
            //glCullFace(GL_BACK); // Cull back faces
            //glFrontFace(GL_CCW);
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void resize(KeyType key, hg::Vec2i size) {
            auto pass = get(key);
            auto formats = pass->formats;
            bool hasDepth = pass->hasDepth;
            create(key, size, formats, hasDepth);
        }

        void resizeAll(hg::Vec2i size) {
            for (const auto &[key, pass] : m_passes) {
                resize(key, size);
            }
        }

        void render(KeyType key, int attachments) {
            bind(key);
            // A vector rather than a variable length array, which is a Clang extension, not C++
            std::vector<unsigned int> attach(attachments);
            for (int i = 0; i < attachments; i++) {
                attach[i] = GL_COLOR_ATTACHMENT0 + i;
            }
            glDrawBuffers(attachments, attach.data());
            unbind(key);
        }

        void unbind(KeyType key) {
            m_passes[key]->buffer->unbind();
        }

        void bind(KeyType key) {
            m_passes[key]->buffer->bind();
        }

        hg::graphics::RenderPass* get(KeyType key) const {
            if (!has(key)) {
                throw std::runtime_error("RenderPass does not exist");
            }
            return m_passes.at(key).get();
        }

        bool has(KeyType key) const {
            return m_passes.find(key) != m_passes.end();
        }
    private:
        std::unordered_map<KeyType, std::unique_ptr<RenderPass>> m_passes;
    };

}

//
// Created by henry on 1/24/23.
//



#endif //HAGAME2_RENDERPASSES_H
