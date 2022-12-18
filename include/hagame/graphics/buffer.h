//
// Created by henry on 12/17/22.
//

#ifndef HAGAME2_BUFFER_H
#define HAGAME2_BUFFER_H

#include <memory>
#include <vector>
#include <cassert>

#include "glfw.h"

namespace hg::graphics {
    template <class DataType, unsigned int Type>
    class Buffer {
    public:

        unsigned int id;
        bool dynamic;
        size_t max;
        size_t allocated;

        void initialize() {
            glGenBuffers(1, &id);
        }

        Buffer() : max(0), allocated(0) {}

        ~Buffer() {
            glDeleteBuffers(1, &id);
        }

        static std::shared_ptr<Buffer> Dynamic(int maxSize) {
            std::shared_ptr<Buffer<DataType, Type>> buffer = std::make_shared<Buffer<DataType, Type>>();
            buffer->initialize();
            buffer->dynamic = true;
            buffer->max = maxSize;
            glBindBuffer(Type, buffer->id);
            glBufferData(Type, sizeof(DataType) * maxSize, NULL, GL_DYNAMIC_DRAW);
            glBindBuffer(Type, 0);
            return buffer;
        }

        static std::shared_ptr<Buffer> Dynamic(std::vector<DataType> data) {
            std::shared_ptr<Buffer<DataType, Type>> buffer = std::make_shared<Buffer<DataType, Type>>();
            buffer->initialize();
            buffer->dynamic = true;
            buffer->allocated = data.size();
            buffer->max = buffer->allocated;
            glBindBuffer(Type, buffer->id);
            glBufferData(Type, sizeof(DataType) * buffer->max, &data[0], GL_DYNAMIC_DRAW);
            glBindBuffer(Type, 0);
            return buffer;
        }

        static std::shared_ptr<Buffer> Static(std::vector<DataType> data) {
            std::shared_ptr<Buffer<DataType, Type>> buffer = std::make_shared<Buffer<DataType, Type>>();
            buffer->initialize();
            buffer->dynamic = false;
            buffer->max = data.size();
            buffer->allocated = buffer->max;
            glBindBuffer(Type, buffer->id);
            glBufferData(Type, sizeof(DataType) * data.size(), &data[0], GL_STATIC_DRAW);
            glBindBuffer(Type, 0);
            return buffer;
        }

        void resize(int maxSize) {
            clear();
            max = maxSize;
            glBindBuffer(Type, id);
            glBufferData(Type, sizeof(DataType) * maxSize, NULL, GL_DYNAMIC_DRAW);
            glBindBuffer(Type, 0);
        }

        void canAssign(unsigned int index, size_t size) {
            assert(index + size < max);
        }

        // Update a single instance of the DataType at a given index
        void update(unsigned int index, DataType data) {
            canAssign(index, sizeof(DataType));
            bind();
            glBufferSubData(Type, index * sizeof(DataType), sizeof(DataType), &data);
            unbind();
        }

        // Update several instances of the DataType starting at a given index
        void update(unsigned int index, std::vector<DataType> data) {
            canAssign(index, sizeof(DataType) * data.size());
            bind();
            glBufferSubData(Type, index * sizeof(DataType), sizeof(DataType) * data.size(), &data[0]);
            unbind();
        }

        // Update a member of a single instance at a given index. Useful for updating a member of a struct or class
        template <class MemberType>
        void update(unsigned int index, size_t memberOffseType, MemberType memberData) {
            canAssign(index, sizeof(MemberType));
            bind();
            glBufferSubData(Type, index * sizeof(DataType) + memberOffseType, sizeof(MemberType), &memberData);
            unbind();
        }

        // Return the data at the given index. Useful for updating the state of a buffer
        DataType read(unsigned int index) {
            canAssign(index, sizeof(DataType));
            bind();
            DataType data;
            glGetBufferSubData(Type, index * sizeof(DataType), sizeof(DataType), &data);
            return data;
        }

        void clear() {
            bind();
            glClearBufferData(Type, GL_RG32F, GL_RG32F, GL_FLOAT, NULL);
            allocated = 0;
        }

        void bind() {
            glBindBuffer(Type, id);
        }

        void unbind() {
            glBindBuffer(Type, 0);
        }
    };

    template <class DataType>
    using VertexBuffer = Buffer<DataType, GL_ARRAY_BUFFER>;

    template <class DataType>
    using ElementBuffer = Buffer<DataType, GL_ELEMENT_ARRAY_BUFFER>;
}

#endif //HAGAME2_BUFFER_H
