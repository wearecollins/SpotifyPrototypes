/* Fragment shader */
//#version 150
#define NUM_BALLS 30
#define BALL_SUB  10

//out vec4 color_out;
uniform vec3 balls[NUM_BALLS]; //.xy is position .z is radius
uniform vec4 color;
uniform float iso;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

bool energyField(vec2 p, float gooeyness, float iso)
{
    float PI = 3.14159265358979323846;
    float DEG_TO_RAD = (PI/180.0);

    float en = 0.0;
    bool result = false;
    for(int i=0; i<NUM_BALLS; ++i)
    {
        float radius = balls[i].z;// + rand(balls[i].xy) * 50.0;
        float denom =  max(0.0001, pow(length(vec2(balls[i].xy - p)), gooeyness));
        en += (radius / denom);
    }
    if(en > iso)
        result = true;
    return result;
}

void main()
{
    bool outside;
    /* gl_FragCoord.xy is in screen space / fragment coordinates */
    outside = energyField(gl_FragCoord.xy, 1.0, iso);
    if(outside == true)
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    else
        discard;
        //gl_FragColor = vec4(color.r, color.g, color.b,0.0);
}