#ifndef LIGHT_STRUCTS_GLSL
#define LIGHT_STRUCTS_GLSL

// 光源类型常量
// 0 = 方向光（Directional）：无衰减，Position 当作方向使用
// 1 = 点光源（Point）       ：有距离衰减
// 2 = 聚光灯（Spot）        ：有距离衰减 + 锥角裁剪
struct LightInfo
{
    int  Type;        // 光源类型：0=方向光 1=点光源 2=聚光灯
    vec3 Position;    // 点光源/聚光灯：世界空间位置；方向光：光线方向（指向光源）
    vec3 Direction;   // 聚光灯朝向（方向光也可复用此字段）
    vec3 La;          // 环境光强度
    vec3 Ld;          // 漫反射光强度
    vec3 Ls;          // 镜面反射光强度
    // 点光源/聚光灯衰减系数： att = 1 / (Kc + Kl*d + Kq*d²)
    float Kc;         // 常数项（通常 = 1.0）
    float Kl;         // 线性项（如 0.09）
    float Kq;         // 二次项（如 0.032）
    // 聚光灯锥角（cos 值，避免每帧 acos）
    float CutOff;     // 内锥角 cos 值，如 cos(12.5°)
    float OuterCutOff;// 外锥角 cos 值，如 cos(17.5°)
};
uniform LightInfo Light;

struct MaterialInfo
{
    vec3  Ka;         // 环境反射系数
    vec3  Kd;         // 漫反射系数
    vec3  Ks;         // 镜面反射系数
    float Shininess;  // 高光指数
};
uniform MaterialInfo Material;

uniform vec3 viewPos;

// ── 通用衰减计算（方向光直接返回 1.0）────────────────────────
float calcAttenuation(vec3 fragPos)
{
    if (Light.Type == 0) return 1.0; // 方向光：无衰减
    float d = length(Light.Position - fragPos);
    return 1.0 / (Light.Kc + Light.Kl * d + Light.Kq * d * d);
}

// ── 通用光照方向（方向光用 Position 字段存方向）──────────────
vec3 calcLightDir(vec3 fragPos)
{
    if (Light.Type == 0)
        return normalize(Light.Position); // 方向光：直接是方向向量
    return normalize(Light.Position - fragPos); // 点/聚光：指向光源
}

// ── 聚光灯强度（非聚光灯直接返回 1.0）────────────────────────
float calcSpotIntensity(vec3 lightDir)
{
    if (Light.Type != 2) return 1.0;
    float theta   = dot(lightDir, normalize(-Light.Direction));
    float epsilon = Light.CutOff - Light.OuterCutOff;
    return clamp((theta - Light.OuterCutOff) / epsilon, 0.0, 1.0);
}

#endif // LIGHT_STRUCTS_GLSL
