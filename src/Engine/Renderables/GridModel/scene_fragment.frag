#version 450 core
// 项目的着色器加载器会在编译前展开公共光照代码。
#include "common/scene_lighting.glsl"

out vec4 FragColor;
in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;
uniform sampler2D floorTexture;
uniform bool gamma;

void main()
{
	// 步骤 1：采样地板纹理，作为材质的环境光和漫反射系数。
	vec3 baseColor = texture(floorTexture, fs_in.TexCoords).rgb;
	// 步骤 2：逐灯计算 Blinn-Phong；镜面反射系数为白色，高光指数为 64。
	vec3 color = vec3(0.0);
	for (int i = 0; i < lightCount; ++i)
		color += evaluateLight(lights[i], fs_in.FragPos, normalize(fs_in.Normal),
							   baseColor, baseColor, vec3(1.0), 64.0, true);
	// 步骤 3：需要时将线性光照结果做 gamma 编码，再输出。
	if (gamma) color = pow(color, vec3(1.0 / 2.2));
	FragColor = vec4(color, 1.0);
}