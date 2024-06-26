#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/log.hpp"
#include "gl/index_buffer.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"
#include "gl/vertex_buffer.hpp"
#include "gl/vertex_buffer_layout.hpp"
#include "window/gl_window.hpp"

// TODO: OpenGL error reporting

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
    glm::vec2 tex_coords;
    glm::vec4 color;
};

int main()
{
    GlWindow window(window_title, window_width, window_height, &window_hints);
    window.set_vsync(true);
    window.set_resize_callback([](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); });

    log_notification("{}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    // clang-format off
    RectVertex vertices[] = {
        {{ -0.5f,  0.5f, }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
        {{  0.5f,  0.5f, }, { 1.0f, 1.0f }, { 0.5f, 1.0f, 0.7f, 1.0f }},
        {{  0.5f, -0.5f, }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }},
    };

    GLushort indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    // clang-format on

    VertexArray va;
    VertexBuffer vb(std::span{ vertices }, GL_STATIC_DRAW);
    IndexBuffer ib(std::span{ indices }, GL_STATIC_DRAW);
    bind_vertex_buffer_layout<RectVertex>();

    Shader shader("shaders/basic.vert", "shaders/basic.frag");
    Texture2D texture("res/emoji.png");

    while (!window.should_close())
    {
        double time = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT);

        shader.set_unif<GLfloat>("time", static_cast<GLfloat>(time));
        shader.set_unif<GLint>("sampler", 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        window.swap_buffers();
        window.poll_events();
    }
}
