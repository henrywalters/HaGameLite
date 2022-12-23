//
// Created by henry on 12/18/22.
//
#include "../../include/hagame/graphics/mesh.h"

hg::graphics::MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh) {
    m_mesh = mesh;
    initialize();
}

void hg::graphics::MeshInstance::initialize() {
    m_vbo = VertexBuffer<Vertex>::Dynamic(m_mesh->vertices);
    m_ebo = ElementBuffer<unsigned int>::Dynamic(m_mesh->indices);
    m_vao = std::make_unique<VertexArray>();

    m_vao->initialize();

    m_ebo->bind();
    m_vao->bind();

    m_vao->defineAttribute<Vertex>(m_vbo.get(), DataType::Float, 0, 3, offsetof(Vertex, position));
    m_vao->defineAttribute<Vertex>(m_vbo.get(), DataType::Float, 1, 3, offsetof(Vertex, normal));
    m_vao->defineAttribute<Vertex>(m_vbo.get(), DataType::Float, 2, 2, offsetof(Vertex, texCoords));
}

void hg::graphics::MeshInstance::update() {
    m_vbo->clear();
    auto newSize = m_mesh->vertices.size();
    m_vbo->resize(newSize);
    m_ebo->resize(newSize);
    m_vbo->update(0, m_mesh->vertices);
    m_ebo->update(0, m_mesh->indices);
}

void hg::graphics::MeshInstance::render() {
    m_vao->bind();
    m_ebo->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_mesh->vertices.size());
}

void hg::graphics::MeshBuffer::initialize(int layoutStart) {
    m_buffer = VertexBuffer<math::Transform>::Dynamic(capacity());
    m_buffer->bind();
    m_vao = std::make_unique<VertexArray>();
    m_vao->initialize();
    m_vao->bind();

    for (int i = 0; i < 12; i++) {
        m_vao->defineAttribute(m_buffer.get(), DataType::Float, layoutStart + i, 4, sizeof(Vec4) * i % 4);
    }

    m_vao->setInstanced(layoutStart, layoutStart + 12);
}

void hg::graphics::MeshBuffer::clear() {
    m_buffer->clear();
}

hg::graphics::MeshBufferInstance hg::graphics::MeshBuffer::insert(hg::math::Transform transform) {
    m_buffer->update(m_size++, transform);
    return m_size - 1;
}

void hg::graphics::MeshBuffer::update(hg::graphics::MeshBufferInstance instance, hg::math::Transform transform) {
    m_buffer->update(instance, transform);
}

hg::math::Transform hg::graphics::MeshBuffer::get(hg::graphics::MeshBufferInstance instance) {
    return m_buffer->read(instance);
}

