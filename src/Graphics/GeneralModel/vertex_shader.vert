#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;





out VS_OUT {
     vec3 aPos;
	 vec3 aNormal;
} vs_out;

void main()
{
	vs_out.aPos = aPos;
	vs_out.aNormal = aNormal;
	gl_Position = vec4(aPos, 1.0);


}