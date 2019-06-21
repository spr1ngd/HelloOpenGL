attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec4 V_Color;

void main()
{
    V_Color = vec4(1,1,1,1);
    gl_Position = P*V*M*vec4(vertex,1.0);
}