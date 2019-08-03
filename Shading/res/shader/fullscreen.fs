
uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    vec4 color = texture2D(U_MainTexture,V_Texcoord); 
    gl_FragColor = color;
}
