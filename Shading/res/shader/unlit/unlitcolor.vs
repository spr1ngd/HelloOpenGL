attribute vec3 vertex;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    gl_Position = P*V*M*vec4(vertex,1.0);
}