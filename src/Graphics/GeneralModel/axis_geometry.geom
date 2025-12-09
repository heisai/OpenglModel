#version 330 core
layout(points) in;               // 输入类型是点
layout(line_strip, max_vertices = 15) out;  // 输出线段
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 axisColor;

void drawAxis(vec3 origin, vec3 dir, vec3 color) {
    float length = 1.0;
    float lineWidth = 0.02;
    float arrowLength = 0.1;
    float arrowWidth = 0.05;

    // 计算两个垂直向量
    vec3 up = vec3(0.0, 1.0, 0.0);
    if (abs(dot(dir, up)) > 0.9) {
        up = vec3(0.0, 0.0, 1.0);
    }
    vec3 perp1 = normalize(cross(dir, up));
    vec3 perp2 = normalize(cross(dir, perp1));

    mat4 mvp = projection * view * model;

    // 1. 绘制线段（两个三角形，组成一个矩形）
    vec3 start1 = origin - perp1 * lineWidth / 2.0;
    vec3 start2 = origin + perp1 * lineWidth / 2.0;
    vec3 end1 = origin + dir * length - perp1 * lineWidth / 2.0;
    vec3 end2 = origin + dir * length + perp1 * lineWidth / 2.0;

    // 第一个三角形
    axisColor = color;
    gl_Position = mvp * vec4(start1, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(end1, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(start2, 1.0);
    EmitVertex();
    EndPrimitive();

    // 第二个三角形
    gl_Position = mvp * vec4(end1, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(end2, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(start2, 1.0);
    EmitVertex();
    EndPrimitive();

    // 2. 绘制箭头（四棱锥，4个三角形）
    vec3 arrowBase = origin + dir * length;
    vec3 arrowTip = arrowBase + dir * arrowLength;

    // 箭头底部的四个点
    vec3 base1 = arrowBase + perp1 * arrowWidth / 2.0 + perp2 * arrowWidth / 2.0;
    vec3 base2 = arrowBase + perp1 * arrowWidth / 2.0 - perp2 * arrowWidth / 2.0;
    vec3 base3 = arrowBase - perp1 * arrowWidth / 2.0 - perp2 * arrowWidth / 2.0;
    vec3 base4 = arrowBase - perp1 * arrowWidth / 2.0 + perp2 * arrowWidth / 2.0;

    // 三角形1
    gl_Position = mvp * vec4(arrowTip, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base1, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base2, 1.0);
    EmitVertex();
    EndPrimitive();

    // 三角形2
    gl_Position = mvp * vec4(arrowTip, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base2, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base3, 1.0);
    EmitVertex();
    EndPrimitive();

    // 三角形3
    gl_Position = mvp * vec4(arrowTip, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base3, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base4, 1.0);
    EmitVertex();
    EndPrimitive();

    // 三角形4
    gl_Position = mvp * vec4(arrowTip, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base4, 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(base1, 1.0);
    EmitVertex();
    EndPrimitive();
}
void main()
{
    // 轴的长度和箭头的大小
    float axisLength = 2.0f;
    float arrowSize = 0.1f;
    // 起点位置
    vec3 origin = gl_in[0].gl_Position.xyz;
    mat4 mvp = projection * view * model;
   axisColor = vec3(1.0, 0.0, 0.0);  // 红色
    gl_Position = mvp *vec4(origin + vec3(2.0, 0.0, 0.0), 1.0);
    EmitVertex();
    gl_Position = mvp *vec4(origin, 1.0); 
    EmitVertex();
    EndPrimitive();
 // 绘制 X 轴箭头（三角形）
    gl_Position = mvp* vec4(origin + vec3(axisLength - arrowSize, 0.0, arrowSize), 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(origin + vec3(axisLength, 0.0, 0.0), 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(origin + vec3(axisLength - arrowSize, 0.0, -arrowSize), 1.0);
    EmitVertex();
    EndPrimitive();
   


    axisColor = vec3(0.0, 1.0, 0.0);  // 绿色
    gl_Position = mvp *vec4(origin + vec3(0.0, 2.0, 0.0), 1.0); 
    EmitVertex();
    gl_Position = mvp *vec4(origin, 1.0); 
    EmitVertex();
  EndPrimitive();
    gl_Position = mvp * vec4(origin + vec3(arrowSize, axisLength - arrowSize, 0.0), 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(origin + vec3(0.0, axisLength, 0.0), 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(origin + vec3(-arrowSize, axisLength - arrowSize, 0.0), 1.0);
    EmitVertex();

   EndPrimitive();
    axisColor = vec3(0.0, 0.0, 1.0);  // 蓝色
    gl_Position = mvp *vec4(origin + vec3(0.0, 0.0, 2.0), 1.0); 
    EmitVertex();
    gl_Position = mvp *vec4(origin, 1.0); 
    EmitVertex();
  EndPrimitive();
// 绘制 Z 轴箭头（三角形）
    gl_Position = mvp * vec4(origin + vec3(arrowSize, 0.0, axisLength - arrowSize), 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(origin + vec3(0.0, 0.0, axisLength), 1.0);
    EmitVertex();
    gl_Position = mvp * vec4(origin + vec3(-arrowSize, 0.0, axisLength - arrowSize), 1.0);
    EmitVertex();
    EndPrimitive();


}
