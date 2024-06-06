#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

class Shader
{
    // TODO: copy/move semantics
public:
    using ShaderPath = std::filesystem::path;

    // throws CreateShaderError
    Shader(const ShaderPath& vertex_src_path, const ShaderPath& fragment_src_path);
    inline ~Shader() noexcept { glDeleteProgram(_id); };

    inline auto bind() const noexcept -> void { glUseProgram(_id); }
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }

private:
    [[nodiscard]] static auto compile_shader(GLenum shader_type, const ShaderPath& shader_src_path) -> GLuint;
    [[nodiscard]] static auto compile_shader(GLenum shader_type, std::string_view shader_src) -> GLuint;
    [[nodiscard]] static auto link_shader(GLuint vertex_shader, GLuint fragment_shader) -> GLuint;

private:
    GLuint _id;

    static std::unordered_map<GLenum, const char*> shader_type_to_str;
};

class CreateShaderError : public std::runtime_error
{
protected:
    inline CreateShaderError(const char* message) noexcept : std::runtime_error(message) {}
    inline CreateShaderError(const std::string& message) noexcept : std::runtime_error(message) {}
    virtual inline ~CreateShaderError() noexcept {}
};

class FailedToReadShaderSourceFile : public CreateShaderError
{
public:
    inline FailedToReadShaderSourceFile(const char* message) noexcept : CreateShaderError(message) {}
    inline FailedToReadShaderSourceFile(const std::string& message) noexcept : CreateShaderError(message) {}
};

class FailedToCompileShader : public CreateShaderError
{
public:
    inline FailedToCompileShader(const char* message) noexcept : CreateShaderError(message) {}
    inline FailedToCompileShader(const std::string& message) noexcept : CreateShaderError(message) {}
};

class FailedToLinkShader : public CreateShaderError
{
public:
    inline FailedToLinkShader(const char* message) noexcept : CreateShaderError(message) {}
    inline FailedToLinkShader(const std::string& message) noexcept : CreateShaderError(message) {}
};
