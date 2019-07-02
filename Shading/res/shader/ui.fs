uniform sampler2D U_MainTexture;

varying vec2 V_Texcoord;

void main()
{
    // gl_FragColor = texture2D(U_MainTexture,V_Texcoord);
	gl_FragColor = vec4(1.0);
}