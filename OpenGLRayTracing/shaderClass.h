#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <iostream>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// reference ID of the Shader Program
	GLuint ID;
	// constructor for Shader Program; compiles and attaches vertex and fragment shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// uses the Shader Program
	void Use();
	// deletes the Shader Program
	void Delete();
};

#endif
