#ifndef SCENE_LIGHTING_GLSL
#define SCENE_LIGHTING_GLSL

// 与 C++ 的 Lighting::LightConfig::MaxLights 保持一致。
const int MAX_LIGHTS = 8;
struct LightInfo {
	int Type;           // 0：方向光，1：点光，2：聚光
	vec3 Position;      // 点光和聚光的位置，世界坐标
	vec3 Direction;     // 光线传播方向，世界坐标
	vec3 La;            // 环境光强度
	vec3 Ld;            // 漫反射光强度
	vec3 Ls;            // 镜面光强度
	vec3 Attenuation;   // 距离衰减的常数、一次和二次系数
	float CutOff;       // 聚光内锥角余弦值
	float OuterCutOff;  // 聚光外锥角余弦值
};
uniform LightInfo lights[MAX_LIGHTS];
uniform int lightCount;
uniform vec3 viewPos;

vec3 lightDirection(LightInfo light, vec3 fragPos)
{
	// 计算片元指向光源的方向；方向光的传播方向与之相反。
	return light.Type == 0 ? normalize(-light.Direction) : normalize(light.Position - fragPos);
}

float lightFactor(LightInfo light, vec3 fragPos, vec3 toLight)
{
	// 方向光不随距离衰减；点光和聚光使用 1/(Kc + Kl*d + Kq*d²)。
	if (light.Type == 0) return 1.0;
	float d = length(light.Position - fragPos);
	float attenuation = 1.0 / dot(light.Attenuation, vec3(1.0, d, d * d));
	if (light.Type != 2) return attenuation;
	// 聚光灯在内外锥之间平滑过渡，锥外强度为零。
	float theta = dot(toLight, normalize(-light.Direction));
	return attenuation * clamp((theta - light.OuterCutOff) / (light.CutOff - light.OuterCutOff), 0.0, 1.0);
}

vec3 evaluateLight(LightInfo light, vec3 position, vec3 normal,
				   vec3 ka, vec3 kd, vec3 ks, float shininess, bool blinn)
{
	// ka/kd/ks 是材质的环境、漫反射、镜面反射系数；shininess 控制高光集中程度。
	// 步骤 1：计算片元到光源和相机的方向，以及漫反射比例。
	vec3 toLight = lightDirection(light, position);
	vec3 toView = normalize(viewPos - position);
	float diffuse = max(dot(normal, toLight), 0.0);
	float specular = 0.0;
	// 步骤 2：受光面计算高光。Blinn 使用半程向量，Phong 使用反射向量。
	if (diffuse > 0.0) {
		vec3 direction = blinn ? normalize(toLight + toView) : reflect(-toLight, normal);
		specular = pow(max(dot(blinn ? normal : toView, direction), 0.0), shininess);
	}
	// 步骤 3：La*ka 为环境光，Ld*kd*diffuse 为漫反射，Ls*ks*specular 为镜面反射。
	// 三项相加后整体应用距离和聚光衰减；多灯结果由调用方累加。
	return (light.La * ka + light.Ld * kd * diffuse + light.Ls * ks * specular)
		* lightFactor(light, position, toLight);
}
#endif