
uniform sampler2D U_MainTexture;
uniform sampler2D U_SecondTexture;

varying vec2 V_Texcoord;

void main()
{
    vec3 src = texture2D(U_MainTexture,V_Texcoord).rgb;
    vec3 dst = texture2D(U_SecondTexture,V_Texcoord).rgb;
    vec3 blend = abs(src - dst);
    gl_FragColor = vec4(blend,1.0);
}