
#version 330 core


out vec4 FragColor;


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

void main()
{
    // Evaluate the lighting equation.
   vec3  LightIntensity = phongModel( FragPos, Normal );

  FragColor = vec4(LightIntensity, 1.0);

}