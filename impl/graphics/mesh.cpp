//
// Created by henry on 12/18/22.
//
#include "../../include/hagame/graphics/mesh.h"
#include "../../include/hagame/utils/file.h"
#include "../../include/hagame/utils/string.h"

hg::graphics::MeshInstance::MeshInstance(Mesh* mesh) {
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

void hg::graphics::MeshInstance::update(Mesh* mesh) {
    m_mesh = mesh;
    auto newSize = m_mesh->vertices.size();
    m_vbo->resize(newSize);
    m_ebo->resize(newSize);
    m_vbo->update(0, m_mesh->vertices);
    m_ebo->update(0, m_mesh->indices);
}

void hg::graphics::MeshInstance::render() {
    m_vao->bind();
    m_ebo->bind();
    glDrawElements(GL_TRIANGLES, m_mesh->indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, m_mesh->vertices.size());
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


hg::graphics::Mesh::Mesh(std::string objPath) {

    std::vector<Vec3> positions = std::vector<Vec3>();
    std::vector<Vec2> textures = std::vector<Vec2>();
    std::vector<Vec3> normals = std::vector<Vec3>();

    unsigned int idx = 0;

    for (auto line : hg::utils::f_readLines(objPath)) {
        auto parts = utils::s_split(line, ' ');

        if (parts.size() == 0) continue;

        if (parts[0] == "v") {
            positions.push_back(Vec3(stof(parts[1]), stof(parts[2]), stof(parts[3])));
        }

        if (parts[0] == "vt") {
            textures.push_back(Vec2(stof(parts[1]), stof(parts[2])));
        }

        if (parts[0] == "vn") {
            normals.push_back(Vec3(stof(parts[1]), stof(parts[2]), stof(parts[3])));
        }

        if (parts[0] == "f") {

            auto f1 = utils::s_split(parts[1], '/');
            auto f2 = utils::s_split(parts[2], '/');
            auto f3 = utils::s_split(parts[3], '/');

            Vertex v1;
            Vertex v2;
            Vertex v3;

            v1.position = positions[stoi(f1[0]) - 1];
            v2.position = positions[stoi(f2[0]) - 1];
            v3.position = positions[stoi(f3[0]) - 1];

            v1.texCoords = textures[stoi(f1[1]) - 1];
            v2.texCoords = textures[stoi(f2[1]) - 1];
            v3.texCoords = textures[stoi(f3[1]) - 1];

            v1.normal = normals[stoi(f1[2]) - 1];
            v2.normal = normals[stoi(f2[2]) - 1];
            v3.normal = normals[stoi(f2[2]) - 1];

            vertices.insert(vertices.end(), { v1, v2, v3 });
            indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
            idx += 3;

            if (parts.size() == 5) {
                auto f4 = utils::s_split(parts[4], '/');
                Vertex v4;
                v4.position = positions[stoi(f4[0]) - 1];
                v4.texCoords = textures[stoi(f4[1]) - 1];
                v4.normal = normals[stoi(f4[2]) - 1];

                vertices.insert(vertices.end(), { v1, v3, v4});
                indices.insert(indices.end(), { idx, idx + 1, idx + 2 });
                idx += 3;
            }
        }
    }
}
