#pragma once

#include "ofMain.h"
#include "ImageFilter.h"
#include "ImageManager.h"
#include "ImageSaver.h"
#include "Gui.h"
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
        void windowResized( int w, int h );
		
        ImageManager manager;
        ColorFilter  filter;
        ImageSaver   saver;
    
//        ofxUICanvas * gui;
    
        collins::Gui    gui;
        collins::Button saveButton;
        collins::Button saveAllButton;
        void saveImage( bool & b );
        void saveAll( bool & b );
    
        void onNewImage( ofImage & img );
        void onFileLoaded( string & img );
};
