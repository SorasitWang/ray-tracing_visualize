
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
 
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCurOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 



in vec3 pos;
in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpaces[10];
out vec4 FragColor;
 
# define MAX_POINT_LIGHTS 4  

uniform int numPointLight;
// for now, just consider only point light
uniform samplerCube shadowMaps[MAX_POINT_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform vec3 lightPos[MAX_POINT_LIGHTS];
uniform Material material;
//uniform DirLight dirLight;
//uniform vec3 lightPos;  
uniform vec3 viewPos;
uniform float farPlane;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,int lightIndex);  
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
float ShadowCubeCalculation(vec4 fragPosLightSpace,samplerCube shadowMap,vec3 lightPos);


void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: directional lighting
    vec3 result = vec3(0);//= CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < numPointLight; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir,i);    
    // phase 3: spot light
    //result += calcspotlight(spotlight, norm, fragpos, viewdir);    

    
    
    FragColor = vec4(result, 1);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,int lightIndex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  *material.diffuse;
    vec3 diffuse  = light.diffuse  * diff *material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    float shadow = ShadowCubeCalculation(FragPosLightSpaces[lightIndex],shadowMaps[lightIndex],lightPos[lightIndex]);    
    return (ambient + (diffuse + specular))*(1-shadow);
} 

float ShadowCubeCalculation(vec4 fragPosLightSpace,samplerCube shadowCubeMap,vec3 _lightPos)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowCubeMap, projCoords.xyz).r; 
    // get depth of current fragment from light's perspective
    //float currentDepth = projCoords.z;
    vec3 fragToLight = FragPos - _lightPos;
	float currentDepth = length(fragToLight);
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(_lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    // check whether current frag pos is in shadow
    //return currentDepth - bias > closestDepth  ? 1.0 : 0.0;    
    // PCF
    int sampleRadius = 1;
    float shadow = 0.0;
    float texelSize = 1.0 / 1024.0f;
    for(int x = -sampleRadius; x <= sampleRadius; ++x)
    {
        for(int y = -sampleRadius; y <= sampleRadius; ++y)
        {
            for (int z=-sampleRadius;z<=sampleRadius;++z){
                float pcfDepth = texture(shadowCubeMap, fragToLight + vec3(x, y,z) * texelSize).r; 
                pcfDepth *= farPlane;
                shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;    
            }
        }    
    }
    shadow /= pow((sampleRadius*2+1),3);
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(currentDepth > farPlane)
        shadow = 0.0;
        
    return shadow;
}