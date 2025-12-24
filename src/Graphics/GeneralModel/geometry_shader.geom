#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform float time;
uniform bool geom_flag;


in VS_OUT {
	vec3 Normal;
	vec3 FragPos;
	mat4 Projection;
	mat4 View;
    vec3 aNormal;
} gs_in[];

out vec3 Normal;  
out vec3 FragPos; 

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 1.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) ; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}
void main() {    
        vec3 normal = GetNormal();
        normal =  gs_in[0].Normal;
	   vec4  position = gs_in[0].Projection * gs_in[0].View * vec4(gs_in[0].FragPos, 1.0);
       FragPos = vec3(explode(vec4(gs_in[0].FragPos, 1.0),normal));
       gl_Position = explode(position,normal);
        EmitVertex();
  normal =  gs_in[0].Normal;
      position = gs_in[1].Projection * gs_in[1].View * vec4(gs_in[1].FragPos, 1.0);
  FragPos = vec3(explode(vec4(gs_in[1].FragPos, 1.0),normal));
       gl_Position = explode(position,normal);
        EmitVertex();
  normal =  gs_in[0].Normal;
       position = gs_in[2].Projection * gs_in[2].View * vec4(gs_in[2].FragPos, 1.0);
  FragPos = vec3(explode(vec4(gs_in[2].FragPos, 1.0),normal));
       gl_Position = explode(position,normal);
        EmitVertex();
        EndPrimitive();
    
}