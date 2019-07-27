
uniform vec4 U_AmbientColor;

uniform vec4 U_LightPos;
uniform vec4 U_LightColor;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

uniform vec4 U_SpecularColor;
uniform vec4 U_SpecularMaterial;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    vec3 L = vec3(0.0);

    float attenuation = 1.0;
    float constantFactor = 1.0;
    float linearFactor = 0.05;
    float expFactor = 0.06;

    // Light Direction
    if( U_LightPos.w == 0.0 )
    {
        L = normalize(U_LightPos.xyz);
    }
    else
    {
        // point light
        L = normalize(U_LightPos.xyz - V_WorldPos.xyz);
        float distance = length(L);
        attenuation = 1.0 /(constantFactor + linearFactor * distance + expFactor * distance * distance);
    }
    // Normal Direction In world Pos
    vec3 N = normalize(V_Normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    vec4 diffuseColor = U_LightColor * U_DiffuseColor * diffuseIntensity * attenuation;

    // specular color
    // vec3 reflectDir = normalize(reflect(-L,N));
    vec3 viewDir = normalize(U_LightPos.xyz-V_WorldPos.xyz);
    vec3 halfDir = normalize(viewDir+L);
    // vec4 reflectColor = U_SpecularColor * U_SpecularMaterial * pow(max(0.0,dot(reflectDir,viewDir)),32);
    float specularIntensity = pow( max(0.0,dot(halfDir,N)),128);
    vec4 reflectColor =  U_SpecularColor * U_SpecularMaterial * specularIntensity * attenuation;

    gl_FragColor = U_AmbientColor + diffuseColor + reflectColor;
}