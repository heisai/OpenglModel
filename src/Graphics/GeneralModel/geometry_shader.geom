#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform float time;
uniform bool geom_flag;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


in VS_OUT {
     vec3 aPos;
	 vec3 aNormal;
} gs_in[];

out vec3 Normal;  
out vec3 FragPos; 

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
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
        FragPos = vec3(model *gl_in[0].gl_Position);
	    Normal = mat3(transpose(inverse(model))) * gs_in[0].aNormal;  
        vec4 position;
	    position = projection * view * vec4(FragPos, 1.0);
         gl_Position = explode(position,normal);
        EmitVertex();
        FragPos = vec3(model *gl_in[1].gl_Position);
	    Normal = mat3(transpose(inverse(model))) * gs_in[1].aNormal;  
	  position = projection * view * vec4(FragPos, 1.0);
         gl_Position = explode(position,normal);
        EmitVertex();
        FragPos = vec3(model *gl_in[2].gl_Position);
	    Normal = mat3(transpose(inverse(model))) * gs_in[2].aNormal;  
	   position = projection * view * vec4(FragPos, 1.0);
         gl_Position = explode(position,normal);
        EmitVertex();
        EndPrimitive();
    
}