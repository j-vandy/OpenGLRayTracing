#include "VAO.h"

// constructor that generates a single VAO and obtains its ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// links a VBO to the VAO using a given layout
void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	// binds the Vertex Buffer Object (VBO)
	VBO.Bind();
	// specifies the format of the VBO
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, (GLsizei) stride, offset);
	// enables the corresponding attribute to the VBO
	glEnableVertexAttribArray(layout);
	// unbinds the VBO
	VBO.Unbind();
}

// binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// unbinds the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
