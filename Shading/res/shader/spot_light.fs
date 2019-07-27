
uniform vec4 U_AmbientColor;

uniform vec4 U_LightPos;
uniform vec4 U_LightColor;
uniform vec4 U_LightDirection;
uniform float U_SpotLightCutoff;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

uniform vec4 U_SpecularColor;
uniform vec4 U_SpecularMaterial;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    vec3 L = vec3(0.0);
    float attenuation = 0.0;
    // Light Direction
    if( U_LightPos.w == 0.0 )
    {
        L = normalize(U_LightPos.xyz);
    }
    else
    {
        vec3 lightDir = normalize(U_LightDirection.xyz);    
        float minCos = cos(U_SpotLightCutoff/180*3.14);
        L = normalize(V_WorldPos.xyz-U_LightPos.xyz);
        float c = max(0.0,dot(L,lightDir)); 
        if( c > minCos )
            attenuation = pow(c,U_LightDirection.w);
    }
    // Normal Direction In world Pos
    vec3 N = normalize(V_Normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    vec4 diffuseColor = U_LightColor * U_DiffuseColor * attenuation * 2.0;

    // specular color
    // vec3 reflectDir = normalize(reflect(-L,N));
    vec3 viewDir = normalize(U_LightPos.xyz-V_WorldPos.xyz);
    vec3 halfDir = normalize(viewDir+L);
    // vec4 reflectColor = U_SpecularColor * U_SpecularMaterial * pow(max(0.0,dot(reflectDir,viewDir)),32);
    float specularIntensity = pow( max(0.0,dot(halfDir,N)),32);
    vec4 reflectColor =  U_SpecularColor * U_SpecularMaterial * specularIntensity * attenuation;

    gl_FragColor = U_AmbientColor + diffuseColor;//+ reflectColor; 
}