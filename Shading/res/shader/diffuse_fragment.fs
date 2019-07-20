
uniform vec4 U_AmbientColor;

uniform vec3 U_LightPos;
uniform vec4 U_LightColor;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

varying vec3 V_Normal;

void main()
{
    // Light Direction
    vec3 L = normalize(U_LightPos);
    // Normal Direction In world Pos
    vec3 N = normalize(V_Normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    vec4 diffuseColor = U_LightColor * U_DiffuseColor * diffuseIntensity;
    gl_FragColor = U_AmbientColor + diffuseColor;
}