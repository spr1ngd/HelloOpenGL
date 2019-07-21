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

varying vec4 V_Color;

void main()
{ 
    // Light Direction
    vec3 L = normalize(U_LightPos);
    // Normal Direction In world Pos
    vec3 N = normalize(mat3(NM)*normal);

    float diffuseIntensity = max(0.0,dot(L,N));
    // V_Color = U_LightColor * U_DiffuseColor * diffuseIntensity;
    if( diffuseIntensity  < 0.5)
    {
        V_Color = vec4(0.1,0.1,0.1,1.0);
    }
    else
    {
        V_Color = vec4(0.6,0.6,0.6,1.0);
    }

    gl_Position = P*V*M*vec4(vertex,1.0);
}