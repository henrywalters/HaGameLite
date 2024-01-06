//
// Created by henry on 1/24/23.
//

#ifndef HAGAME2_RENDERPASSES_H
#define HAGAME2_RENDERPASSES_H

#include "./frameBuffer.h"

namespace hg::graphics {
    struct RenderPass {
        std::unique_ptr<RawTexture<GL_RGBA32F>> texture;
        std::unique_ptr<FrameBuffer> buffer;
    };

    template <class KeyType>
    class RenderPasses {
    public:

        hg::graphics::RenderPass *create(KeyType key, Vec2i size) {
            m_passes.insert(std::make_pair(key, std::make_unique<RenderPass>()));
            auto pass = get(key);
            pass->texture = std::make_unique<RawTexture<GL_RGBA32F>>(size);
            pass->buffer = std::make_unique<FrameBuffer>();
            pass->buffer->initialize();
            pass->buffer->bind();
            pass->buffer->initializeRenderBufferObject(size);

            pass->buffer->attachRawTexture(pass->texture.get());
            return pass;
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
            glClearColor(color[0], color[1], color[2], 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glEnable(GL_BLEND);
            //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void resize(KeyType key, hg::Vec2i size) {
            m_passes.erase(key);
            create(key, size);
        }

        void resizeAll(hg::Vec2i size) {
            for (const auto &[key, pass] : m_passes) {
                resize(key, size);
            }
        }

        void render(KeyType key, int attachments) {
            bind(key);
            unsigned int attach[attachments];
            for (int i = 0; i < attachments; i++) {
                attach[i] = GL_COLOR_ATTACHMENT0 + i;
            }
            glDrawBuffers(1, attach);
            unbind(key);
        }

        void unbind(KeyType key) {
            m_passes[key]->buffer->unbind();
        }

        void bind(KeyType key) {
            m_passes[key]->buffer->bind();
        }

        hg::graphics::RenderPass* get(KeyType key) {
            if (!has(key)) {
                throw std::runtime_error("RenderPass does not exist");
            }
            return m_passes[key].get();
        }

        bool has(KeyType key) {
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
