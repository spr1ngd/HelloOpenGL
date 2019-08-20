
uniform vec4 U_AmbientColor;

varying vec4 V_VertexColor;

void main()
{
    gl_FragColor = U_AmbientColor + V_VertexColor;
}