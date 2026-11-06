#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 128) out; // 3个三角锥 + 3个轴身圆柱近似，预留足够顶点
            
in VS_OUT {
    vec3 position;
} gs_in[];

out vec3 axisColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 轴的长度和箭头的大小
uniform float axisLength = 2.0f;
uniform float arrowHeight = 0.2f;
uniform float arrowRadius = 0.05f;
uniform float shaftRadius = 0.02f; // 轴身半径
uniform int shaftSegments = 12; // 圆柱近似的分段数

// 在轴方向上生成圆柱形状（用 triangle_strip 输出）
void createCylinderShaft(vec3 start, vec3 end, vec3 color) {
    vec3 dir = normalize(end - start);

    vec3 up = vec3(0.0, 1.0, 0.0);
    if (abs(dot(dir, up)) > 0.9) {
        up = vec3(1.0, 0.0, 0.0);
    }
    vec3 right = normalize(cross(dir, up));
    up = normalize(cross(right, dir));

    mat4 mvp = projection * view * model;

    // 输出环的顶点对，以 triangle_strip 形式连接
    int seg = max(3, shaftSegments);
    for (int i = 0; i <= seg; ++i) {
        float angle = 2.0 * 3.1415926 * float(i) / float(seg);
        vec3 offset = cos(angle) * right * shaftRadius + sin(angle) * up * shaftRadius;

        vec3 sPos = start + offset;
        vec3 ePos = end + offset;

        axisColor = color;
        gl_Position = mvp * vec4(sPos, 1.0);
        EmitVertex();

        axisColor = color;
        gl_Position = mvp * vec4(ePos, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}

void createPyramid(vec3 tip, vec3 direction, vec3 color) {
    vec3 dir = normalize(direction);
    // 计算圆锥底部中心
    vec3 baseCenter = tip - dir * arrowHeight;

    // 计算两个垂直向量用于构建底面
    vec3 up = vec3(0.0, 1.0, 0.0);
    if (abs(dot(dir, up)) > 0.9) {
        up = vec3(1.0, 0.0, 0.0);
    }

    vec3 right = normalize(cross(dir, up));
    up = normalize(cross(right, dir));

    // 三角形底面 3 个顶点
    vec3 base0 = baseCenter + arrowRadius * (cos(0.0) * right + sin(0.0) * up);
    float angle120 = 2.0 * 3.1415926 / 3.0;
    vec3 base1 = baseCenter + arrowRadius * (cos(angle120) * right + sin(angle120) * up);
    vec3 base2 = baseCenter + arrowRadius * (cos(2.0 * angle120) * right + sin(2.0 * angle120) * up);

    mat4 mvp = projection * view * model;

    // 三个侧面（尖端 -> basei -> base(i+1)）
    for (int i = 0; i < 3; ++i) {
        vec3 b0 = (i == 0) ? base0 : (i == 1) ? base1 : base2;
        vec3 b1 = (i == 0) ? base1 : (i == 1) ? base2 : base0;

        axisColor = color;
        gl_Position = mvp * vec4(tip, 1.0);
        EmitVertex();

        axisColor = color;
        gl_Position = mvp * vec4(b0, 1.0);
        EmitVertex();

        axisColor = color;
        gl_Position = mvp * vec4(b1, 1.0);
        EmitVertex();

        EndPrimitive();
    }

    // 底面（三角形），颜色稍暗
    axisColor = color * 0.7;
    gl_Position = mvp * vec4(base0, 1.0);
    EmitVertex();

    axisColor = color * 0.7;
    gl_Position = mvp * vec4(base2, 1.0);
    EmitVertex();

    axisColor = color * 0.7;
    gl_Position = mvp * vec4(base1, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main() {
    vec3 origin = gs_in[0].position;

    // 准备轴的方向与颜色
    vec3 xDir = vec3(1.0, 0.0, 0.0);
    vec3 yDir = vec3(0.0, 1.0, 0.0);
    vec3 zDir = vec3(0.0, 0.0, 1.0);

    vec3 xColor = vec3(1.0, 0.0, 0.0);
    vec3 yColor = vec3(0.0, 1.0, 0.0);
    vec3 zColor = vec3(0.0, 0.0, 1.0);

    vec3 xTip = origin + xDir * axisLength;
    vec3 yTip = origin + yDir * axisLength;
    vec3 zTip = origin + zDir * axisLength;

    // 绘制轴身圆柱（圆环分段）直到箭头底部
    createCylinderShaft(origin, xTip - normalize(xDir) * arrowHeight, xColor);
    createCylinderShaft(origin, yTip - normalize(yDir) * arrowHeight, yColor);
    createCylinderShaft(origin, zTip - normalize(zDir) * arrowHeight, zColor);

    // 绘制三角锥箭头
    createPyramid(xTip, xDir * arrowHeight, xColor);
    createPyramid(yTip, yDir * arrowHeight, yColor);
    createPyramid(zTip, zDir * arrowHeight, zColor);
}