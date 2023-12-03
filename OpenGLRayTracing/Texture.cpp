#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum textureUnit, GLenum sampleType, GLenum format, GLenum pixelType)
{
	type = texType;

	// get texture byte data
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// create OpenGL texture with ID
	glGenTextures(1, &ID);

	// set active texture unit
	glActiveTexture(textureUnit);
	// bind the texture object ID reference to that texture unit
	glBindTexture(texType, ID);

	// set texture sampling
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, sampleType);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, sampleType);

	// set texture repetition to GL_REPEAT (uv coords are called st coords in OpenGL)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// link texture data to bound texture object reference
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// generate mip maps
	glGenerateMipmap(texType);

	// free bytes and unbind texture
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::LinkUni(Shader shader, const char* uniform, GLuint unit)
{
	// get sampler2D uniform variable ID
	GLuint samplerUniID = glGetUniformLocation(shader.ID, uniform);
	shader.Use();

	// set sampler uniform variable to texture unit
	glUniform1i(samplerUniID, unit);
}

void Texture::Bind()
{
	// bind the texture object
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
