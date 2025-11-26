#version 460 core
in vec3 Normal;
in vec2 TexCoords;
in float Height;
in vec3 FragPos;

out vec4 FragColor;

uniform float maxMeshHeight;
uniform vec3 lightDir;
uniform vec3 lightColor;

vec3 colorWater = vec3(0.0, 0.2, 0.8);  
vec3 colorSand  = vec3(0.9, 0.8, 0.6); 
vec3 colorGrass = vec3(0.1, 0.6, 0.1);  
vec3 colorRock  = vec3(0.4, 0.4, 0.4);  
vec3 colorSnow  = vec3(1.0, 1.0, 1.0);


vec3 CalculateHeightMap(float h);
void main()
{
	float h = Height / maxMeshHeight; 
    vec3 heightColor = CalculateHeightMap(h);

    float ambientStrength = 0.2;
    vec3 ambient = lightColor * ambientStrength;

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*lightColor;
    
    
    vec3 finalColor = (ambient + diffuse)*heightColor;

    FragColor = vec4(finalColor, 1.0);

}
vec3 CalculateHeightMap(float h)
{
    vec3 finalColor;
 if (h < 0.1) {
    
        float blend = smoothstep(0.0, 0.1, h); 
        finalColor = mix(colorWater, colorSand, blend);
    }
    else if (h < 0.4) {
        
        float blend = smoothstep(0.1, 0.4, h);
        finalColor = mix(colorSand, colorGrass, blend);
    }
    else if (h < 0.75) {
        
        float blend = smoothstep(0.4, 0.75, h);
        finalColor = mix(colorGrass, colorRock, blend);
    }
    else {
        
        float blend = smoothstep(0.75, 1.0, h);
        finalColor = mix(colorRock, colorSnow, blend);
    }
    return finalColor;
}
