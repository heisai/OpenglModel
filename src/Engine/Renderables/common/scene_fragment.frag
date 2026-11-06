#version 330 core
// 项目的着色器加载器会在编译前展开公共光照代码。
#include "common/scene_lighting.glsl"

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

// 普通模型和星球的材质 uniform；三个颜色分别是不同光照项的反射系数。
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

void main()
{
	// 步骤 1：以 Phong 模型计算每盏灯，并累加该材质的反射结果。
	vec3 result = vec3(0.0);
	for (int i = 0; i < lightCount; ++i)
		result += evaluateLight(lights[i], FragPos, normalize(Normal), material.ambient,
								material.diffuse, material.specular, material.shininess, false);
	// 步骤 2：保留此渲染路径原有的亮度倍率，输出不透明颜色。
	FragColor = vec4(result * 2.0, 1.0);
}