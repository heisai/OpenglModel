#version 450 core

// ── 公共库：光照结构体 + Phong/Blinn subroutine 实现 ──────────
#include "common/phong_subroutines.glsl"
#include "common/surface_inputs.glsl"

layout(location = 0) out vec4 FragColor;

uniform int flatShading;  // 0=平滑 1=平面 2=网格线

in vec3 Normal;
in vec3 FragPos;

// ── 表面描述：此模型只需填写几何信息，颜色由材质 uniform 决定 ──
void getSurface(out SurfaceInputs surface)
{
    // 网格线模式：丢弃网格内部片元
    const float scale = 15.0;
    bvec2 toDiscard = greaterThan(fract(FragPos.xy * scale), vec2(0.2));
    if (flatShading == 2 && all(toDiscard))
        discard;

    surface.fragPos   = FragPos;
    surface.alpha     = 1.0;
    surface.baseColor = vec3(1.0);  // 颜色由 Material.Kd 决定，此处不参与

    if (flatShading == 0)
        surface.normal = normalize(Normal);
    else
        // 屏幕空间导数计算真实面法线，实现平面着色
        surface.normal = normalize(cross(dFdx(FragPos), dFdy(FragPos)));
}

void main()
{
    SurfaceInputs surface;
    getSurface(surface);
    FragColor = vec4(shadeModel(surface.fragPos, surface.normal), 1.0);
}


