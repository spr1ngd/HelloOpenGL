uniform vec4 U_AmbientColor;

uniform vec4 U_CameraPos;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    vec3 L = normalize(U_CameraPos.xyz - V_WorldPos.xyz);
    vec3 N = normalize(V_Normal);
    float cosThta = dot(L,N);
    float alpha = 1.0;
    if( cosThta > 0.0 )
    {
        // alpha = 1.0 - cosThta;
        alpha = 1.0 - pow(cosThta,32);
    }
    gl_FragColor = vec4(0.1,0.4,0.7,alpha);
}