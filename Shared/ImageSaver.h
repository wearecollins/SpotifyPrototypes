//
//  ImageSaver.h
//  Colorizer
//
//  Created by Brett Renfer on 8/21/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCv.h"

class ImageSaver : public ofThread {
public:
    
    ImageSaver() :
    saveToolTip(false),
    directory("../../../"){
        
    }
    
    void save( ofImage img, ColorFilter & _filter, float _contrast, string dir = "../../../", bool bThread = true ){
        img.setUseTexture(false);
        temp.setUseTexture(false);
        temp.clone(img);
        filter = &_filter;
        contrast = _contrast;
        directory = dir;
        if ( directory.substr(directory.size()-1, 1) != "/"){
            directory += "/";
        }
        
        if ( bThread ) startThread();
        else process();
        img.setUseTexture(true);
    }
    
    void process(){
        static cv::Mat t_mat;
        static cv::Mat t_p;
        t_mat = ofxCv::toCv(temp);
        t_mat.convertTo(t_p, -1,contrast,0);
        ofxCv::toOf(t_p, temp);
        filter->process(temp);
        string str = "image_"+ofGetTimestampString()+"_"+ofToString(floor(ofRandom(100)));
        if ( saveToolTip ){
            ofImage a,b;
            a.setUseTexture(false);
            b.setUseTexture(false);
            a.allocate(100,100,OF_IMAGE_COLOR);
            b.allocate(100,100,OF_IMAGE_COLOR);
            for ( int i=0; i<100; i++){
                for ( int j=0; j<100; j++){
                    a.setColor(i, j, filter->getColorPair(0)->at(0));
                    b.setColor(i, j, filter->getColorPair(0)->at(1));
                }
            }
            a.saveImage( directory + str +"_color1" +".png" );
            b.saveImage( directory + str +"_color2" +".png" );
        }
        temp.saveImage( directory + str +".png");
    }
    
    void threadedFunction(){
        process();
    }
    
    bool saveToolTip;
    
protected:
    ofImage temp;
    float contrast;
    ColorFilter * filter;
    string directory;
};
