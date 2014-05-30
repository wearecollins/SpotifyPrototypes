#pragma once

#include "ofMain.h"
#include "Metaballs.h"

#include "ofxCv.h"
#include "ImageFilter.h"
#include "Logo.h"
#include "ofxRCUtils.h"

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
    
        ofFbo render;
        vector<Metaballs> metaballs;
        vector<ofImage> images;
        vector<string> imagesProcessed;
    
        ColorManager colors;
    
        ofxCv::ContourFinder contourFinder;
        ColorFilter imageProcessor;
    
        Logo logo;
    
        void setupImage( ofImage & img );
        void randomizeColor();
    
        ScreenManager screen;
};
