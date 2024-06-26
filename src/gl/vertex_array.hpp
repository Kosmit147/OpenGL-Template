#pragma once

#include <glad/glad.h>

class VertexArray
{
public:
    explicit inline VertexArray() noexcept
    {
        glGenVertexArrays(1, &_id);
        bind();
    }

    inline ~VertexArray() noexcept { glDeleteVertexArrays(1, &_id); }

    VertexArray(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) = delete;

    inline auto bind() const noexcept -> void { glBindVertexArray(_id); }
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }

private:
    GLuint _id;
};
