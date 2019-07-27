#version 330

layout(location = 0) out vec4 out_color1;
layout(location = 1) out vec4 out_color2;

uniform sampler2D U_MainTexture;
varying vec2 V_Texcoord;

void main()
{
    gl_FragColor = vec4(1.0);
    // gl_FragColor = texture2D(U_MainTexture,V_Texcoord);
}