#include "texture.hpp"

#include <stb_image.h>

#include "core/log.hpp"

auto Texture2DOptions::apply() const noexcept -> void
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizontal_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertical_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

[[nodiscard]] static inline auto get_format_from_channels(u32 channels) noexcept -> GLint
{
    switch (channels)
    {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    }

    std::unreachable();
}

Texture2D::Texture2D(const std::filesystem::path& path, bool generate_mipmap, const Texture2DOptions* options)
{
    if (!std::filesystem::exists(path)) [[unlikely]]
    {
        auto message = std::format("Can't create texture: Invalid file path: {}", path.string());
        log_error("{}", message);
        throw CreateTextureError{ message };
    }

    stbi_set_flip_vertically_on_load(true);
    auto path_str = path.string();

    int width, height, channels;
    auto data = stbi_load(path_str.c_str(), &width, &height, &channels, 0);

    if (!data) [[unlikely]]
    {
        auto message = std::format("Can't create texture: Can't read texture file: {}", path_str);
        log_error("{}", message);
        throw CreateTextureError{ message };
    }

    glGenTextures(1, &_id);
    bind();

    if (options)
    {
        options->apply();
    }
    else
    {
        Texture2DOptions default_opts;
        default_opts.apply();
    }

    _internal_format = get_format_from_channels(channels);

    glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, static_cast<GLsizei>(width),
                 static_cast<GLsizei>(height), 0, _internal_format, GL_UNSIGNED_BYTE,
                 reinterpret_cast<const void*>(data));

    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

auto Texture2D::bind(u32 slot) const noexcept -> void
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
}
