/*#version 330 core

out vec4 FragColor;
struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct Material 
{
	sampler2D diffuse;
    sampler2D specular;  
	sampler2D emission;
    float shininess;
};

in vec3 Vertpos;			
in vec3 NoralVector;		//法向量
in vec2 TexCoords;


uniform Material MaterialData;
uniform Light LightData;
uniform float UTime;





void main()
{
	vec3 ambient = LightData.ambient *texture(MaterialData.diffuse, TexCoords).rgb;
	

	vec3 LightVector = -LightData.direction;
	vec3 N = normalize(NoralVector);
	vec3 L = normalize(LightVector);
	float diff  = max(dot(N,L),0.0);
	vec3 diffuse = LightData.diffuse * diff * texture(MaterialData.diffuse, TexCoords).rgb;


	vec3 V = normalize(vec3(0.0f, 0.0f, 3.0f) - Vertpos);
	vec3 R = normalize(reflect(-L,N));
	float spec  = pow(max(dot(V,R),0.0),MaterialData.shininess);
	vec3 specular = LightData.specular * spec * texture(MaterialData.specular, TexCoords).rgb ;
	


	vec3 emission = texture(MaterialData.emission,TexCoords + 
	vec2(0.0,UTime)).rgb * floor(vec3(1.f) - texture(MaterialData.specular,TexCoords ).rgb);
	
	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result,1.0f);


}*/


#version 330 core
out vec4 FragColor;
struct Light
{
	vec3 positon;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;	//常数
	float linear;	//线性
	float quadratic;//二次方
};

struct Material 
{
	sampler2D diffuse;
    sampler2D specular;  
	sampler2D emission;
    float shininess;
};

in vec3 Vertpos;			
in vec3 NoralVector;		//法向量



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


	vec3 V = normalize(vec3(0.0f, 0.0f, 6.0f) - Vertpos);
	vec3 R = normalize(reflect(-L,N));
	float cosPhi = dot(V,R);
	vec3 specular = LightData.specular * (texture(MaterialData.specular, TexCoords).rgb * pow(max(cosPhi,0.0),MaterialData.shininess));
	
	vec3 emission = texture(MaterialData.emission,TexCoords + 
	vec2(0.0,UTime)).rgb * floor(vec3(1.f) - texture(MaterialData.specular,TexCoords ).rgb);
	
	float distant = length(LightData.positon - Vertpos);
	float attenuation  = 1.0 / (LightData.constant + LightData.linear * distant + LightData.quadratic * distant * distant);


	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	//vec3 result = ambient + diffuse + specular + emission;
	vec3 result = ambient + diffuse + specular;


	FragColor = vec4(result,1.0f);


}
