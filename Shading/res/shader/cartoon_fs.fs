
uniform vec4 U_AmbientColor;

uniform vec3 U_LightPos;
uniform vec4 U_LightColor;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

uniform vec4 U_SpecularColor;
uniform vec4 U_SpecularMaterial;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    // Light Direction
    vec3 L = normalize(U_LightPos);
    // Normal Direction In world Pos
    vec3 N = normalize(V_Normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    vec4 diffuseColor;
    if( diffuseIntensity  < 0.5)
    {
        diffuseColor = vec4(0.1,0.1,0.1,1.0);
    }
    else
    {
        diffuseColor = vec4(0.6,0.6,0.6,1.0);
    }

    // specular color
    vec3 reflectDir = normalize(reflect(-L,N));
    vec3 viewDir = normalize(U_LightPos-V_WorldPos.xyz);
    float specular = pow(max(0.0,dot(reflectDir,viewDir)),32);
    vec4 specularColor;
    if( specular < 0.5 )
    {
        specularColor = vec4(0.0,0.0,0.0,1.0)
    }
    else
    {
        specularColor = vec4(1.0,1.0,1.0,1.0);
    }
    gl_FragColor = U_AmbientColor + diffuseColor + specularColor;
}