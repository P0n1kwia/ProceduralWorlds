#version 460 core
out vec4 FragColor;
in vec3 TexCoords;

unform samplerCube skybox;

void main()
{
	FragColor = texture(skybox,texCoords);
}