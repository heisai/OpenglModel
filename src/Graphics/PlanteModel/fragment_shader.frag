#version 330 core
out vec4 FragColor;
in vec3 Normal;  
in vec3 FragPos; 


struct Material
{
	vec3 ambient;	//环境光
	vec3 diffuse;	//漫反射
	vec3 specular; //镜面反射
	float shininess;//高光系数
};
struct Light
{
	vec3 positiom; //位置
	vec3 ambient;	//环境光
	vec3 diffuse;	//漫反射
	vec3 specular; //镜面反射
};

 
  
uniform vec3 viewPos; 
uniform Material material;
uniform Light light;
void main()
{

		// ambient
		vec3 ambient = material.ambient * light.ambient;
		
		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.positiom - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse* (diff * material.diffuse);
		
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular *( spec * material.specular);  
		
		vec3 result = (ambient + diffuse + specular) *2.0 ;
		FragColor = vec4(result, 1.0);
		
} 