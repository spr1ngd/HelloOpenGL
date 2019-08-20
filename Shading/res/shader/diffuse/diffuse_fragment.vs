attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

varying vec3 V_Normal;

void main()
{ 
    V_Normal = mat3(NM)*normal;
    gl_Position = P*V*M*vec4(vertex,1.0);
}