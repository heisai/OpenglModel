#version 450 core
// 项目的着色器加载器会在编译前展开此 include。
#include "../common/scene_lighting.glsl"

// 圆环的材质由 Shader::setMaterial 上传；Ka/Kd/Ks 分别对应三种反射系数。
struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
	sampler2D diffuseMap;
	sampler2D specularMap;
	bool hasDiffuseMap;
	bool hasSpecularMap;
};
uniform MaterialInfo Material;
uniform int flatShading; // 0：插值法线；1：面法线；2：面法线并丢弃部分片元
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

// C++ 通过 GLSL subroutine 在 Phong 和 Blinn-Phong 之间选择。
subroutine vec3 shadeModelType(vec3 position, vec3 normal);
subroutine uniform shadeModelType shadeModel;

vec3 shadeScene(vec3 position, vec3 normal, bool blinn)
{
	vec3 ambientColor = Material.hasDiffuseMap
		? texture(Material.diffuseMap, TexCoords).rgb : Material.Ka;
	vec3 diffuseColor = Material.hasDiffuseMap
		? texture(Material.diffuseMap, TexCoords).rgb : Material.Kd;
	vec3 specularColor = Material.hasSpecularMap
		? texture(Material.specularMap, TexCoords).rgb : Material.Ks;

	// 对每盏活动灯传入同一材质，逐灯计算并累加光照。
	vec3 result = vec3(0.0);
	for (int i = 0; i < lightCount; ++i)
		result += evaluateLight(lights[i], position, normal, ambientColor, diffuseColor,
								specularColor, Material.Shininess, blinn);
	return result;
}
subroutine(shadeModelType)
vec3 phongModel(vec3 position, vec3 normal) { return shadeScene(position, normal, false); }
subroutine(shadeModelType)
vec3 blinnModel(vec3 position, vec3 normal) { return shadeScene(position, normal, true); }

void main()
{
	// 步骤 1：按当前渲染模式决定是否丢弃网格状区域。
	bvec2 toDiscard = greaterThan(fract(FragPos.xy * 15.0), vec2(0.2));
	if (flatShading == 2 && all(toDiscard)) discard;
	// 步骤 2：选择插值法线或利用屏幕空间导数求面法线。
	vec3 normal = flatShading == 0 ? normalize(Normal)
								  : normalize(cross(dFdx(FragPos), dFdy(FragPos)));
	// 步骤 3：运行选中的光照模型，输出不透明颜色。
	FragColor = vec4(shadeModel(FragPos, normal), 1.0);
}