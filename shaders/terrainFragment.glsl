#version 330 core
in vec3 Normal;
in vec2 TexCoords;
in float Height;
uniform sampler2D text;

out vec4 FragColor;

void main()
{
	if(Height <= 0.4)
		FragColor = vec4(0.4,0.4,0.4,1.0);
	if (Height<=0.6)
		FragColor = vec4(0.5,0.5,0.5,1.0);
	if (Height <= 0.8)
		FragColor = vec4(0.8,0.8,0.8,1.0);
	FragColor = vec4(1.0,1.0,1.0,1.0);

}