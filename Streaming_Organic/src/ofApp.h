#pragma once

#include "ofMain.h"
#include "ThreadedContourFinder.h"
#include "ofxUI.h"
#include "ofAppGLFWWindow.h"

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
		
        void randomizeColors();
    
        vector<ThreadedContourFinder *> contourFinders;
        vector<ofxUICanvas *> guis;
    
        vector<ofImage> imgs;
        vector<ofImage> drawImg;
        string toLoad;
        int whichToLoad;
};
