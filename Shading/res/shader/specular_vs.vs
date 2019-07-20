attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 NM;

uniform vec3 U_ViewPos;
uniform vec3 U_LightPos;
uniform vec4 U_LightColor;

uniform vec4 U_DiffuseColor;
uniform vec4 U_DiffuseMaterial;

uniform vec4 U_SpecularColor;
uniform vec4 U_SpecularMaterial;

varying vec4 V_VertexColor;
varying vec4 V_SpecularColor;

void main()
{ 
    // Light Direction
    vec3 L = normalize(U_LightPos);
    // Normal Direction In world Pos
    vec3 N = normalize(mat3(NM)*normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    V_VertexColor = U_LightColor * U_DiffuseColor * diffuseIntensity;

    // specular
    vec3 reflectDir = normalize(reflect(-L,N));
    vec3 worldPos = M*vec4(vertex,1.0);
    vec3 viewDir = normalize(U_ViewPos-worldPos);
    V_SpecularColor = U_SpecularColor * U_SpecularMaterial * pow(max(0.0,dot(viewDir,reflectDir)),16);

    gl_Position = P*V*worldPos;
}