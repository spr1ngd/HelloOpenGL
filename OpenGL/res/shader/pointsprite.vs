attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    gl_PointSize = 36.0;
    // TODO: PVM和MVP的区别???
    gl_Position = P*V*M*vec4(vertex,1.0);
}