#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>

#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"

// vertices of a square (vertex pos, vertex color, vertex uv coords)
GLfloat vertices[] =
{
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // lower left corner,  (0,0)
	1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // lower right corner, (1,0)
	-1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // upper left corner,  (0, 1)
	1.0f, 1.0f, 0.0f,    1.0f, 1.0f  // upper right corner, (1, 1)
};

GLuint indices[] =
{
	0, 1, 2, // lower left triangle
	2, 1, 3, // upper right triangle
};

// window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// texture dimensions
const int TEXTURE_WIDTH = 256;
const int TEXTURE_HEIGHT = 256;

void setTexturePixels(GLubyte* pixels)
{
	for (int i = 0; i < TEXTURE_WIDTH; i++)
	{
		for (int j = 0; j < TEXTURE_HEIGHT; j++)
		{
			// (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 - r^2) = 0
			// a = ray origin
			// b = ray direction
			// r = sphere radius
			// t = hit distance
			glm::vec4 color;
			glm::vec4 backgroundColor(0.07f, 0.13f, 0.17f, 1.0f);
			glm::vec4 sphereColor(1.0f, 0.0f, 1.0f, 1.0f);

			// calculations came from:
			// https://www.youtube.com/watch?v=v9vndyfk2U8&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=3

			float radius = 0.5f;

			// convert i,j (our x and y coords) to range -1->1
			int x = i / 256 - 1;
			int y = j / 256 - 1;

			glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
			glm::vec3 rayDirection(x, y, -1.0f);

			// terms for quadratic formula
			float a = glm::dot(rayDirection, rayDirection);
			float b = 2.0f * glm::dot(rayOrigin, rayDirection);
			float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

			// b^2 - 4ac (quadratic formula discriminant)
			float discriminant = b * b - 4.0f * a * c;

			if (discriminant > 0.0f)
			{
				color = sphereColor;
			}
			else
			{
				color = backgroundColor;
			}

			// each pixel has rgba values
			int index = (i * TEXTURE_WIDTH + j) * 4;
			pixels[index] = 255; // red channel
			pixels[index + 1] = 0; // green channel
			pixels[index + 2] = 255; // blue channel
			pixels[index + 3] = 255; // alpha channel
			 
			pixels[index] = color.r;
			pixels[index + 1] = color.g;
			pixels[index + 2] = color.b;
			pixels[index + 3] = color.a;
		}
	}
}

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLRayTracing", NULL, NULL);
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
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


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
	// aPos layout location = 0
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	// aTexCoord layout location = 1
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// generate pixel data
	GLubyte* pixels = new GLubyte[TEXTURE_WIDTH * TEXTURE_HEIGHT * 4];
	setTexturePixels(pixels);

	// create a texture
	Texture texture(pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_TEXTURE_2D, GL_TEXTURE0, GL_NEAREST, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.LinkUni(shaderProgram, "sampler", 0);

	// set clear color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		// clear back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// tell OpenGL which shader program to use
		shaderProgram.Use();

		// bind Texture
		texture.Bind();

		// bind VAO 
		VAO1.Bind();

		// draw the quad
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
	texture.Delete();
	shaderProgram.Delete();

	// delete GLFW window & GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}