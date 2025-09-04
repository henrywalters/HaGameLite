//
// Created by henry on 12/10/23.
//
#include "../../../include/hagame/graphics/batchRenderer.h"
#include "../../../include/hagame/core/assets.h"

using namespace hg::graphics;

BatchQuad::BatchQuad(Quad *quad, hg::Entity *entity):
    size(quad->size),
    offset(quad->offset + (quad->centered ? hg::Vec2::Zero() : quad->size * 0.5)),
    color(quad->color),
    model(entity->model())
{}

BatchQuad::BatchQuad(hg::Vec2 _size, hg::Vec2 _offset, int _texIndex, Color _color, hg::Mat4 _model):
    size(_size),
    offset(_offset),
    color(_color),
    textureIndex(_texIndex),
    model(_model)
{
}

BatchQuad::BatchQuad(hg::Vec2 _size, hg::Vec2 _offset, int _texIndex, hg::Vec2 _texOffset, hg::Vec2 _texSize, Color _color,
                     hg::Mat4 _model):
    size(_size),
    texOffset(_texOffset),
    texSize(_texSize),
    textureIndex(_texIndex),
    offset(_offset),
    color(_color),
    model(_model)
{
}

Batcher::Batcher():
        m_mesh(&m_primitive),
        m_buffer(VertexBuffer<BatchQuad>::Dynamic(0))
{
    m_primitive.size(1.0);
    m_primitive.centered(true);
    m_mesh.update(&m_primitive);

    auto vao = m_mesh.getVAO();
    vao->bind();
    vao->defineAttribute(m_buffer.get(), DataType::Float, 3, 2, offsetof(BatchQuad, size));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 4, 2, offsetof(BatchQuad, offset));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 5, 2, offsetof(BatchQuad, texOffset));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 6, 2, offsetof(BatchQuad, texSize));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 7, 4, offsetof(BatchQuad, color));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 8, 1, offsetof(BatchQuad, textureIndex));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 9, 4, offsetof(BatchQuad, model) + (0 * sizeof(float)));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 10, 4, offsetof(BatchQuad, model) + (4 * sizeof(float)));
    vao->defineAttribute(m_buffer.get(), DataType::Float,  11, 4, offsetof(BatchQuad, model) + (8 * sizeof(float)));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 12, 4, offsetof(BatchQuad, model) + (12 * sizeof(float)));

    vao->setInstanced(3, 12);
}

void Batcher::clear() {
    m_data.clear();
    onClear();
}

void BatchQuads::batch(hg::Entity *entity, Quad *quad) {
    m_data.emplace_back(quad, entity);
}

void BatchQuads::batch(hg::Vec2 size, hg::Vec2 offset, Color color, hg::Mat4 model) {
    m_data.emplace_back(size, offset, 0, color, model);
}

void BatchQuads::render() {
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

void BatchSprites::onClear() {

}

void BatchSprites::batch(hg::Entity *entity, Sprite *sprite) {
    auto index = getTextureIndex(sprite->texture.path());
    m_data.emplace_back(sprite->size, sprite->offset, index, sprite->color, entity->model());
}

void BatchSprites::batch(std::string texture, hg::Vec2 size, hg::Vec2 offset, Color color, hg::Mat4 model) {
    auto index = getTextureIndex(texture);
    m_data.emplace_back(size, offset, index, color, model);
}

void BatchSprites::batch(std::string texture, Vec2 size, Vec2 offset, Rect texRect, Color color, Mat4 model){
    auto index = getTextureIndex(texture);
    m_data.emplace_back(size, offset, index, texRect.pos, texRect.size, color, model);
}

int BatchSprites::getTextureIndex(std::string texture) {
    if (m_textures.find(texture) == m_textures.end()) {
        m_textures.insert(std::make_pair(texture, m_textures.size()));
    }
    return m_textures[texture];
}
