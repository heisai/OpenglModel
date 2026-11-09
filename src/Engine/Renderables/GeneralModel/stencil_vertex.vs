#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 instanceMatrix;
uniform bool instanced;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
    vec3 outlinePos = instanced ? aPos * 1.025 : aPos;
    gl_Position = projection * view * model * (instanced ? instanceMatrix : mat4(1.0)) * vec4(outlinePos, 1.0);
}