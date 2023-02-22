#version 330 core
layout (location = 0) in vec3 aStartPos;
layout (location = 1) in vec3 aEndPos;
layout (location = 2) in vec3 aColor;

uniform mat4 view;
uniform mat4 projection;
out VS_OUT {
    vec3 color;
    vec4 startPos;
    vec4 endPos;
} vs_out;

void main()
{
    mat4 pv = projection * view;
    vs_out.startPos = pv * vec4(aStartPos,1.0);
    vs_out.endPos = pv * vec4(aEndPos,1.0);
    vs_out.color = aColor;
   
    
    gl_Position = vec4((aStartPos + aEndPos)/2.0,1.0);
}