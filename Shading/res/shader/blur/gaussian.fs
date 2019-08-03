
uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    // gaussian kernel
    float kernel[9];
    kernel[6] = 1;kernel[7] = 2;kernel[8] = 1;
    kernel[3] = 2;kernel[4] = 4;kernel[5] = 2;
    kernel[0] = 1;kernel[1] = 2;kernel[2] = 1;

    vec4 color = vec4(0.0);
    int coreSize = 3;
    float texelOffset = 1/350.0;

    int kernelIndex = 0;
    for( int y = 0 ; y < coreSize;y++ ) 
    {
        for( int x = 0 ; x < coreSize;x++ )
        {
            vec2 uv = V_Texcoord + vec2( (-1+x)*texelOffset,(-1+y)*texelOffset );
            vec4 curColor = texture2D(U_MainTexture,uv);
            color += curColor * kernel[kernelIndex++];
        }        
    }
    color/=16;
    gl_FragColor = color;
}