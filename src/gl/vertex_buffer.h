#pragma once

#include <glad/glad.h>

#include <span>

#include "types.h"

class VertexBuffer
{
    // TODO: move/copy semantics
public:
    explicit inline VertexBuffer() noexcept
    {
        glGenBuffers(1, &_id);
        bind();
    }

    explicit inline VertexBuffer(const void* data, usize data_size, GLenum usage) noexcept
    {
        glGenBuffers(1, &_id);
        bind();
        buffer_data(data, data_size, usage);
    }

    template <typename DataType>
    explicit inline VertexBuffer(std::span<DataType> data, GLenum usage) noexcept
    {
        glGenBuffers(1, &_id);
        bind();
        buffer_data(data, usage);
    }

    template <typename DataType, std::size_t data_size>
    explicit inline VertexBuffer(std::span<DataType, data_size> data,
        GLenum usage) noexcept
    {
        glGenBuffers(1, &_id);
        bind();
        buffer_data(data, usage);
    }

    inline ~VertexBuffer() noexcept { glDeleteBuffers(1, &_id); }

    // bind first!
    inline auto buffer_data(const void* data, usize data_size,
        GLenum usage) const noexcept -> void
    {
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data_size), data, usage);
    }

    // bind first!
    template <typename DataType>
    inline auto buffer_data(std::span<DataType> data, GLenum usage) const noexcept -> void
    {
        glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    }

    // bind first!
    template <typename DataType, std::size_t data_size>
    inline auto buffer_data(std::span<DataType, data_size> data,
        GLenum usage) const noexcept -> void
    {
        glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    }

    inline auto bind() const noexcept -> void { glBindBuffer(GL_ARRAY_BUFFER, _id); }
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }

private:
    GLuint _id;
};
