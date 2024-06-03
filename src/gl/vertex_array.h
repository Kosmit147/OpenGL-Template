#pragma once

#include <glad/glad.h>

class VertexArray
{
    // TODO: copy / move semantics
public:
    explicit inline VertexArray()
    {
        glGenVertexArrays(1, &_id);
        bind();
    }

    inline ~VertexArray() { glDeleteVertexArrays(1, &_id); }

    inline auto bind() const -> void { glBindVertexArray(_id); }
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }

private: 
    GLuint _id;
};