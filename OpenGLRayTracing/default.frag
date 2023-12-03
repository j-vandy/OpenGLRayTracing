// fragment shader source code
#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D sampler;

void main()
{

	FragColor = texture(sampler, texCoord);
}
