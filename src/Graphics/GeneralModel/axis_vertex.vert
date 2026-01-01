#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
    vec3 position;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 只传递原始位置
    vs_out.position =  vec4(aPos, 1.0).xyz;
    gl_Position =  vec4(aPos, 1.0);
}