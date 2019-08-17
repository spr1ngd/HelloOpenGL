
varying vec3 V_VertexColor;

void main()
{
    gl_FragColor = vec4(V_VertexColor,1.0);
}