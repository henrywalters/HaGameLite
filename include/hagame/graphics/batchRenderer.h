//
// Created by henry on 12/9/23.
//

#ifndef HAGAME2_BATCHRENDERER_H
#define HAGAME2_BATCHRENDERER_H

#include "primitives/quad.h"
#include "components/quad.h"
#include "components/sprite.h"

namespace hg::graphics {

    struct BatchQuad {
    public:
        Vec2 size;
        Vec2 offset;
        Vec2 texOffset = Vec2::Zero();
        Vec2 texSize = Vec2::Identity();
        Color color;
        Mat4 model;

        BatchQuad(Quad* quad, Entity* entity);
        BatchQuad(Vec2 _size, Vec2 _offset, Color _color, Mat4 _model);
        BatchQuad(Vec2 _size, Vec2 _offset, Vec2 _texOffset, Vec2 _texSize, Color _color, Mat4 _model);
    };

    class BatchQuads {
    public:

        BatchQuads();

        void clear();

        void batch(Entity* entity, Quad* quad);
        void batch(Vec2 size, Vec2 offset, Color color, Mat4 model);

        void render();

    private:
        std::vector<BatchQuad> m_data;
        std::unique_ptr<VertexBuffer<BatchQuad>> m_buffer;
        primitives::Quad m_primitive;
        MeshInstance m_mesh;
    };

    class BatchSprites {
    public:

        void clear();

        void batch(Entity* entity, Sprite* sprite);
        void batch(std::string texture, Vec2 size, Vec2 offset, Color color, Mat4 model);
        void batch(std::string texture, Vec2 size, Vec2 offset, Rect texRect, Color color, Mat4 model);

        void render();

    private:

        struct SpriteBuffer {

            SpriteBuffer()
            {
                primitive.size(1.0);
                primitive.centered(true);
                mesh = std::make_unique<MeshInstance>(&primitive);
                mesh->update(&primitive);
            }

            std::vector<BatchQuad> data;
            std::unique_ptr<VertexBuffer<BatchQuad>> buffer;
            std::unique_ptr<MeshInstance> mesh;
            primitives::Quad primitive;
        };

        SpriteBuffer* getBuffer(std::string texture);

        std::unordered_map<std::string, std::shared_ptr<SpriteBuffer>> m_buffers;
    };

    class BatchRenderer {
    public:

        BatchQuads quads;
        BatchSprites sprites;

    };

}

#endif //HAGAME2_BATCHRENDERER_H
