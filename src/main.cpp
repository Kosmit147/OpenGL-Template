#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>

#include "gl/vertex_array.h"
#include "gl/vertex_buffer.h"
#include "gl/index_buffer.h"
#include "types.h"

static constexpr std::string_view window_title = "Example";
static constexpr int window_width = 800;
static constexpr int window_height = 600;

static constexpr std::string_view vertex_shader_src = R"(
#version 430 core

layout (location = 0) in vec4 inPosition;

void main()
{
	gl_Position = inPosition;
}
)";

static constexpr std::string_view fragment_shader_src = R"(
#version 430 core

out vec4 outColor;

void main()
{
	outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

static auto verify_shader_compilation(GLuint shader) -> bool
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cerr << "Shader compilation failed: " << info_log << std::endl;
    }

    return success;
}

static auto verify_program_linkage(GLuint program) -> bool
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLchar info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cerr << "Shader program linkage failed: " << info_log << std::endl;
    }

    return success;
}

static auto compile_shader(const char* shader_src, GLenum type) -> GLuint
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    return shader;
}

static auto create_shader(std::string_view vertex_src, std::string_view fragment_src)
    -> GLuint
{
    GLuint vertex_shader = compile_shader(vertex_src.data(), GL_VERTEX_SHADER);
    verify_shader_compilation(vertex_shader);
    GLuint fragment_shader = compile_shader(fragment_src.data(), GL_FRAGMENT_SHADER);
    verify_shader_compilation(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    verify_program_linkage(shader_program);

#ifndef _DEBUG
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
#endif

    glUseProgram(shader_program);
    return shader_program;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(window_width, window_height, window_title.data(), NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create a window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, window_width, window_height);

    GLfloat vertices[] = {
        -0.5f, 0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f,
         -0.5f, -0.5f,
    };

    u16 indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    VertexArray va;
    VertexBuffer vb(std::span{ vertices }, GL_STATIC_DRAW);
    IndexBuffer ib(std::span{ indices }, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    GLuint shader = create_shader(vertex_shader_src, fragment_shader_src);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
}
