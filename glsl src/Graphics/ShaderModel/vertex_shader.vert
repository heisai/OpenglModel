layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo {
    vec4 Position; // 光源在观察空间（眼坐标）中的位置
    vec3 La;       // 环境光强度
    vec3 Ld;       // 漫反射光强度
    vec3 Ls;       // 镜面反射光强度
};
//光照
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka;            // 环境反射系数
    vec3 Kd;            // 漫反射反射率
    vec3 Ks;            // 镜面反射反射率
    float Shininess;    // 镜面高光（光滑度）因子
};
//材质
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void getEyeSpace( out vec3 norm, out vec4 position )
{
    norm = normalize( NormalMatrix * VertexNormal);
    position = ModelViewMatrix * vec4(VertexPosition,1.0);
}

vec3 phongModel( vec4 position, vec3 norm )
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Ls * Material.Ks *
         pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

void main()
{
    vec3 eyeNorm;
    vec4 eyePosition;

    // 在观察空间中获取位置和法线
    getEyeSpace(eyeNorm, eyePosition);

    // 计算光照方程（Phong 模型）
    LightIntensity = phongModel( eyePosition, eyeNorm );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}