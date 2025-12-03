#version 460 core
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;


uniform vec3 lightDir;
uniform vec3 lightColor;


void main()
{
	
 
 vec3 branchColor = vec3(0.8,0.5,0.2);
    float ambientStrength = 0.2;
    vec3 ambient = lightColor * ambientStrength;

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*lightColor;
    
    
    vec3 finalColor = (ambient + diffuse) * branchColor;

    FragColor = vec4(finalColor, 1.0);

}

