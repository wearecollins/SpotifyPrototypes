#pragma once

#include "ofMain.h"
#include "ofxSVG.h"
#include "ofxEasing.h"
#include "ofxEasingQuad.h"

#include "ofxUI.h"

class ofApp : public ofBaseApp {

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
		
        ofImage charles;
        ofxSVG  path;
        ofxEasing * easing;
        ofxEasingQuad quad;
        ofMesh  pathMesh, sourceMesh;
        vector<int> rands;
        vector<float> texIndices, texScales, circleScales;
    
        ofMesh burstMesh, burstSource;
    
        void randomize();
        ofShader shader, spriteShader, lineShader, circleShader;
    
        ofFbo render;
    
        ofxUICanvas * gui;
        void onGui( ofxUIEventArgs & e );
    
        ofImage sprites, raySprites;
};
