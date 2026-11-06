#ifndef PHONG_SUBROUTINES_GLSL
#define PHONG_SUBROUTINES_GLSL

// ============================================================
// Phong / Blinn-Phong 着色模型（subroutine 运行时切换）
// 依赖：common/light_structs.glsl（LightInfo / MaterialInfo）
// 修改光照算法只需改这一处
// ============================================================

#include "common/light_structs.glsl"

subroutine vec3 shadeModelType(vec3 position, vec3 normal);
subroutine uniform shadeModelType shadeModel;

subroutine(shadeModelType)
vec3 phongModel(vec3 position, vec3 norm)
{
    vec3  s      = normalize(Light.Position - position);
    vec3  v      = normalize(viewPos - position);
    vec3  r      = reflect(-s, norm);
    vec3  ambient = Light.La * Material.Ka;
    float sDotN  = max(dot(norm, s), 0.0);
    vec3  diffuse = Light.Ld * Material.Kd * sDotN;
    vec3  spec    = Light.Ls * Material.Ks
                    * pow(max(dot(v, r), 0.0), Material.Shininess);
    return ambient + diffuse + spec;
}

subroutine(shadeModelType)
vec3 blinnModel(vec3 position, vec3 norm)
{
    vec3  s          = normalize(Light.Position - position);
    vec3  v          = normalize(viewPos - position);
    vec3  ambient    = Light.La * Material.Ka;
    float sDotN      = max(dot(norm, s), 0.0);
    vec3  diffuse    = Light.Ld * Material.Kd * sDotN;
    vec3  halfwayDir = normalize(s + v);
    vec3  spec       = Light.Ls * Material.Ks
                       * pow(max(dot(norm, halfwayDir), 0.0), Material.Shininess);
    return ambient + diffuse + spec;
}

#endif // PHONG_SUBROUTINES_GLSL
