#include "shader.hpp"

#include <format>
#include <print>
#include <sstream>
#include <utility>
#include <vector>

#include "io/file_io.hpp"

static constexpr GLuint invalid_shader_id = 0;
static constexpr GLuint invalid_shader_program_id = 0;

static auto delete_shader_if_invalid(GLuint id) noexcept -> void
{
    if (id == invalid_shader_id)
        glDeleteShader(id);
}

static auto delete_shader_program_if_invalid(GLuint id) noexcept -> void
{
    if (id == invalid_shader_program_id)
        glDeleteProgram(id);
}

std::unordered_map<GLenum, const char*> Shader::shader_type_to_str = {
    { GL_VERTEX_SHADER, "vertex" },
    { GL_FRAGMENT_SHADER, "fragment" },
};

Shader::Shader(const ShaderPath& vertex_src_path, const ShaderPath& fragment_src_path)
{
    GLuint vertex_shader = invalid_shader_id;
    GLuint fragment_shader = invalid_shader_id;
    GLuint shader_program = invalid_shader_program_id;

    try
    {
        vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src_path);
        fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src_path);
        shader_program = link_shader(vertex_shader, fragment_shader);
    }
    catch ([[maybe_unused]] CreateShaderError& e)
    {
        delete_shader_if_invalid(vertex_shader);
        delete_shader_if_invalid(fragment_shader);
        delete_shader_program_if_invalid(shader_program);

        throw;
    }

#ifndef _DEBUG
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
#endif

    _id = shader_program;
    bind();
}

auto Shader::compile_shader(GLenum shader_type, const ShaderPath& shader_src_path) -> GLuint
{
    try
    {
        std::stringstream src = read_from_file(shader_src_path);
        auto src_view = src.view();
        return compile_shader(shader_type, src_view);
    }
    catch (FileIoError& e)
    {
        auto message =
            std::format("Can't read {} shader source file: {}", shader_type_to_str[shader_type], e.what());
        std::println(stderr, "{}", message);
        throw FailedToReadShaderSourceFile{ message };
    }
}

auto Shader::compile_shader(GLenum shader_type, std::string_view shader_src) -> GLuint
{
    const char* src_ptr = shader_src.data();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &src_ptr, NULL);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) [[unlikely]]
    {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> error_log(log_length);
        glGetShaderInfoLog(shader, log_length, &log_length, &error_log[0]);

        auto message =
            std::format("Can't compile {} shader: {}", shader_type_to_str[shader_type], error_log.data());
        std::println(stderr, "{}", message);
        throw FailedToCompileShader{ message };
    }

    return shader;
}

auto Shader::link_shader(GLuint vertex_shader, GLuint fragment_shader) -> GLuint
{
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint success = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) [[unlikely]]
    {
        GLint log_length = 0;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> error_log(log_length);
        glGetProgramInfoLog(shader_program, log_length, &log_length, &error_log[0]);

        auto message = std::format("Can't link shader: {}", error_log.data());
        std::println(stderr, "{}", message);
        throw FailedToLinkShader{ message };
    }

    return shader_program;
}
