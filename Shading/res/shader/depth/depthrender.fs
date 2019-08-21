
uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    float depthColor = texture2D(U_MainTexture,V_Texcoord).r;
    // gl_FragColor = vec4(vec3(pow(depthColor,128.0)),1.0);
    gl_FragColor = texture2D(U_MainTexture,V_Texcoord);
}