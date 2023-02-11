//
// Created by henry on 1/24/23.
//

#ifndef HAGAME2_RENDERPASSES_H
#define HAGAME2_RENDERPASSES_H

#include "./frameBuffer.h"

namespace hg::graphics {
    struct RenderPass {
        std::unique_ptr<RawTexture<GL_RGBA>> texture;
        std::unique_ptr<FrameBuffer> buffer;
    };

    template <class KeyType>
    class RenderPasses {
    public:
        RenderPass* create(KeyType key, Vec2i size);
        bool has(KeyType key);
        RenderPass* get(KeyType key);
        void bind(KeyType key);
        void unbind(KeyType key);
        void clear(KeyType key, Color color);
        void render(KeyType key, int attachments = 1);
        void activateTextures();

        void removeAll();
    private:
        std::unordered_map<KeyType, std::unique_ptr<RenderPass>> m_passes;
    };

}

//
// Created by henry on 1/24/23.
//

template<class KeyType>
hg::graphics::RenderPass *hg::graphics::RenderPasses<KeyType>::create(KeyType key, Vec2i size) {
    m_passes.insert(std::make_pair(key, std::make_unique<RenderPass>()));
    auto pass = get(key);
    pass->texture = std::make_unique<RawTexture<GL_RGBA>>(size);
    pass->buffer = std::make_unique<FrameBuffer>();
    pass->buffer->initialize();
    pass->buffer->bind();
    pass->buffer->initializeRenderBufferObject(size);

    pass->buffer->attachRawTexture(pass->texture.get());
    return pass;
}


template<class KeyType>
void hg::graphics::RenderPasses<KeyType>::removeAll() {
    m_passes.clear();
}


template<class KeyType>
void hg::graphics::RenderPasses<KeyType>::activateTextures() {
    int idx = 0;
    for (auto& [key, pass] : m_passes) {
        glActiveTexture(GL_TEXTURE0 + idx++);
        pass->texture->bind();
    }
}

template<class KeyType>
void hg::graphics::RenderPasses<KeyType>::clear(KeyType key, Color color) {
    m_passes[key]->buffer->bind();
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

template<class KeyType>
void hg::graphics::RenderPasses<KeyType>::render(KeyType key, int attachments) {
    bind(key);
    unsigned int attach[attachments];
    for (int i = 0; i < attachments; i++) {
        attach[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(1, attach);
    unbind(key);
}

template<class KeyType>
void hg::graphics::RenderPasses<KeyType>::unbind(KeyType key) {
    m_passes[key]->buffer->unbind();
}

template<class KeyType>
void hg::graphics::RenderPasses<KeyType>::bind(KeyType key) {
    m_passes[key]->buffer->bind();
}

template<class KeyType>
hg::graphics::RenderPass *hg::graphics::RenderPasses<KeyType>::get(KeyType key) {
    if (!has(key)) {
        throw std::runtime_error("RenderPass does not exist");
    }
    return m_passes[key].get();
}

template<class KeyType>
bool hg::graphics::RenderPasses<KeyType>::has(KeyType key) {
    return m_passes.find(key) != m_passes.end();
}

#endif //HAGAME2_RENDERPASSES_H
