#include "EBO.h"

// constructor creates EBO and links it to indices
EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	// generate a single buffer object and obtain its ID
	glGenBuffers(1, &ID);
	// bind the generated buffer object as an Element Array Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// fill the EBO with the array of indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

// binds the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// unbinds the EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}