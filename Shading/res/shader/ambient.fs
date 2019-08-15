
uniform sampler2D U_MainTexture;
uniform vec4 U_LightColor;
uniform vec4 U_MaterialColor;

varying vec2 V_Texcoord;

void main()
{
    vec4 texColor = texture2D(U_MainTexture,V_Texcoord);
    gl_FragColor = U_LightColor * U_MaterialColor;
}