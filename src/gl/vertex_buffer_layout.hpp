#pragma once

template<usize n> struct Anything
{
    template<typename T> constexpr inline operator T() const { return T{}; };
};

template<typename T, usize... ints>
concept ConstructibleFromNInitializers = requires { T{ Anything<ints>{}... }; };

template<typename T, usize n> static constexpr inline auto is_constructible_from_n_initializers() -> bool
{
    constexpr auto unpack = [&]<usize... ints>(std::index_sequence<ints...>) {
        return ConstructibleFromNInitializers<T, ints...>;
    };

    return unpack(std::make_index_sequence<n>{});
}

template<typename T, usize n = 0u> static constexpr inline auto get_struct_arity() -> usize
{
    constexpr auto constructible = is_constructible_from_n_initializers<T, n>();

    if constexpr (!constructible)
        return n - 1;
    else if constexpr (constructible)
        return get_struct_arity<T, n + 1>();
    else
        return get_struct_arity<T, n + 1>();
}

namespace {

struct AttribSpecification
{
    GLint count;
    GLenum type;
};

template<typename T> constexpr inline auto get_attrib_specification() -> AttribSpecification
{
    static_assert(false, "not implemented");
}

template<> constexpr inline auto get_attrib_specification<GLfloat>() -> AttribSpecification
{
    return { 1, GL_FLOAT };
}

template<> constexpr inline auto get_attrib_specification<glm::vec2>() -> AttribSpecification
{
    static_assert(sizeof(glm::vec2) == sizeof(GLfloat) * 2);
    return { 2, GL_FLOAT };
}

template<> constexpr inline auto get_attrib_specification<glm::vec3>() -> AttribSpecification
{
    static_assert(sizeof(glm::vec3) == sizeof(GLfloat) * 3);
    return { 3, GL_FLOAT };
}

template<> constexpr inline auto get_attrib_specification<glm::vec4>() -> AttribSpecification
{
    static_assert(sizeof(glm::vec4) == sizeof(GLfloat) * 4);
    return { 4, GL_FLOAT };
}

template<typename T>
inline auto bind_vertex_buffer_attrib(GLuint& location, GLsizei stride, GLsizei& offset) noexcept -> void
{
    constexpr auto spec = get_attrib_specification<T>();
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, spec.count, spec.type, GL_FALSE, stride,
                          reinterpret_cast<const void*>(static_cast<usize>(offset)));

    location++;
    offset += sizeof(T);
}

} // namespace

template<typename VertexType> inline auto bind_vertex_buffer_layout() noexcept -> void
{
    VertexType dummy{};
    constexpr auto arity = get_struct_arity<VertexType>();

    GLuint location = 0;
    constexpr GLsizei stride = sizeof(VertexType);
    GLsizei offset = 0;

    if constexpr (arity == 0)
    {
        return;
    }
    else if constexpr (arity == 1)
    {
        [[maybe_unused]] auto& [f1] = dummy;
        bind_vertex_buffer_attrib<decltype(f1)>(location, stride, offset);
    }
    else if constexpr (arity == 2)
    {
        [[maybe_unused]] auto& [f1, f2] = dummy;
        bind_vertex_buffer_attrib<decltype(f1)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f2)>(location, stride, offset);
    }
    else if constexpr (arity == 3)
    {
        [[maybe_unused]] auto& [f1, f2, f3] = dummy;
        bind_vertex_buffer_attrib<decltype(f1)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f2)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f3)>(location, stride, offset);
    }
    else if constexpr (arity == 4)
    {
        [[maybe_unused]] auto& [f1, f2, f3, f4] = dummy;
        bind_vertex_buffer_attrib<decltype(f1)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f2)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f3)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f4)>(location, stride, offset);
    }
    else if constexpr (arity == 5)
    {
        [[maybe_unused]] auto& [f1, f2, f3, f4, f5] = dummy;
        bind_vertex_buffer_attrib<decltype(f1)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f2)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f3)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f4)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f5)>(location, stride, offset);
    }
    else if constexpr (arity == 6)
    {
        [[maybe_unused]] auto& [f1, f2, f3, f4, f5, f6] = dummy;
        bind_vertex_buffer_attrib<decltype(f1)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f2)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f3)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f4)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f5)>(location, stride, offset);
        bind_vertex_buffer_attrib<decltype(f6)>(location, stride, offset);
    }
    else
    {
        static_assert(false, "not implemented");
    }
}
