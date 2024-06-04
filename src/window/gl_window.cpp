#include "gl_window.hpp"

auto GlWindow::create(std::string_view title, u32 width, u32 height,
    const GlWindowHints* hints) noexcept -> std::optional<GlWindow>
{
    if (hints)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, hints->gl_context_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, hints->gl_context_version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, hints->gl_profile);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, hints->gl_debug_context);
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title.data(), NULL, NULL);

    if (!window)
        return {};

    glfwMakeContextCurrent(window);

    // have to be explicit with std::optional construction for RVO to work
    return std::optional<GlWindow>{ GlWindow{ window } };
}

auto GlWindow::should_close() const noexcept -> bool
{
    return glfwWindowShouldClose(_window);
}

auto GlWindow::size() const noexcept -> GlWindowSize
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return { .width = (u32)width, .height = (u32)height };
}

auto GlWindow::width() const noexcept -> u32
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return (u32)width;
}

auto GlWindow::height() const noexcept -> u32
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return (u32)height;
}

auto GlWindow::set_vsync(bool enabled) const noexcept -> void
{
    int interval = enabled ? 1 : 0;
    glfwSwapInterval(interval);
}

auto GlWindow::fill_viewport() const noexcept -> void
{
    auto window_size = size();
    set_viewport(0, 0, window_size.width, window_size.height);
}

auto GlWindow::set_viewport(GLint x, GLint y, GLsizei width, GLsizei height) const noexcept -> void
{
    glViewport(x, y, width, height);
}
