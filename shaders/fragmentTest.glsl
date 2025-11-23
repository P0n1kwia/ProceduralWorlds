#version 330 core
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D text;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0,1.0,1.0,1.0);

}