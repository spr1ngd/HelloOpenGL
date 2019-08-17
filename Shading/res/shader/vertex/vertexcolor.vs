attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

varying vec3 V_VertexColor;

void main()
{
    // V_VertexColor = vertex;
    V_VertexColor = vec3(texcoord.xy,0.0);
    gl_Position = P * V * M * vec4(vertex,1.0);
}