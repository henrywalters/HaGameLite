//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_MESH_H
#define HAGAME2_MESH_H

#include <memory>

#include "../math/aliases.h"
#include "../math/transform.h"
#include "buffer.h"
#include "vertexArray.h"
#include "vertex.h"

namespace hg::graphics {

    // A Mesh is a collection of vertices and their indices representing a 3D object
    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    // A MeshInstance is an instance of a Mesh able to be rendered. This is easier to use, but slower, than the MeshBufferInstance
    class MeshInstance {
    public:

        MeshInstance(std::shared_ptr<Mesh> mesh);

        // Update the MeshInstance to reflect the current mesh state
        void update();

        void render();

    private:
        std::shared_ptr<Mesh> m_mesh;
        std::unique_ptr<VertexBuffer<Vertex>> m_vbo;
        std::unique_ptr<ElementBuffer<unsigned int>> m_ebo;
        std::unique_ptr<VertexArray> m_vao;

        void initialize();
    };

    const size_t DEFAULT_MAX_BUFFER_SIZE = 100;

    using MeshBufferInstance = unsigned int;

    class MeshBuffer {
    public:

        MeshBuffer(std::shared_ptr<Mesh> mesh, size_t capacity = DEFAULT_MAX_BUFFER_SIZE):
            m_mesh(mesh),
            m_capacity(capacity),
            m_size(0)
        {}

        void initialize(int layoutStart);

        void clear();

        MeshBufferInstance insert(math::Transform transform);

        void update(MeshBufferInstance instance, math::Transform transform);

        math::Transform get(MeshBufferInstance instance);

        size_t size() { return m_size; }
        size_t capacity() { return m_capacity; }

    private:

        std::shared_ptr<Mesh> m_mesh;
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<VertexBuffer<math::Transform>> m_buffer;
        size_t m_size;
        size_t m_capacity;
    };
}

#endif //HAGAME2_MESH_H
