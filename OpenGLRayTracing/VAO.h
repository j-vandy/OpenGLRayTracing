#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
	// reference ID of the Vertex Array Buffer (VAO)
	GLuint ID;
	// constructor that generates a single VAO and obtains its ID
	VAO();

	// links a VBO to the VAO using a given layout
	void LinkVBO(VBO VBO, GLuint layout);
	// binds the VAO
	void Bind();
	// unbinds the VAO
	void Unbind();
	// deletes the VAO
	void Delete();
};

#endif