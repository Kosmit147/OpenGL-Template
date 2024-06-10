#pragma once

#include <glad/glad.h>

#include <filesystem>

struct Texture2DOptions
{
    GLint horizontal_wrap = GL_REPEAT;
    GLint vertical_wrap = GL_REPEAT;
    GLint min_filter = GL_LINEAR;
    GLint mag_filter = GL_LINEAR;

    auto apply() const noexcept -> void;
};

class Texture2D
{
public:
    explicit Texture2D(const std::filesystem::path& path, bool generate_mipmap = true,
                       const Texture2DOptions* options = nullptr);
    inline ~Texture2D() noexcept { glDeleteTextures(1, &_id); }

    Texture2D(const Texture2D& other) = delete;
    Texture2D(Texture2D&& other) = delete;

    auto bind(u32 slot = 0) const noexcept -> void;
    [[nodiscard]] inline auto id() const noexcept -> GLuint { return _id; }
    [[nodiscard]] inline auto internal_format() const noexcept -> GLint { return _internal_format; }

private:
    GLuint _id;
    GLint _internal_format;
};

class CreateTextureError : public std::runtime_error
{
public:
    inline CreateTextureError(const char* message) noexcept : std::runtime_error(message) {}
    inline CreateTextureError(const std::string& message) noexcept : std::runtime_error(message) {}
};
