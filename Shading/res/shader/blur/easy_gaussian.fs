#version 120
uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    float texelOffset = 1/100.0;
    float weight[5] = float[](0.22,0.19,0.12,0.08,0.01);
    // vec4 color = vec4(0.0);
    vec4 color = texture2D(U_MainTexture,V_Texcoord) * weight[0];
    for( int i = 1 ;i < 5 ;i++ )
    {
        color += texture2D(U_MainTexture,V_Texcoord + vec2(texelOffset*i,0.0)) * weight[i];
        color += texture2D(U_MainTexture,V_Texcoord - vec2(texelOffset*i,0.0)) * weight[i];
    }

    for( int i = 1 ;i < 5 ;i++ )
    {
        color += texture2D(U_MainTexture,V_Texcoord + vec2(0.0,texelOffset*i)) * weight[i];
        color += texture2D(U_MainTexture,V_Texcoord - vec2(0.0,texelOffset*i)) * weight[i];
    }
    gl_FragColor = color / 2.0 * 1.1;
}