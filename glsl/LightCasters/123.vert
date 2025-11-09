#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

out vec2 TexCoords;
out vec3 Vertpos;			
out vec3 NoralVector;		//∑®œÚ¡ø
void main()
{
	

	Vertpos = vec3(model * vec4(aPos, 1.0f));
	
	NoralVector = mat3(transpose(inverse(model))) * normal;  
	TexCoords = aTexCoords;
	gl_Position = project * view *  vec4(Vertpos,1.0);

	
}