#version 450 core

layout(location = 0) in vec3 position;
layout(location = 3) in mat4 instanceMatrix;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform bool instanced;

void main()
{
	gl_Position = lightSpaceMatrix * model
		* (instanced ? instanceMatrix : mat4(1.0)) * vec4(position, 1.0);
}
