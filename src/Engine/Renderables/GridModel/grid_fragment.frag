#version 450 core

// ── 公共库：单光源结构体（与 ShaderModel 保持一致）────────────
#include "common/light_structs.glsl"

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
    vec3 baseColor = texture(floorTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 s      = normalize(Light.Position - fs_in.FragPos);  // 光源方向（点光源）
    vec3 v      = normalize(viewPos - fs_in.FragPos);
    vec3 h      = normalize(s + v);

    vec3  ambient  = Light.La * baseColor;
    float sDotN    = max(dot(normal, s), 0.0);
    vec3  diffuse  = Light.Ld * sDotN * baseColor;
    float spec     = pow(max(dot(normal, h), 0.0), 64.0);
    vec3  specular = Light.Ls * spec;

    vec3 color = ambient + diffuse + specular;

    if (gamma)
        color = pow(color, vec3(1.0 / 2.2));        // 线性空间 → sRGB 输出

    FragColor = vec4(color, 1.0);
}