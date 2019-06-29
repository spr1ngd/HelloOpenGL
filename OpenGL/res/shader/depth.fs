uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    float depth = pow(texture2D(U_MainTexture,V_Texcoord).r,16.0);
    gl_FragColor = vec4(vec3(depth),1.0);
}