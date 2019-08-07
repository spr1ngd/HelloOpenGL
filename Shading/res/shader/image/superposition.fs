
uniform sampler2D U_MainTexture;
uniform sampler2D U_SecondTexture;

varying vec2 V_Texcoord;

void main()
{
    vec3 src = texture2D(U_MainTexture,V_Texcoord).rgb;
    vec3 dst = texture2D(U_SecondTexture,V_Texcoord).rgb;

    vec3 lumCoeff = vec3(0.2125,0.7145,0.02721);
    float luminance = dot(dst,lumCoeff);
    vec3 blend = vec3(0.0);
    if( luminance < 0.45 )
    {
        blend = 2.0 * src * dst;
    }
    else if( luminance > 0.55 )
    {
        blend = vec3(1.0) - 2.0 * (vec3(1.0)-dst) * (vec3(1.0) - src);
    }
    else
    {
        vec3 color1 =2.0 * src * dst;
        vec3 color2 = vec3(1.0) - 2.0 * (vec3(1.0)-dst) * (vec3(1.0) - src);
        blend = mix(color1,color2,(luminance - 0.45)*10.0);
    }
 
    gl_FragColor = vec4(blend,1.0);
}