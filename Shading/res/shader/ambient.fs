
uniform vec4 U_LightColor;
uniform vec4 U_MaterialColor;

void main()
{
    gl_FragColor = U_LightColor * U_MaterialColor;
}