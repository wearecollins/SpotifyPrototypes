#pragma once

#include "ofMain.h"
#include "ThreadedContourFinder.h"
#include "ofxUI.h"
#include "ofAppGLFWWindow.h"
#include "ImageManager.h"
#include "ColorManager.h"
#include "Gui.h"

#include "Logo.h"
#include "Metaballs.h"
#include "Bubbles.h"

enum DrawMode {
    MODE_BUBBLES = 0,
    MODE_META,
    MODE_PHOTO
};

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
		
        void randomizeColors();
    
        // GUI
        vector<ofxUICanvas *> guisAdvanced;
        vector<ofxUICanvas *> guis;
        void toggleGuiVisible();
        void toggleAdvVisible();
        void saveGui();
    
        collins::Gui gui;
        collins::Button saveButton, randomButton;
        void onSave( bool & b );
        void onRandom( bool & b );
    
        // managers
        ColorManager    colorMgr;
        ScreenManager   screen;
        rc::KeyManager  keys;
    
        // images
        void setupImage( ofImage & img );
        vector<ofImage> drawImg;
        ImageManager    images;
        ColorFilter     colorsProcessor;
        vector<ofColor> colors;
        void onImageLoaded( ofImage & img );
        void onFileLoaded( string & img );
    
        // layer 1: blobs
        vector<ThreadedContourFinder *> contourFinders;
    
        // layer 2: logo
        Logo logo;
    
        // layer 1/3
        int mode,modeBG;
    
        ofxCv::ContourFinder metaballProcessor;
        Metaballs metaballs, metaballsBG;
        ofFbo render;
        ofPixels pix;
        ofTessellator tess;
        ofMesh drawMesh;
    
        Bubbles bubbles;
        void drawLayerOne( bool bRender=true );
        void drawLayerTwo();
        void drawLayerThree( bool bRender=true);
};
