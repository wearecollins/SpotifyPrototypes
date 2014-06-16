//
//  ImageFilter.h
//  Randomizer
//
//  Created by Brett Renfer on 5/1/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ScreenManager {
public:
    
    void setup(){
        saveScale = 3.0;
        ofAddListener(ofEvents().keyPressed, this, &ScreenManager::keyPressed);
        mode = 0;
    }
    
    void keyPressed( ofKeyEventArgs & k ){
        float maxW = ofGetScreenWidth();
        float maxH = ofGetScreenHeight() * .8;
        
        float w = maxW;
        float h = maxH;
        
        bool bChange = false;
        
        switch ( k.key ){
            // 16x9
            case '1':
                w = maxW;
                h = maxW * 9.0/16.0;
                bChange = true;
                mode = 1;
                break;
                
            // 4x3
            case '2':
                //scale = fmin( 4.0/3.0 * maxW, 3.0/4.0 * maxH );
                w = maxW;
                h = maxW * 3.0/4.0;
                bChange = true;
                mode = 2;
                break;
            
            // billboard
            case '3':
                w = maxW;
                h = maxW * 37./87.0;
                bChange = true;
                mode = 3;
                break;
            
            // webad
            case '4':
                w = maxH * 4./15.0;
                h = maxH;
                bChange = true;
                mode = 4;
                break;
        }
        if ( bChange ){
            ofSetWindowPosition(0,0);
            ofSetWindowShape(floor(w), floor(h));
            ofSetWindowPosition(0,0);
        }
    }
    
    int mode;
    
    void startSave( string file, ofColor background ){
        currentFile = file;
        if ( output.isAllocated() ){
        }
        output.allocate(ofGetWidth() * saveScale, ofGetHeight() * saveScale, GL_RGBA, 8);
        output.begin();
        ofClear(background.r, background.g, background.b, 0.0);
        ofPushMatrix();
        ofScale(saveScale, saveScale, saveScale);
    }
    
    void endSave(){
        ofPopMatrix();
        output.end();
        static ofPixels outpix;
        output.readToPixels(outpix);
        ofSaveImage(outpix, currentFile);
    }
    
    float saveScale;
    
protected:
    ofFbo output;
    string currentFile;
    
};

class ImageFilter : protected ofThread {
public:
    
    ImageFilter(){
        bNeedToProcess = false;
    }
    
    virtual void process( ofImage & img ) = 0;
    virtual void loadAndProcess( string image, bool bThreaded = true, bool bSameDir = false, string suffix = "processed" ){
        
        ofFile f(image);
        dir = bSameDir ? f.getEnclosingDirectory() + "/" : "";
        name   = f.getBaseName() + "_" + suffix;
        ext    = f.getExtension();
        suff   = suffix;
        
        currentImage.setUseTexture(false);
        if ( currentImage.loadImage(image) ){
            bNeedToProcess = true;
            if ( bThreaded ) startThread();
            else {
                process( currentImage );
                currentImage.saveImage( dir + name + "." + ext );
                ofNotifyEvent(onProcessed, currentImage, this);
            }
        }
    }
    
    ofEvent<ofImage> onProcessed;
    
protected:
    
    bool        bNeedToProcess;
    ofImage     currentImage;
    string      dir, name, ext, suff;
    
    virtual void threadedFunction(){
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
    
    void setColorPair( int index, ofColor a, ofColor b ){
        if ( index < colorPairs.size()){
            colorPairs[index].clear();
            colorPairs[index].push_back(ofColor(a));
            colorPairs[index].push_back(ofColor(b));
        } else {
            addColorPair(a, b);
        }
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
        unsigned char * alph = img.getPixels();
        int ch = img.getPixelsRef().getNumChannels();
        ofImage clone; clone.clone(img);
        clone.setImageType(OF_IMAGE_GRAYSCALE);
        ofPixelsRef p = clone.getPixelsRef();
        
        int index = 0;
        ofPixels pix;
        for ( auto pair : colorPairs ){
            pix.setFromPixels(p.getPixels(), p.getWidth(), p.getHeight(), p.getNumChannels());
            pix.setImageType( ch < 4 ? OF_IMAGE_COLOR : OF_IMAGE_COLOR_ALPHA );
            for ( int i=0; i<pix.size(); i+=pix.getNumChannels() ){
                pix[i + 0] = ofLerp(pair[0].r, pair[1].r, 1.0 - pix[i + 0]/255.0);
                pix[i + 1] = ofLerp(pair[0].g, pair[1].g, 1.0 - pix[i + 1]/255.0);
                pix[i + 2] = ofLerp(pair[0].b, pair[1].b, 1.0 - pix[i + 2]/255.0);
                if ( ch == 4 ){
                    pix[i + 3] = alph[i + 3];
                }
            }
            ofImage temp; temp.setUseTexture(false);
            temp.setFromPixels(pix);
            temp.saveImage( dir + name + "_" + ofToString( index ) + "." + ext );
            index++;
        }
        img.setFromPixels(pix);
    }
    
    vector< vector<ofColor> > colorPairs;
    
};

