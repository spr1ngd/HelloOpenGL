uniform sampler2D U_MainTexture;
varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_Texcoord; 

void main()
{
    vec4 materialColor = vec4(1.0);

    // light
    vec3 lightPos = vec3(10,10,10);
    vec4 lightColor = vec4(1, 1, 1, 1.0);
    vec3 L = normalize(lightPos);
    vec3 N = normalize(V_Normal);

    // ambient
    vec4 AMBIENT_COLOR = vec4(0.102, 0.102, 0.102, 1.0);

    // diffuse
    vec4 diffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 diffuse = diffuseColor * lightColor * max(0.0,dot(L,N));
    gl_FragColor = diffuse * texture2D(U_MainTexture,V_Texcoord);
}