
#version 430 core
layout( location = 0 ) out vec4 FragColor;
//引入子例程模块
subroutine vec3 shadeModelType(vec3 position, vec3 normal);
subroutine uniform shadeModelType shadeModel;

struct LightInfo {
    vec3 Position; // Light position in eye coords.
    vec3 La;       // Ambient light intensity
    vec3 Ld;       // Diffuse light intensity
    vec3 Ls;       // Specular light intensity
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;


uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos; 

uniform bool flatShading; // 平面着色开关
subroutine (shadeModelType)
vec3 phongModel( vec3 position, vec3 norm )
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(viewPos - position.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot(norm,s), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    spec = Light.Ls * Material.Ks *pow( max( dot(v,r), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}
subroutine (shadeModelType)
vec3 blinnModel(vec3 position,vec3 norm)
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(viewPos - position.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot(norm,s), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    vec3 halfwayDir = normalize(s+v);
    spec = Light.Ls * Material.Ks *pow( max( dot(norm,halfwayDir), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}
void main()
{
    //光照位置不对，导致正面和背面相反
    vec3 n;
    if(flatShading)
    {
        // 通过屏幕空间导数计算真实面法线，实现平面着色
        n = normalize(cross(dFdx(FragPos), dFdy(FragPos)));
    }
    else
    {
        n = normalize(Normal);
    }

   // vec3 norm = gl_FrontFacing ? -n : n;
  vec3 norm = n;
  vec3  LightIntensity = shadeModel(FragPos, norm);
  FragColor = vec4(LightIntensity, 1.0);

}