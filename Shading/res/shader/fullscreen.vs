
attribute vec3 vertex;
attribute vec2 texcoord; 

varying vec2 V_Texcoord;

void main()
{
    vec4 position = vec4(vertex.x,vertex.y,vertex.z,1.0);
    V_Texcoord = vec2(position.x + 0.5,position.y + 0.5);
    // V_Texcoord = vec2(position.x + 1,position.y);
    gl_Position = position;
}