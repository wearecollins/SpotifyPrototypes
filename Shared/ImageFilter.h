//
//  ImageFilter.h
//  Randomizer
//
//  Created by Brett Renfer on 5/1/14.
//
//

#pragma once

#include "ofMain.h"

class ImageFilter : protected ofThread {
public:
    
    ImageFilter(){
        bNeedToProcess = false;
    }
    
    virtual void process( ofImage & img ) = 0;
    virtual void loadAndProcess( string image, bool bSameDir = false, string suffix = "processed" ){
        
        ofFile f(image);
        dir = bSameDir ? f.getEnclosingDirectory() + "/" : "";
        name   = f.getBaseName() + "_" + suffix;
        ext    = f.getExtension();
        suff   = suffix;
        
        currentImage.setUseTexture(false);
        if ( currentImage.loadImage(image) ){
            bNeedToProcess = true;
            startThread();
        }
    }
    
    ofEvent<ofImage> onProcessed;
    
protected:
    
    bool        bNeedToProcess;
    ofImage     currentImage;
    string      dir, name, ext, suff;
    
    void threadedFunction(){
        while (isThreadRunning()){
            if ( bNeedToProcess ){
                process( currentImage );
                currentImage.saveImage( dir + name + "." + ext );
                ofNotifyEvent(onProcessed, currentImage, this);
                bNeedToProcess = false;
            }
        }
    }
};

class ColorFilter : public ImageFilter {
public:
    
    ColorFilter() : ImageFilter() {
    }
    
    void addColorPair( ofColor a, ofColor b ){
        colorPairs.push_back(vector<ofColor>());
        colorPairs.back().push_back(ofColor(a));
        colorPairs.back().push_back(ofColor(b));
    }
    
    vector<ofColor> * getColorPair( int index ){
        if ( index < colorPairs.size() && index >= 0 ){
            return &colorPairs[index];
        } else {
            return &colorPairs[0];
        }
    }
    
    void process( ofImage & img ){
        if ( colorPairs.size() == 0 ){
            ofColor c, d;
            c.setHue(ofRandom(255.0));
            c.setSaturation(200);
            c.setBrightness(200);
            d.setHue(ofRandom(255.0));
            d.setSaturation(200);
            d.setBrightness(200);
            
            colorPairs.push_back(vector<ofColor>());
            colorPairs.back().push_back(c);
            colorPairs.back().push_back(d);
        }
        img.setImageType(OF_IMAGE_GRAYSCALE);
        ofPixelsRef p = img.getPixelsRef();
        
        int index = 0;
        for ( auto pair : colorPairs ){
            ofPixels pix;
            pix.setFromPixels(p.getPixels(), p.getWidth(), p.getHeight(), p.getNumChannels());
        
            pix.setImageType(OF_IMAGE_COLOR);
            for ( int i=0; i<pix.size(); i+=pix.getNumChannels() ){
                pix[i + 0] = ofLerp(pair[0].r, pair[1].r, 1.0 - pix[i + 0]/255.0);
                pix[i + 1] = ofLerp(pair[0].g, pair[1].g, 1.0 - pix[i + 1]/255.0);
                pix[i + 2] = ofLerp(pair[0].b, pair[1].b, 1.0 - pix[i + 2]/255.0);
            }
            ofImage temp; temp.setUseTexture(false);
            temp.setFromPixels(pix);
            temp.saveImage( dir + name + "_" + ofToString( index ) + "." + ext );
            index++;
        }
    }
    
    vector< vector<ofColor> > colorPairs;
    
};

