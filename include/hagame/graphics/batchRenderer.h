//
// Created by henry on 12/9/23.
//

#ifndef HAGAME2_BATCHRENDERER_H
#define HAGAME2_BATCHRENDERER_H

#include <functional>
#include <limits>
#include <unordered_map>
#include <algorithm>
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
        // How strongly the sprite's emissive map glows, at attribute 13. 1 is as painted, 0 is off. Last, so the
        // fields before it keep their places
        float emission = 1.0f;

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
        void batch(std::string texture, Vec2 size, Vec2 offset, Rect texRect, Color color, Mat4 model, float emission = 1.0f);

        // Draws every sprite in one go, each texture in the unit its index says
        void render() override {
            render(std::numeric_limits<int>::max(), nullptr);
        }

        // Draws back to front, in as many draws as it takes for each to use at most maxTextures different textures:
        // the most a shader has samplers for. In each draw, texture i of the draw is in unit i and sprites' texture
        // indices are remapped to match. bindRun, if given, is called before each draw with its textures in unit
        // order, to bind whatever goes with them, like normal maps
        void render(int maxTextures, std::function<void(std::vector<std::string> const& textures)> const& bindRun) {

            if (m_data.empty()) return;

            std::vector<std::string> names(m_textures.size());
            for (const auto& [texture, index] : m_textures) {
                names[index] = texture;
            }

            // Stable, so sprites at the same depth keep the order they were batched in rather than swapping
            // Read through mat: stable_sort compares const quads, and Mat4::get isn't const
            const int z = Mat4::FlattenIndex(2, 3);
            std::stable_sort(m_data.begin(), m_data.end(), [z](BatchQuad const& a, BatchQuad const& b) {
                return a.model.mat[z] < b.model.mat[z]; // Compare the Z values of each quad
            });

            size_t start = 0;
            while (start < m_data.size()) {
                // As many sprites as fit in maxTextures textures, in depth order
                std::unordered_map<int, int> units;
                std::vector<std::string> run;
                size_t end = start;
                for (; end < m_data.size(); end++) {
                    int index = (int) m_data[end].textureIndex;
                    if (units.find(index) == units.end()) {
                        if ((int) run.size() == maxTextures) {
                            break;
                        }
                        units[index] = (int) run.size();
                        run.push_back(names[index]);
                    }
                }

                std::vector<BatchQuad> draw(m_data.begin() + start, m_data.begin() + end);
                for (auto& quad : draw) {
                    quad.textureIndex = (float) units[(int) quad.textureIndex];
                }

                for (int i = 0; i < (int) run.size(); i++) {
                    glActiveTexture(GL_TEXTURE0 + i);
                    hg::getTexture(run[i])->bind();
                }
                if (bindRun) {
                    bindRun(run);
                }

                m_buffer->bind();
                m_buffer->clear();
                m_buffer->resize(draw.size());
                m_buffer->update(0, draw);
                m_mesh.getVAO()->bind();
                glDrawArraysInstanced(GL_TRIANGLES, 0, m_mesh.size(), draw.size());
                m_buffer->unbind();

                start = end;
            }
        }

        std::unordered_map<std::string, int> m_textures;

    protected:

        void onClear() override;

    private:

        int getTextureIndex(std::string texture);



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
