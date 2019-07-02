#version 400
subroutine vec4 SurfaceColor();

uniform sampler2D U_MainTexture;
subroutine uniform SurfaceColor U_SurfaceColor;
varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_Texcoord;

subroutine (SurfaceColor) vec4 Ambient()
{
    vec4 materialColor = vec4(1,1,1,1.0);

    // light
    vec3 lightPos = vec3(10,10,10);
    vec4 lightColor = vec4(1, 1, 1, 1.0);
    vec3 L = normalize(lightPos);
    vec3 N = normalize(V_Normal);

    // ambient
    vec4 AMBIENT_COLOR = vec4(0.2,0.2,0.2,1.0);
    vec4 ambient = AMBIENT_COLOR * materialColor;
    return ambient;
}

subroutine (SurfaceColor) vec4 Diffuse()
{
    // model property
    vec4 materialColor = vec4(1,1,1,1.0);

    // light
    vec3 lightPos = vec3(10,10,10);
    vec4 lightColor = vec4(1, 1, 1, 1.0);
    vec3 L = normalize(lightPos);
    vec3 N = normalize(V_Normal);

    // ambient
    vec4 AMBIENT_COLOR = vec4(0.2,0.2,0.2,1.0);
    vec4 ambient = AMBIENT_COLOR * materialColor;

    // diffuse
    vec4 diffuseColor = vec4(0.8,0.8,0.8,1.0);
    vec4 diffuse = diffuseColor * lightColor * max(0.0,dot(L,N));
    return ambient + diffuse * texture2D(U_MainTexture,V_Texcoord);
}

subroutine (SurfaceColor) vec4 Specular()
{
    // model property
    vec4 materialColor = vec4(1,1,1,1.0);

    // light
    vec3 lightPos = vec3(10,10,10);
    vec4 lightColor = vec4(1, 1, 1, 1.0);
    vec3 L = normalize(lightPos);
    vec3 N = normalize(V_Normal);

    // ambient
    vec4 AMBIENT_COLOR = vec4(0.2,0.2,0.2,1.0);
    vec4 ambient = AMBIENT_COLOR * materialColor;

    // diffuse
    vec4 diffuseColor = vec4(0.8,0.8,0.8,1.0);
    vec4 diffuse = diffuseColor * lightColor * max(0.0,dot(L,N));

    // specular
    float specularIndensity = 0.5;
    vec4 specularColor = vec4(1, 1.0, 1, 1.0);
    vec4 specularMaterial = vec4(0.66);
    vec3 reflectDir = normalize(reflect(-L,N));
    vec3 viewDir = normalize(-V_WorldPos.xyz);
    vec4 specular = specularColor * specularMaterial * pow(max(0,dot(reflectDir,viewDir)),2) * specularIndensity;
    return materialColor* ( diffuse + specular) * texture2D(U_MainTexture,V_Texcoord);
}



void main()
{
    gl_FragColor = U_SurfaceColor();
}