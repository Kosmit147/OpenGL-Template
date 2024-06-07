#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/log.hpp"
#include "gl/index_buffer.hpp"
#include "gl/shader.hpp"
#include "gl/vertex_array.hpp"
#include "gl/vertex_buffer.hpp"
#include "window/gl_window.hpp"

static constexpr std::string_view window_title = "Example";
static constexpr int window_width = 800;
static constexpr int window_height = 600;

static constexpr GlWindowHints window_hints = {
    .gl_context_version_major = 4,
    .gl_context_version_minor = 3,
    .gl_profile = GLFW_OPENGL_CORE_PROFILE,
    .gl_debug_context = true,
};

int main()
{
    GlWindow window(window_title, window_width, window_height, &window_hints);
    window.set_vsync(true);

    log_notification("{}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    GLfloat vertices[] = {
        -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
    };

    GLushort indices[] = {
        0, 1, 2, 0, 2, 3,
    };

    VertexArray va;
    VertexBuffer vb(std::span<GLfloat>{ vertices }, GL_STATIC_DRAW);
    IndexBuffer ib(std::span<GLushort>{ indices }, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    Shader shader("shaders/basic.vert", "shaders/basic.frag");

    while (!window.should_close())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        window.swap_buffers();
        window.poll_events();
    }
}
