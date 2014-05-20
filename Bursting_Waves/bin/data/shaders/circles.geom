#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

varying float scale[];

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
    
    float circleRes = 12.0;
    
    int i = 0;
    float size = abs(((abs(gl_PositionIn[i].y))) / 768) * 20.0;// / screenSize;
    float off = abs(gl_PositionIn[i].x / 512.0) * 3;
    
    gl_TexCoord[0] = gl_TexCoordIn[0][0];
    
    //for (int i=0; i<1; i++){
    int bdraw = 0;
    for ( float a = off; a < circleRes + off; a++ ){
        if ( bdraw == 0 ){
            // Angle between each side in radians
            float ang = (PI * 2.0 / circleRes) * a;
            float mult = 1. + ofMap( a, off, (circleRes + off), 0.0, .5,false);
            vec4 offset = vec4(cos(ang) * size, -sin(ang) * size, 0.0, -sin(ang) * size);
            gl_Position = (gl_ModelViewProjectionMatrix * gl_PositionIn[i]) + offset;
            gl_FrontColor = gl_FrontColorIn[0];// * vec4(mult,mult,mult,1.0);// + vec4(cos(ang),-sin(ang),cos(ang),1.0);
            EmitVertex();
            
            ang = (PI * 2.0 / circleRes) * (a + 1);
            offset = vec4(cos(ang) * size, -sin(ang) * size, 0.0, -sin(ang) * size);
            gl_Position = (gl_ModelViewProjectionMatrix * gl_PositionIn[i]) + offset;
            gl_FrontColor = gl_FrontColorIn[0];// * vec4(mult,mult,mult,1.0);// + vec4(cos(ang),-sin(ang),cos(ang),1.0);
            EmitVertex();
            
            gl_Position = (gl_ModelViewProjectionMatrix * gl_PositionIn[i]);
            gl_FrontColor = gl_FrontColorIn[0];// + vec4(mult,mult,mult,1.0);
            EmitVertex();
            EndPrimitive();
        }
    }
    //}
    
    
}