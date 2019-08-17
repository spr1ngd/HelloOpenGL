
uniform samplerCube U_MainTexture;
uniform vec3 U_CameraPos;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    vec3 eyeDir = V_WorldPos.xyz - U_CameraPos;
    V_Normal = normalize(V_Normal);
    vec3 refractionDir = refract(eyeDir,V_Normal,1.0/1.3);
    gl_FragColor = textureCube(U_MainTexture,refractionDir);
}