
uniform sampler2D U_MainTexture;
uniform sampler2D U_HDRTetxure;

varying vec2 V_Texcoord;

void main()
{
    // gl_FragColor = texture2D(U_MainTexture,V_Texcoord);
    // gl_FragColor = texture2D(U_HDRTetxure,V_Texcoord);
    gl_FragColor = texture2D(U_MainTexture,V_Texcoord) + texture2D(U_HDRTetxure,V_Texcoord);
}