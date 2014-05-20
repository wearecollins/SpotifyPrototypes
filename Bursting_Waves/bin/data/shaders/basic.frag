#version 120
#extension GL_ARB_texture_rectangle: enable

uniform sampler2DRect tex0;

void main()
{
    vec4 myOutputColor  = gl_Color;
    vec4 texColor       = texture2DRect(tex0, gl_TexCoord[0].st);
    vec4 color          = texColor * myOutputColor + myOutputColor * .1;
    if (texColor.a == 0.0){
        color = myOutputColor * .9;
    }
    gl_FragColor = myOutputColor;//color;
    //gl_FragColor.r = 0.5;
//    gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}