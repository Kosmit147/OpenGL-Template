#pragma once

#include <glad/glad.h>

#include <filesystem>

class Shader
{
public:
    using ShaderPath = std::filesystem::path;

    // throws CreateShaderError
    explicit Shader(const ShaderPath& vertex_src_path, const ShaderPath& fragment_src_path);
    inline ~Shader() noexcept { glDeleteProgram(_id); };

    Shader(const Shader& other) = delete;
    Shader(Shader&& other) = delete;

    inline auto use() const noexcept -> void { glUseProgram(_id); }
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }

    [[nodiscard]] auto get_unif_location(std::string_view name) const noexcept -> GLint;

    template<typename T> auto set_unif(std::string_view name, T val) const noexcept -> void
    {
        static_assert(false, "not implemented");
    }

    template<> inline auto set_unif<GLfloat>(std::string_view name, GLfloat val) const noexcept -> void
    {
        GLint location = get_unif_location(name);
        glUniform1f(location, val);
    }

    template<> inline auto set_unif<GLint>(std::string_view name, GLint val) const noexcept -> void
    {
        GLint location = get_unif_location(name);
        glUniform1i(location, val);
    }

private:
    [[nodiscard]] static auto compile_shader(GLenum shader_type, const ShaderPath& shader_src_path) -> GLuint;
    [[nodiscard]] static auto compile_shader(GLenum shader_type, std::string_view shader_src) -> GLuint;
    [[nodiscard]] static auto link_shader(GLuint vertex_shader, GLuint fragment_shader) -> GLuint;

private:
    GLuint _id;
    std::string _vertex_shader_src_file_path;
    std::string _fragment_shader_src_file_path;
    mutable std::unordered_map<std::string_view, GLint> _unif_cache{};

    static std::unordered_map<GLenum, const char*> _shader_type_to_str;
};

class CreateShaderError : public std::runtime_error
{
public:
    inline CreateShaderError(const char* message) noexcept : std::runtime_error(message) {}
    inline CreateShaderError(const std::string& message) noexcept : std::runtime_error(message) {}
};
