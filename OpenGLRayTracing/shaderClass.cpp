#include "shaderClass.h"

// reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// constructor for Shader Program; compiles and attaches vertex and fragment shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// create vertex shader object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach vertex shader source to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// compile the vertex shader
	glCompileShader(vertexShader);
	// check for compilation errors
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// get error log
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER COMPILATION ERROR:\n" << infoLog << std::endl;

		// delete the vertex shader object
		glDeleteShader(vertexShader);
		return;
	}

	// create fragment shader object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// attach fragment shader source to the fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// compile the vertex shader
	glCompileShader(fragmentShader);
	// check for fragment shader compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// get error log
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER COMPILATION ERROR:\n" << infoLog << std::endl;

		// delete the vertex & frag shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return;
	}

	// create shader program object and get its reference
	ID = glCreateProgram();
	// attach the vertex shader to the shader program
	glAttachShader(ID, vertexShader);
	// attach the fragment shader to the shader program
	glAttachShader(ID, fragmentShader);
	// link all the shaders together into the shader program
	glLinkProgram(ID);
	// check for shader program compilation errors
	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// get error log
		char infoLog[512];
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM COMPILATION ERROR:\n" << infoLog << std::endl;

		// delete the vertex & frag & program shader objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(ID);
		return;
	}

	// delete the vertex shader object
	glDeleteShader(vertexShader);
	// delete the fragment shader object
	glDeleteShader(fragmentShader);
}

// uses the Shader Program
void Shader::Use()
{
	glUseProgram(ID);
}

// deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}
