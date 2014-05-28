#pragma once

#include "ofMain.h"
#include "SongMesh.h"
#include "ofxEasingFunc.h"
#include "ofxFft.h"

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
    
        SongMesh mesh;
        ofxEasingFunc::Sine easingCubic;
        ofxEasingFunc::Linear easingLinear;
        ofxEasingFunc::Expo easingQuad;
    
        vector<string> toLoad;
        vector<ofImage> images;
    
        // fft
    
        ofxFft* fft;
        void updateFFT();
    
        ofMutex soundMutex;
        vector<float> drawBins, middleBins, audioBins;
        vector<float> drawBuffer, middleBuffer, audioBuffer;
        void plot(vector<float>& buffer, float scale, float offset);
};
