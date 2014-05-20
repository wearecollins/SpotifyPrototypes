#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

uniform float pointSize;
uniform float screenSize;

const float PI = 3.1415926;

float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool cl) {
    float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
    
    if( cl ){
        if(outputMax < outputMin){
            if( outVal < outputMax )outVal = outputMax;
            else if( outVal > outputMin )outVal = outputMin;
        }else{
            if( outVal > outputMax )outVal = outputMax;
            else if( outVal < outputMin )outVal = outputMin;
        }
    }
    return outVal;
}

void main(void){
    
    float circleRes = 6.0;
    float size = pointSize;// / screenSize;
    
    int i = 0;
    float off = abs(gl_PositionIn[i].x / 512.0) * 3;
    size = abs(distance(gl_PositionIn[i].xy, vec2(0,100))) / (512 + 384) * size;
    
    gl_TexCoord[0] = gl_TexCoordIn[0][0];
    
    vec2 tempTex = gl_TexCoord[0].st;// * vec2(0.5,0.5);
    
    //for (int i=0; i<1; i++){
    int bdraw = 0;
        for ( float a = off; a < circleRes + off; a++ ){
            if ( bdraw == 0 ){
                // Angle between each side in radians
                float ang = (PI * 2.0 / circleRes) * a;
                
                vec4 offset = vec4(cos(ang) * size, -sin(ang) * size, 0.0, 1.0);
                gl_TexCoord[0].st = tempTex + offset.xy;
                gl_Position = (gl_PositionIn[i]) + offset;
                gl_FrontColor = gl_FrontColorIn[0] + vec4(cos(ang),-sin(ang),cos(ang),1.0);
                EmitVertex();
                
                ang = (PI * 2.0 / circleRes) * (a + 1);
                offset = vec4(cos(ang) * size, -sin(ang) * size, 0.0, 1.0);
                gl_TexCoord[0].st = tempTex + offset.xy;
                gl_Position = (gl_PositionIn[i]) + offset;
                gl_FrontColor = gl_FrontColorIn[0] + vec4(cos(ang),-sin(ang),cos(ang),1.0);
                EmitVertex();
                
                gl_TexCoord[0].st = tempTex;
                gl_Position = (gl_PositionIn[i]);
                gl_FrontColor = gl_FrontColorIn[0];
                EmitVertex();
                EndPrimitive();
            }
        }
    //}
    
    
}