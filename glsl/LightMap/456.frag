#version 330 core
out vec4 FragColor;
struct Light
{
	vec3 positon;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material 
{
	sampler2D diffuse;
    sampler2D specular;  
	sampler2D emission;
    float shininess;
};

in vec3 Vertpos;			
in vec3 NoralVector;		//∑®œÚ¡ø



uniform Material MaterialData;
uniform Light LightData;
uniform float UTime;
in vec2 TexCoords;




void main()
{
	vec3 ambient = LightData.ambient *texture(MaterialData.diffuse, TexCoords).rgb;
	vec3 LightVector = -(LightData.positon - Vertpos);
	vec3 N = normalize(NoralVector);
	vec3 L = normalize(LightVector);
	float cosTheta  = dot(L,N);
	vec3 diffuse = LightData.diffuse * (texture(MaterialData.diffuse, TexCoords).rgb* max(cosTheta,0.0));


	vec3 V = normalize(vec3(0.0f, 0.0f, 3.0f) - Vertpos);
	vec3 R = normalize(reflect(-L,N));
	float cosPhi = dot(V,R);
	vec3 specular = LightData.specular * (texture(MaterialData.specular, TexCoords).rgb * pow(max(cosPhi,0.0),MaterialData.shininess));
	
	vec3 emission = texture(MaterialData.emission,TexCoords + 
	vec2(0.0,UTime)).rgb * floor(vec3(1.f) - texture(MaterialData.specular,TexCoords ).rgb);
	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result,1.0f);


}