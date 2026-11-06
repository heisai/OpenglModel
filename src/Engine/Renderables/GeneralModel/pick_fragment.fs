#version 330 core
out vec4 FragColor;

uniform vec3 pickColor;

void main() 
{
    FragColor = vec4(pickColor, 1.0);
   //FragColor = vec4(2.0, 8.0, 0.0, 1.0);

}