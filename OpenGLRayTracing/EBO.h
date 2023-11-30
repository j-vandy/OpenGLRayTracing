#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	// reference ID of the Element Buffer Object (EBO)
	GLuint ID;
	// constructor creates EBO and links it to indices
	EBO(GLuint* indices, GLsizeiptr size);

	// binds the EBO
	void Bind();
	// unbinds the EBO
	void Unbind();
	// deletes the EBO
	void Delete();
};

#endif
