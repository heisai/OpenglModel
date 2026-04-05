#version 450 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
out vec3 FragPos;
out vec3 Normal;
void main()
{

     FragPos = vec3(ModelMatrix *vec4(VertexPosition,1.0f));
	 Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;  
     gl_Position = ProjectionMatrix * ViewMatrix *  ModelMatrix *vec4(VertexPosition, 1.0);
}      

