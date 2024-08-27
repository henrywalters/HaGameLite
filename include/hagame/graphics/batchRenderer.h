//
// Created by henry on 12/9/23.
//

#ifndef HAGAME2_BATCHRENDERER_H
#define HAGAME2_BATCHRENDERER_H

#include "primitives/quad.h"
#include "components/quad.h"
#include "components/sprite.h"
#include "../core/assets.h"

namespace hg::graphics {

    struct BatchQuad {
    public:
        Vec2 size;
        Vec2 offset;
        Vec2 texOffset = Vec2::Zero();
        Vec2 texSize = Vec2::Identity();
        float textureIndex = 0;
        Color color;
        Mat4 model;

        BatchQuad(Quad* quad, Entity* entity);
        BatchQuad(Vec2 _size, Vec2 _offset, int _texIndex, Color _color, Mat4 _model);
        BatchQuad(Vec2 _size, Vec2 _offset, int _texIndex, Vec2 _texOffset, Vec2 _texSize, Color _color, Mat4 _model);
    };

    class Batcher {
    public:

        Batcher();

        void clear();
        virtual void render() = 0;

    protected:

        virtual void onClear() {};

        std::vector<BatchQuad> m_data;
        std::unique_ptr<VertexBuffer<BatchQuad>> m_buffer;
        primitives::Quad m_primitive;
        MeshInstance m_mesh;

    };

    class BatchQuads : public Batcher {
    public:

        void batch(Entity* entity, Quad* quad);
        void batch(Vec2 size, Vec2 offset, Color color, Mat4 model);

        void render() override;
    };

    class BatchSprites : public Batcher {
    public:

        void batch(Entity* entity, Sprite* sprite);
        void batch(std::string texture, Vec2 size, Vec2 offset, Color color, Mat4 model);
        void batch(std::string texture, Vec2 size, Vec2 offset, Rect texRect, Color color, Mat4 model);

        void render() override{

            for (const auto&[texture, index] : m_textures) {
                auto tex = hg::getTexture(texture);
                glActiveTexture(GL_TEXTURE0 + index);
                tex->bind();
            }

            std::sort(m_data.begin(), m_data.end(), [](auto& a, auto& b) {
                return a.model.get(2, 3) < b.model.get(2, 3); // Compare the Z values of each quad
            });

            m_buffer->bind();
            m_buffer->clear();
            m_buffer->resize(m_data.size());
            m_buffer->update(0, m_data);
            m_mesh.getVAO()->bind();
            glDrawArraysInstanced(GL_TRIANGLES, 0, m_mesh.size(), m_data.size());
            m_buffer->unbind();
        }

    protected:

        void onClear() override;

    private:

        int getTextureIndex(std::string texture);

        std::unordered_map<std::string, int> m_textures;

//        struct SpriteBuffer {
//
//            SpriteBuffer()
//            {
//                primitive.size(1.0);
//                primitive.centered(true);
//                mesh = std::make_unique<MeshInstance>(&primitive);
//                mesh->update(&primitive);
//            }
//
//            std::unordered_map<std::string, int> textures;
//            std::vector<BatchQuad> data;
//            std::unique_ptr<VertexBuffer<BatchQuad>> buffer;
//            std::unique_ptr<MeshInstance> mesh;
//            primitives::Quad primitive;
//        };
//
//
//
//        //SpriteBuffer* getBuffer(std::string texture);
//
//        //std::unordered_map<std::string, std::shared_ptr<SpriteBuffer>> m_buffers;
    };

    class BatchRenderer {
    public:

        BatchQuads quads;
        BatchSprites sprites;

    };
}

#endif //HAGAME2_BATCHRENDERER_H
