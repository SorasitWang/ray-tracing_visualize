
#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform int numPointLight;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrixs[10];
out vec2 TexCoords;
out vec3 FragPos;  
out vec3 Normal;
out vec4 FragPosLightSpaces[10];

void main()
{
  
    gl_Position = projection *view* model * vec4(aPos, 1.0);

    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    for (int i=0;i<numPointLight;i++)
        FragPosLightSpaces[i] = lightSpaceMatrixs[i] * vec4(FragPos, 1.0);
}