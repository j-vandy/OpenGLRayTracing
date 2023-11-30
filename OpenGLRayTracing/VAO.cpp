#include "VAO.h"

// constructor that generates a single VAO and obtains its ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// links a VBO to the VAO using a given layout
void VAO::LinkVBO(VBO VBO, GLuint layout)
{
	// binds the Vertex Buffer Object (VBO)
	VBO.Bind();
	// specifies the format of the VBO
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
