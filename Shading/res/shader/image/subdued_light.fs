
uniform sampler2D U_MainTexture;
uniform sampler2D U_SecondTexture;

varying vec2 V_Texcoord;

void main()
{
    vec3 src = texture2D(U_MainTexture,V_Texcoord).rgb;
    vec3 dst = texture2D(U_SecondTexture,V_Texcoord).rgb;
    vec3 blend = 2.0 * dst * src + dst * dst -2.0 * dst *dst*src;
    gl_FragColor = vec4(blend,1.0);
}