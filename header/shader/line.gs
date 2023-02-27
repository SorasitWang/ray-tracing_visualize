#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 color;
    vec4 startPos;
    vec4 endPos;
} gs_in[];

out vec3 fColor;
uniform float lineWidth;
void main() {    
   
   
    fColor = gs_in[0].color; 

    vec2 dir = normalize(gs_in[0].endPos.xy - gs_in[0].startPos.xy);
    vec2 normal = vec2(-dir.y,dir.x) * lineWidth/2.0;

    gl_Position = gs_in[0].startPos + vec4(normal, 0.0, 0.0);
    EmitVertex();   

    gl_Position = gs_in[0].startPos - vec4(normal, 0.0, 0.0);
    EmitVertex();

    gl_Position = gs_in[0].endPos + vec4(normal, 0.0, 0.0);
    EmitVertex();   

    gl_Position = gs_in[0].endPos - vec4(normal, 0.0, 0.0);
    EmitVertex();


    EndPrimitive();
}