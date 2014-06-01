#pragma once

#include "ofMain.h"

#include "Logo.h"
#include "ofxRCUtils.h"
#include "ImageManager.h"
#include "ofxUI.h"
#include "CvFilters.h"
#include "Bubbles.h"

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
		
        void toggleGuiVisible();
    
        Logo logo;
        vector<ofMesh>  logoMeshes;
        ImageManager        imageManager;
        ColorManager        colorManager;
        rc::KeyManager      keyManager;
        ScreenManager       screen;
    
        ColorFilter     colors;
        ofColor colorA, colorB;
    
        // gui
        ofxUICanvas * gui;
    
        Bubbles bubbles;
        void onImageLoaded( ofImage & img );
};
