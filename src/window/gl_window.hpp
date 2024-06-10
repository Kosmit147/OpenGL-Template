#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

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
    using OnResizeCallback = void(*)(GLFWwindow*, int, int);

    // throws CreateGlWindowError
    GlWindow(std::string_view title, u32 width, u32 height, const GlWindowHints* hints = nullptr);
    virtual ~GlWindow() noexcept;

    GlWindow(const GlWindow& other) = delete;
    GlWindow(GlWindow&& other) = delete;

    [[nodiscard]] auto should_close() const noexcept -> bool;
    [[nodiscard]] auto size() const noexcept -> GlWindowSize;
    [[nodiscard]] auto width() const noexcept -> u32;
    [[nodiscard]] auto height() const noexcept -> u32;

    auto make_context_current() const noexcept -> void;
    auto set_resize_callback(OnResizeCallback callback) -> void;
    auto set_vsync(bool enabled) const noexcept -> void;
    auto fill_viewport() const noexcept -> void;
    auto set_viewport(GLint x, GLint y, GLsizei width, GLsizei height) const noexcept -> void;

    inline auto swap_buffers() const noexcept -> void { glfwSwapBuffers(_window); }
    inline auto poll_events() const noexcept -> void { glfwPollEvents(); }

private:
    GLFWwindow* _window;

    static u32 _window_count;
};

class CreateGlWindowError : public std::runtime_error
{
public:
    inline CreateGlWindowError(const char* message) noexcept : std::runtime_error(message) {}
    inline CreateGlWindowError(const std::string& message) noexcept : std::runtime_error(message) {}
};
