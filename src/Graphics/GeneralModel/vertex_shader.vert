#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out VS_OUT {
	vec3 Normal;
	vec3 FragPos;
	mat4 Projection;
	mat4 View;
	vec3 aNormal;
} vs_out;

void main()
{
     vs_out.FragPos = vec3(model *vec4(aPos,1.0f));
	 vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
	 vs_out.View= view;
	 vs_out.Projection= projection;
	 vs_out.aNormal = aNormal;
	 gl_Position = vec4(aPos, 1.0);


}