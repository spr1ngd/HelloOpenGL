
uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    vec4 maxValue = vec4(0.0);
    int coreSize = 4;
    int halfCoreSize = coreSize / 2;
    float texelOffset = 1 / 100.0;
    for( int y = 0 ; y < coreSize;y++ )
    {
        for( int x = 0; x < coreSize ;x++ )
        {
            vec2 uv = V_Texcoord + vec2((-halfCoreSize + x)* texelOffset,(-halfCoreSize+y)* texelOffset);
            vec4 color = texture2D(U_MainTexture,uv);
            maxValue = max(maxValue,color);
        }
    }
    // gl_FragColor =vec4(texture2D(U_MainTexture,V_Texcoord).rgb,maxValue);
    gl_FragColor = maxValue;
}