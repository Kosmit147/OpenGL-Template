#pragma once

#include <glad/glad.h>

class IndexBuffer
{
public:
    explicit inline IndexBuffer() noexcept
    {
        glGenBuffers(1, &_id);
        bind();
    }

    explicit inline IndexBuffer(const void* data, usize data_size, GLenum usage) noexcept
    {
        glGenBuffers(1, &_id);
        bind();
        buffer_data(data, data_size, usage);
    }

    template <typename DataType> explicit inline IndexBuffer(std::span<DataType> data, GLenum usage) noexcept
    {
        glGenBuffers(1, &_id);
        bind();
        buffer_data(data, usage);
    }

    template <typename DataType, std::size_t data_size>
    explicit inline IndexBuffer(std::span<DataType, data_size> data, GLenum usage) noexcept
    {
        glGenBuffers(1, &_id);
        bind();
        buffer_data(data, usage);
    }

    inline ~IndexBuffer() noexcept { glDeleteBuffers(1, &_id); }

    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) = delete;

    // bind first!
    inline auto buffer_data(const void* data, usize data_size, GLenum usage) const noexcept -> void
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data_size), data, usage);
    }

    // bind first!
    template <typename DataType>
    inline auto buffer_data(std::span<DataType> data, GLenum usage) const noexcept -> void
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), data.data(), usage);
    }

    // bind first!
    template <typename DataType, std::size_t data_size>
    inline auto buffer_data(std::span<DataType, data_size> data, GLenum usage) const noexcept -> void
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), data.data(), usage);
    }

    inline auto bind() const noexcept -> void { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }

private:
    GLuint _id;
};
