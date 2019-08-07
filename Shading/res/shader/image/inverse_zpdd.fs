
uniform sampler2D U_MainTexture;
uniform sampler2D U_SecondTexture;

varying vec2 V_Texcoord;

void main()
{
    vec3 src = texture2D(U_MainTexture,V_Texcoord).rgb;
    vec3 dst = texture2D(U_SecondTexture,V_Texcoord).rgb;
    vec3 blend = vec3(1.0) - (vec3(1.0) -src ) * (vec3(1.0 )-dst);
    gl_FragColor = vec4(blend,1.0);
}