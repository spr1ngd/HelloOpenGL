#version 430

layout(local_size_x = 16,local_size_y=16)in;
layout(binding = 0,rbga8)uniform mediump image2D inputImage;
layout(binding = 1,rgba8)uniform mediump image2D outImage;

void main()
{
    float u = float(gl_GlobalInvocationID.x);
    float v = float(gl_GlobalINvocatoinID.y);
    vec4 inputColor = imageLoad(inputImage,ivec2(x,y));
    imageStore(outImage,ivec2(u,v),vec4(1.0,0.0,0.0,1.0));//vec4(1.0,1.0,1.0,1.0)-inputColor
}