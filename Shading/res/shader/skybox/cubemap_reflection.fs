
uniform samplerCube U_MainTexture;
uniform vec3 U_CameraPos;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    vec3 eyeDir = vec3(V_WorldPos.xyz - U_CameraPos);
    V_Normal = normalize(V_Normal);
    vec3 reflectorDir = reflect(V_Normal,eyeDir);
    gl_FragColor = textureCube(U_MainTexture,V_Normal); 
}