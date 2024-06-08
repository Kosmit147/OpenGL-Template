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

struct RectVertex
{
    glm::vec2 position;
};

int main()
{
    GlWindow window(window_title, window_width, window_height, &window_hints);
    window.set_vsync(true);
    window.set_resize_callback([](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); });

    log_notification("{}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    // clang-format off
    RectVertex vertices[] = {
        {{ -0.5f,  0.5f, }},
        {{  0.5f,  0.5f, }},
        {{  0.5f, -0.5f, }},
        {{ -0.5f, -0.5f, }},
    };

    GLushort indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    // clang-format on

    VertexArray va;
    VertexBuffer vb(std::span{ vertices }, GL_STATIC_DRAW);
    IndexBuffer ib(std::span{ indices }, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    Shader shader("shaders/basic.vert", "shaders/basic.frag");
    GLint unif_loc = glGetUniformLocation(shader.id(), "time");

    while (!window.should_close())
    {
        double time = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(unif_loc, static_cast<GLfloat>(time));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        window.swap_buffers();
        window.poll_events();
    }
}
