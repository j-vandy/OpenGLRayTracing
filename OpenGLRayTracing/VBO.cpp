#include "VBO.h"

// constructor creates VBO and links it to vertices
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	// generate a single buffer object and obtain its ID
	glGenBuffers(1, &ID);
	// bind the generated buffer object as an Array Buffer
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// fill the VBO with the array of vertices
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
