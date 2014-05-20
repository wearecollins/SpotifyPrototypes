#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "RandomTriangulator.h"

//#define USE_LIVE_VIDEO

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // triangulator
        ofImage toTriangulate, toBlur, toDraw;
        string load;
        ofMesh mesh;

        RandomTriangulator triangulator;
        ofxUICanvas * gui;
    
        ofFbo blurFbo;
        ofShader blurHoriz, blurVert;
    
        ofVideoGrabber cam;
};
