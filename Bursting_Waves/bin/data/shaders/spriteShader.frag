uniform vec3 col1;
uniform vec3 col2;
uniform sampler2DRect inTex;
uniform sampler2DRect colorTex;

varying float tIndex;

varying vec3 ePos;
varying vec2 sPos;

vec2 rotTexCoord( float angle, vec2 tc ) {
   vec2 v = tc*2. - 1.;
	return vec2(v.x*cos(angle) - v.y*sin(angle), v.x*sin(angle) + v.y*cos(angle))*.5 + .5;
}

void main(void) {
    float alpha;
    vec2 tc = rotTexCoord(gl_Color.a, gl_TexCoord[0].st);
    tc *= 256.;
    tc.y += 256. * (tIndex + 0.0);
    vec4 color = texture2DRect(colorTex, tc );
    alpha = texture2DRect(inTex, tc ).a;
    
    if (  gl_Color.a * 1.1 > .71 ){
        gl_FragColor.rgb = gl_Color.rgb  * 1.5;
    } else {
        gl_FragColor.rgb = mix(color.rgb, gl_Color.rgb * 1.5, gl_Color.a * 1.1);//gl_Color.rgb + max(-.01,sin(gl_Color.a * 20.));//*.5 ;
    }
    gl_FragColor.a = alpha;
}