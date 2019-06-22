attribute vec3 vertex;
attribute vec2 texcoord;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
varying vec2 V_Texcoord;

void main()
{
    V_Texcoord = texcoord;
    gl_Position = M*V*P*vec4(vertex,1.0);
}