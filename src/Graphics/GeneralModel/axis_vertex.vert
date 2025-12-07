#version 330 core

layout(location = 0) in vec3 aPos; //  ‰»Î∂•µ„Œª÷√

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
