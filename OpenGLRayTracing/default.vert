// vertex Shader source code
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float scale;

out vec3 color;
out vec2 texCoord;

void main()
{
	color = aColor;
	texCoord = aTexCoord;
	gl_Position = vec4(vec3(aPos.x, aPos.y, aPos.z) * scale, 1.0);
}
