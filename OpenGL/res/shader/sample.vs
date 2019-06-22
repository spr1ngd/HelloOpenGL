attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;
 
varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_Texcoord;

void main()
{
    V_Normal = mat3(NM)*normal;
    V_WorldPos = M*vec4(vertex,1.0);
    V_Texcoord = texcoord;
    gl_Position = P*V*M*vec4(vertex,1.0);
}