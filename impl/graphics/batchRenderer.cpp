//
// Created by henry on 12/10/23.
//
#include "../../../include/hagame/graphics/batchRenderer.h"
#include "../../../include/hagame/core/assets.h"

using namespace hg::graphics;

BatchQuad::BatchQuad(Quad *quad, hg::Entity *entity):
    size(quad->size),
    offset(quad->offset),
    color(quad->color),
    model(entity->model())
{}

BatchQuad::BatchQuad(hg::Vec2 _size, hg::Vec2 _offset, Color _color, hg::Mat4 _model):
    size(_size),
    offset(_offset),
    color(_color),
    model(_model)
{
}

BatchQuad::BatchQuad(hg::Vec2 _size, hg::Vec2 _offset, hg::Vec2 _texOffset, hg::Vec2 _texSize, Color _color,
                     hg::Mat4 _model):
    size(_size),
    texOffset(_texOffset),
    texSize(_texSize),
    offset(_offset),
    color(_color),
    model(_model)
{
}


BatchQuads::BatchQuads():
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
    vao->defineAttribute(m_buffer.get(), DataType::Float, 8, 4, offsetof(BatchQuad, model) + (0 * sizeof(float)));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 9, 4, offsetof(BatchQuad, model) + (4 * sizeof(float)));
    vao->defineAttribute(m_buffer.get(), DataType::Float,  10, 4, offsetof(BatchQuad, model) + (8 * sizeof(float)));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 11, 4, offsetof(BatchQuad, model) + (12 * sizeof(float)));

    vao->setInstanced(3, 11);
}

void BatchQuads::batch(hg::Entity *entity, Quad *quad) {
    m_data.emplace_back(quad, entity);
}

void BatchQuads::batch(hg::Vec2 size, hg::Vec2 offset, Color color, hg::Mat4 model) {
    m_data.emplace_back(size, offset, color, model);
}

void BatchQuads::clear() {
    m_data.clear();
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


BatchSprites::SpriteBuffer *BatchSprites::getBuffer(std::string texture) {
    if (m_buffers.find(texture) == m_buffers.end()) {

        auto buffer = std::make_shared<SpriteBuffer>();
        buffer->buffer = VertexBuffer<BatchQuad>::Dynamic(0);
        
        auto vao = buffer->mesh->getVAO();
        vao->bind();
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 3, 2, offsetof(BatchQuad, size));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 4, 2, offsetof(BatchQuad, offset));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 5, 2, offsetof(BatchQuad, texOffset));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 6, 2, offsetof(BatchQuad, texSize));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 7, 4, offsetof(BatchQuad, color));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 8, 4, offsetof(BatchQuad, model) + (0 * sizeof(float)));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 9, 4, offsetof(BatchQuad, model) + (4 * sizeof(float)));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float,  10, 4, offsetof(BatchQuad, model) + (8 * sizeof(float)));
        vao->defineAttribute(buffer->buffer.get(), DataType::Float, 11, 4, offsetof(BatchQuad, model) + (12 * sizeof(float)));

        vao->setInstanced(3, 11);

        m_buffers.insert(std::make_pair(texture, buffer));
    }

    return m_buffers[texture].get();
}

void BatchSprites::clear() {
    for (const auto&[texture, buffer] : m_buffers) {
        buffer->data.clear();
    }
}

void BatchSprites::batch(hg::Entity *entity, Sprite *sprite) {
    getBuffer(sprite->texture)->data.emplace_back(sprite, entity);
}

void BatchSprites::batch(std::string texture, hg::Vec2 size, hg::Vec2 offset, Color color, hg::Mat4 model) {
    getBuffer(texture)->data.emplace_back(size, offset, color, model);
}

void BatchSprites::batch(std::string texture, Vec2 size, Vec2 offset, Rect texRect, Color color, Mat4 model){
    getBuffer(texture)->data.emplace_back(size, offset, texRect.pos, texRect.size, color, model);
}

void BatchSprites::render() {
    for (const auto&[texture, buffer] : m_buffers) {

        std::sort(buffer->data.begin(), buffer->data.end(), [](auto& a, auto& b) {
            return a.model.get(2, 3) < b.model.get(2, 3); // Compare the Z values of each quad
        });

        buffer->buffer->bind();
        buffer->buffer->clear();
        buffer->buffer->resize(buffer->data.size());
        buffer->buffer->update(0, buffer->data);
        buffer->mesh->getVAO()->bind();
        hg::getTexture(texture)->bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, buffer->mesh->size(), buffer->data.size());
        buffer->buffer->unbind();
    }
}

