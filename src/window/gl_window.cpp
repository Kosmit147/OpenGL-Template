#include "gl_window.hpp"

static bool glfw_initialized = false;
static bool glad_loaded = false;

u32 GlWindow::_window_count = 0;

[[nodiscard]] static inline auto init_glfw() -> bool
{
    if (!glfwInit())
        return false;

    glfw_initialized = true;
    return true;
}

static inline auto terminate_glfw() -> void
{
    glfwTerminate();
    glfw_initialized = false;
    glad_loaded = false;
}

[[nodiscard]] static inline auto load_glad() -> bool
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        return false;

    glad_loaded = true;
    return true;
}

GlWindow::GlWindow(std::string_view title, u32 width, u32 height, const GlWindowHints* hints)
{
    if (!glfw_initialized)
    {
        if (!init_glfw())
        {
            std::println(stderr, "Failed to initialize GLFW.");
            throw FailedToInitializeGlfw{};
        }
    }

    if (hints)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, hints->gl_context_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, hints->gl_context_version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, hints->gl_profile);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, hints->gl_debug_context);
    }

    _window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.data(), NULL, NULL);

    if (!_window)
    {
        if (_window_count == 0)
            terminate_glfw();

        std::println(stderr, "Failed to create a window.");
        throw FailedToCreateGlWindow{};
    }

    make_context_current();

    if (!glad_loaded)
    {
        if (!load_glad())
        {
            terminate_glfw();
            std::println(stderr, "Failed to load GLAD.");
            throw FailedToLoadGlad{};
        }
    }

    fill_viewport();
    _window_count++;
}

GlWindow::~GlWindow()
{
    glfwDestroyWindow(_window);
    _window_count--;

    if (_window_count == 0)
        terminate_glfw();
}

auto GlWindow::should_close() const noexcept -> bool
{
    return glfwWindowShouldClose(_window);
}

auto GlWindow::size() const noexcept -> GlWindowSize
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return { .width = static_cast<u32>(width), .height = static_cast<u32>(height) };
}

auto GlWindow::width() const noexcept -> u32
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return static_cast<u32>(width);
}

auto GlWindow::height() const noexcept -> u32
{
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    return static_cast<u32>(height);
}

auto GlWindow::make_context_current() const noexcept -> void
{
    glfwMakeContextCurrent(_window);
}

auto GlWindow::set_vsync(bool enabled) const noexcept -> void
{
    int interval = enabled ? 1 : 0;
    glfwSwapInterval(interval);
}

auto GlWindow::fill_viewport() const noexcept -> void
{
    auto window_size = size();
    set_viewport(0, 0, static_cast<GLsizei>(window_size.width), static_cast<GLsizei>(window_size.height));
}

auto GlWindow::set_viewport(GLint x, GLint y, GLsizei width, GLsizei height) const noexcept -> void
{
    glViewport(x, y, width, height);
}
