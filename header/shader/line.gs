#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 color;
    vec4 startPos;
    vec4 endPos;
} gs_in[];

out vec3 fColor;

void main() {    
   
    fColor = gs_in[0].color; 
    gl_Position = gs_in[0].startPos ;
    EmitVertex();   
    gl_Position = gs_in[0].endPos ;
    EmitVertex();
    EndPrimitive();
}