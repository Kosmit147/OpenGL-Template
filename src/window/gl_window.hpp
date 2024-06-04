#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <optional>
#include <string_view>

#include "types.hpp"

struct GlWindowHints
{
    i32 gl_context_version_major;
    i32 gl_context_version_minor;
    i32 gl_profile;
    bool gl_debug_context;
};

struct GlWindowSize
{
    u32 width;
    u32 height;
};

class GlWindow
{
public:
    [[nodiscard]] static auto create(std::string_view title, u32 width, u32 height,
        const GlWindowHints* hints = nullptr) noexcept -> std::optional<GlWindow>;

    [[nodiscard]] auto should_close() const noexcept -> bool;
    [[nodiscard]] auto size() const noexcept -> GlWindowSize;
    [[nodiscard]] auto width() const noexcept -> u32;
    [[nodiscard]] auto height() const noexcept -> u32;

    auto set_vsync(bool enabled) const noexcept -> void;
    auto fill_viewport() const noexcept -> void;
    auto set_viewport(GLint x, GLint y, GLsizei width, GLsizei height) const noexcept -> void;

    inline auto swap_buffers() const noexcept -> void { glfwSwapBuffers(_window); }
    inline auto poll_events() const noexcept -> void { glfwPollEvents(); }

protected:
    inline GlWindow(GLFWwindow* window) noexcept : _window(window){};

private:
    GLFWwindow* _window;
};
