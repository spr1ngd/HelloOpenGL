uniform vec4 U_AmbientColor;

uniform vec4 U_LightPos;
uniform vec4 U_LightColor;
uniform vec4 U_LightDirection;
uniform float U_SpotLightCutoff;
uniform float U_LightIntensity;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

uniform vec4 U_SpecularColor;
uniform vec4 U_SpecularMaterial;

// FOG SETTING
uniform vec4 U_FogColor;
uniform vec4 U_FogDistance;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec4 V_CameraPos;

float calculateLinearFog( float distance )
{
    float factor = (distance - U_FogDistance.x ) / (U_FogDistance.y - U_FogDistance.x);
    factor = clamp(factor,0.0,1.0);
    return factor;
}

void main()
{
    vec3 N = normalize(V_Normal);
    vec3 L = vec3(0.0);
    float attenuation = 0.0;

    // point light setting
    float constantFactor = 1.0;
    float linearFactor = 0.05;
    float expFactor = 0.1;
    
    if( U_LightPos.w == 0.0 ) // direction light
    {
        attenuation = 1.0;
        L = normalize(U_LightPos.xyz);
    }
    else
    {
        if( U_SpotLightCutoff != 0.0) // spot light
        {
            attenuation = 0.0;
            vec3 lightDir = normalize(U_LightDirection.xyz);    
            float minCos = cos(U_SpotLightCutoff/180*3.14);
            L = normalize(V_WorldPos.xyz-U_LightPos.xyz);
            float c = max(0.0,dot(L,lightDir)); 
            if( c > minCos )
                attenuation = pow(c,U_LightDirection.w) * U_LightIntensity;
            L = -L;
        }
        else // point light
        {
            attenuation = 0.0;
            L = normalize(U_LightPos.xyz - V_WorldPos.xyz);
            float dis = length(L);
            attenuation = 1.0 / (constantFactor + linearFactor*dis + expFactor * dis * dis);
        }
    }
    float diffuseIntensity = max(0.0,dot(L,N));
    vec4 diffuseColor = U_LightColor * U_DiffuseColor * diffuseIntensity * attenuation;

    // specular color
    // vec3 reflectDir = normalize(reflect(-L,N));
    vec3 viewDir = normalize(U_LightPos.xyz-V_WorldPos.xyz);
    vec3 halfDir = normalize(viewDir+L);
    // vec4 reflectColor = U_SpecularColor * U_SpecularMaterial * pow(max(0.0,dot(reflectDir,viewDir)),32);
    float specularIntensity = pow( max(0.0,dot(halfDir,N)),64);
    vec4 reflectColor =  U_SpecularColor * U_SpecularMaterial * specularIntensity * attenuation;

    float fogAlpha = calculateLinearFog(abs(V_CameraPos.z / V_CameraPos.w));
    gl_FragColor = vec4(mix((U_AmbientColor + diffuseColor + reflectColor).rgb,U_FogColor.rgb,fogAlpha),1.0); 
}