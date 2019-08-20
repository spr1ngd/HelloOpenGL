attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

uniform vec3 U_LightPos;
uniform vec4 U_LightColor;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

varying vec4 V_VertexColor;

void main()
{ 
    // Light Direction
    vec3 L = normalize(U_LightPos);
    // Normal Direction In world Pos
    vec3 N = normalize(mat3(NM)*normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    V_VertexColor = U_LightColor * U_DiffuseColor * diffuseIntensity;
    gl_Position = P*V*M*vec4(vertex,1.0);
}