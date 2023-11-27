#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	glfwInit();

	// tell GLFW what version of OpenGL we are using
	int const OPENGL_VERSION = 4;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION);
	// tell GLFW to use CORE profile - modern functions only
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a GLFW window
	int window_width = 800;
	int window_height = 800;
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGLRayTracing", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// set window as current context
	glfwMakeContextCurrent(window);

	// load GLAD so it configures to OpenGL
	gladLoadGL();

	// set viewport
	glViewport(0, 0, window_width, window_height);

	// set clear color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// clear back buffer
	glClear(GL_COLOR_BUFFER_BIT);
	// swap back buffer with the front buffer
	glfwSwapBuffers(window);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		// takes care of GLFW events
		glfwPollEvents();
	}

	// destroy GLFW window & GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}