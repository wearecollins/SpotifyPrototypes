varying vec3 ePos;
varying vec2 sPos;
varying float tIndex;
attribute float texIndex;
attribute float texScale;

vec2 rotateVec2( float angle, vec2 tc ) {
   vec2 v = tc;
	float xrot = v.x*cos(angle) - v.y*sin(angle);
   v.y = v.x*sin(angle) + v.y*cos(angle);
	v.x = xrot;
	return vec2(v.x,v.y);
}

void main(void) {	
    sPos = gl_Vertex.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;//ftransform();
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0 * 2. -1.;
    gl_PointSize = ((2.0 - gl_Color.a) * texScale) * 1.1;// * min(gl_Color.a/.5, 1.) ;
    tIndex = texIndex;
   
	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
	ePos = -normalize(ecPosition.xyz/ecPosition.w);
}