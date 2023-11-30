#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"

// vertices of a square
GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,  // lower left corner
	0.5f, -0.5f, 0.0f, // lower right corner
	-0.5f, 0.5f, 0.0f, // upper left corner
	0.5f, 0.5f, 0.0f // upper right corner
};

GLuint indices[] =
{
	0, 1, 2, // lower left triangle
	2, 1, 3, // upper right triangle
};


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

	// set viewport of OpenGL in the Window
	glViewport(0, 0, window_width, window_height);


	// generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// generates Vertex Array Object
	VAO VAO1;
	// bind VAO1 to Vertex Array Object
	VAO1.Bind();

	// generate Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generate Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// link VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// set clear color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		// clear back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// tell OpenGL which shader program to use
		shaderProgram.Use();
		// bind VAO 
		VAO1.Bind();
		// draw the triangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap back buffer with the front buffer
		glfwSwapBuffers(window);

		// takes care of GLFW events
		glfwPollEvents();
	}

	// delete all objects we have created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// delete GLFW window & GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}