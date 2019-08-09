
attribute vec3 vertex;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 U_EyePos;

varying vec3 V_Texcoord;

void main()
{
    V_Texcoord = U_EyePos;
    gl_Position = P * M * V * vec4(vertex,1.0);
}