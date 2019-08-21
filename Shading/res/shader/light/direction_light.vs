attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;  

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{ 
    mat4 NM = V * transpose(inverse(M));
    V_Normal = normalize(NM*vec4(normal,1.0)).xyz; 
    V_WorldPos = M*vec4(vertex,1.0);
    gl_Position = P*V*V_WorldPos;
}