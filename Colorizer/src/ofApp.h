#pragma once

#include "ofMain.h"
#include "ImageFilter.h"
#include "ImageManager.h"
#include "ofxUI.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        void keyPressed( int k );
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
        ImageManager manager;
        ColorFilter  filter;
        ColorManager colorManager;
    
        ofxUICanvas * gui;
    
        void onNewImage( ofImage & img );
};
