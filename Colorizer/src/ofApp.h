#pragma once

#include "ofMain.h"
#include "ImageFilter.h"
#include "ImageManager.h"
#include "Gui.h"
#include "ofxCv.h"

class ImageSaver : public ofThread {
public:

    void save( ofImage img, ColorFilter & _filter, float _contrast ){
        img.setUseTexture(false);
        temp.setUseTexture(false);
        temp.clone(img);
        filter = &_filter;
        contrast = _contrast;
        startThread();
        img.setUseTexture(true);
    }
    
    void threadedFunction(){
        static cv::Mat t_mat;
        static cv::Mat t_p;
        t_mat = ofxCv::toCv(temp);
        t_mat.convertTo(t_p, -1,contrast,0);
        ofxCv::toOf(t_p, temp);
        filter->process(temp);
        string str = "image_"+ofGetTimestampString()+".png";
        temp.saveImage( "../../../" + str);
    }
protected:
    ofImage temp;
    float contrast;
    ColorFilter * filter;
};

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
        void saveImage( bool & b );
    
        void onNewImage( ofImage & img );
        void onFileLoaded( string & img );
};
