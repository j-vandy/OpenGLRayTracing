#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
public:
	// reference ID of the Vertex Buffer Object (VBO)
	GLuint ID;
	// constructor creates VBO and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);

	// binds the VBO
	void Bind();
	// unbinds the VBO
	void Unbind();
	// deletes the VBO
	void Delete();
};

#endif
