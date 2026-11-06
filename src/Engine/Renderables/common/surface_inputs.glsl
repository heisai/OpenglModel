#ifndef SURFACE_INPUTS_GLSL
#define SURFACE_INPUTS_GLSL

// 每个模型的 getSurface() 填写此结构体，描述表面属性
// 光照算法本身无需关心具体是纹理还是纯色
struct SurfaceInputs
{
    vec3  baseColor;  // 表面颜色（纹理采样或材质颜色）
    vec3  normal;     // 世界空间归一化法线
    vec3  fragPos;    // 世界空间片元坐标
    float alpha;      // 透明度
};

#endif // SURFACE_INPUTS_GLSL
