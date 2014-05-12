/*
 *  Kaleidoscope.h
 *  kaleidoscope
 *
 *  Created by Brett Renfer on 3/2/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 */

class Kaleidoscope
{
	public :
	
	float gDrawWidth, gDrawHeight;
	int r1,r2,r3,tr1,tr2,tr3;
	float count; /// point speed of rotation
	float spinStep; // speed of the step of each rotation///
	int triangleCameraGrab;
	int imgLifeCap;
	float ksx;
	float ksy;
	
	Kaleidoscope(){};
	
	void setup( float _gDrawWidth, float _gDrawHeight){
		count= 0; /// point speed of rotation
		spinStep = .02; // speed of the step of each rotation///
		triangleCameraGrab = 200;
		imgLifeCap 	= 900;
		ksx=150;
		ksy=150;
		
		gDrawWidth = _gDrawWidth;
		gDrawHeight = _gDrawHeight;
	};
	
	void render(float _gDrawWidth, float _gDrawHeight, ofImage textureImg) {
		
		gDrawWidth = _gDrawWidth;
		gDrawHeight = _gDrawHeight;
				
		// println("My texture is "+textureImg);
		float cc = 70;
		float ix,iy;
		float u1;
		
		float  v1, u2, v2, u3, v3;
		
		/*int vw = gVideoWidth;
		 int vh = gVideoHeight;*/
		
		int vw = textureImg.width;
		int vh = textureImg.height;
		int vw10 = vw / 10;
		int vh10 = vh / 10;
		int vw2 = vw / 2;
		int vh2 = vh / 2;
		float mmm = 100000.0;
		
		//  vw2 = (int) ((float) (millis() % mmm) * (float) vw / mmm);
		
		u1 = vw2 + (sin(count/cc+0)*r3); 
		v1 = vh2 + (cos(count/cc+0)*r3); 
		u2 = vw2 + (sin(count/cc+ PI *2/3)*r3); 
		v2 = vh2 + (cos(count/cc+ PI *2/3)*r3); 
		u3 = vw2 + (sin(count/cc+ PI *2/3*2)*r3); 
		v3 = vh2 + (cos(count/cc+ PI *2/3*2)*r3); 
		
		for(iy=0;iy<(gDrawHeight/ksy)+1;iy++) {
			
			ix=0;
			
			ofTexture& tex = textureImg.getTextureReference();
			
			tex.bind();
			
			glBegin(GL_TRIANGLE_STRIP);
			//textureImg.blend(textureImg,0,0,640,480,0,0,640,480,ADD);
			glTexCoord2f(u2, v2);
			glVertex2f(ix * ksx- ksx/2, (iy+1)*ksy); 
			for(ix = 0 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
				glTexCoord2f(u1, v1);
				glVertex2f(ix * ksx, iy * ksy); 
				glTexCoord2f(u3, v3);
				glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy); 
				ix++;
				glTexCoord2f(u2,v2);
				glVertex2f(ix * ksx, iy * ksy); 
				glTexCoord2f(u1, v1);
				glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy); 
				
				ix++;
				glTexCoord2f(u3,v3);
				glVertex2f(ix * ksx, iy * ksy); 
				glTexCoord2f(u2, v2);
				glVertex2f((ix+1)* ksx- ksx/2, (iy+1)*ksy); 
			}
			glEnd(); 
			
			tex.unbind();
			tex.bind();
			
			iy++;
			ix=0;
						
			glBegin(GL_TRIANGLE_STRIP);
			//textureImg.blend(textureImg,0,0,640,480,0,0,640,480,ADD);
			glTexCoord2f(u2,v2);
			glVertex2f(ix* ksx- ksx/2, (iy)*ksy);
			for(ix=0;ix<(gDrawWidth/ ksx)+1;ix++) {
				glTexCoord2f(u1, v1);
				glVertex2f(ix* ksx, (iy+1)*ksy); 
				glTexCoord2f(u3, v3);
				glVertex2f((ix+1)* ksx- ksx/2, iy * ksy); 
				ix++;
				glTexCoord2f(u2, v2);
				glVertex2f(ix* ksx, (iy+1)*ksy); 
				glTexCoord2f(u1, v1);
				glVertex2f((ix+1)* ksx- ksx/2, iy * ksy); 
				ix++;
				glTexCoord2f(u3,v3);
				glVertex2f(ix* ksx, (iy+1)*ksy); 
				glTexCoord2f(u2, v2);
				glVertex2f((ix+1)* ksx- ksx/2, iy * ksy); 
			}
			glEnd(); 
			
			
			tex.unbind();
		}
		
		
		// ucomment this section to see the video overlay with the triangle being grabbed
		
		//////////////////////////////////////////////////////

		//////////////////
		
		
		// this controls the speed of triangle rotation
		count  -= spinStep;
		// make decrement variable...
		
		
		// these control the size of the camer grab/////////////////////////////////////////////////////////////////////////
		tr1 = tr2 = tr3 = triangleCameraGrab;
		
		if (tr1>r1) r1=tr1;
		if (tr2>r2) r2=tr2;
		if (tr3>r3) r3=tr3;
		
		if (r1>350) r1 = 350;
		if (r2>350) r2 = 350;
		if (r3>350) r3 = 350;
		
		if (r1>400) r1=400;
		if (r2>400) r2=400;
		if (r3>400) r3=400;
		
		
	}
};