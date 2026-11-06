#ifndef BLINNPHONG_LIGHTING_GLSL
#define BLINNPHONG_LIGHTING_GLSL

// ============================================================
// 多光源 Blinn-Phong 框架（GridModel 使用）
// 修改光照算法（增加阴影、IBL 等）只需改这一处
// ============================================================

#include "common/surface_inputs.glsl"

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;

// 前向声明：各模型的 .frag 必须实现此函数，填写表面属性
void getSurface(out SurfaceInputs surface);

vec3 _calcBlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    vec3  lightDir   = normalize(lightPos - fragPos);
    float diff       = max(dot(lightDir, normal), 0.0);
    vec3  diffuse    = diff * lightColor;

    vec3  viewDir    = normalize(viewPos - fragPos);
    vec3  halfwayDir = normalize(lightDir + viewDir);
    float spec       = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3  specular   = spec * lightColor;

    float dist       = length(lightPos - fragPos);
    float att        = 1.0 / (gamma ? dist * dist : dist);
    return (diffuse + specular) * att;
}

vec4 evaluateLighting()
{
    SurfaceInputs surface;
    getSurface(surface);

    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 4; ++i)
    {
        lighting += _calcBlinnPhong(surface.normal, surface.fragPos,
                                    lightPositions[i], lightColors[i]);
    }

    vec3 color = surface.baseColor * lighting;
    if (gamma)
    {
     color = pow(color, vec3(1.0 / 2.2));
    }
       

    return vec4(color, surface.alpha);
}

#endif // BLINNPHONG_LIGHTING_GLSL
