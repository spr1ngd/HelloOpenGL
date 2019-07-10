#version 330
attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord; 

varying vec2 V_Texcoord;

void main()
{
    vec4 position = vec4(vertex.x*2,vertex.y*2,vertex.z,1.0);
    V_Texcoord = position.xy;
    // gl_Position = vec4(vertex,1.0);
    gl_Position = position;
}