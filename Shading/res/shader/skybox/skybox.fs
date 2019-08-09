
uniform samplerCube U_MainTexture;
varying vec3 V_Texcoord;

void main()
{
    gl_FragColor = textureCube(U_MainTexture,V_Texcoord);
}