#version 120

varying float scale;
attribute float circleScale;

void main()
{
    scale = circleScale * 1.0;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
    gl_FrontColor = gl_Color;
}