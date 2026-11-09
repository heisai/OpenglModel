#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    vec4 worldPosition = ModelMatrix * vec4(aPos, 1.0);
    vs_out.FragPos = worldPosition.xyz;
    vs_out.Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
}