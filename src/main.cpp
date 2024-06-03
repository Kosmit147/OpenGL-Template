#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>

#include "types.h"
#include "gl/vertex_array.h"

static constexpr std::string_view window_title = "Example";
static constexpr int window_width = 800;
static constexpr int window_height = 600;

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

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title.data(), NULL, NULL);

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

	VertexArray vao;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw here

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
