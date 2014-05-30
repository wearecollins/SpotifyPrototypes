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
        ofAddListener(ofEvents().keyPressed, this, &ScreenManager::keyPressed);
        mode = 0;
        
        _renderCollection = ofPtr<ofRendererCollection>(new ofRendererCollection);
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
                w = maxH;
                h = maxH * 9.0/16.0;
                bChange = true;
                mode = 1;
                break;
                
            // 4x3
            case '2':
                //scale = fmin( 4.0/3.0 * maxW, 3.0/4.0 * maxH );
                w = maxH;
                h = maxH * 3.0/4.0;
                bChange = true;
                mode = 2;
                break;
            
            // billboard
            case '3':
                w = maxH;
                h = maxH * 37./87.0;
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
        output.allocate(ofGetWidth() * 3.0, ofGetHeight() * 3.0, GL_RGBA, 8);
        output.begin();
        ofClear(background.r, background.g, background.b, 0.0);
        ofPushMatrix();
        ofScale(3.0, 3.0, 3.0);
    }
    
    void endSave(){
        ofPopMatrix();
        output.end();
        static ofPixels outpix;
        output.readToPixels(outpix);
        ofSaveImage(outpix, currentFile);
    }
    
protected:
    ofFbo output;
    string currentFile;
    ofPtr<ofBaseRenderer> _origRenderer;
	ofPtr<ofRendererCollection> _renderCollection;
    ofPtr<ofCairoRenderer> _pdfRenderer;
    
};

class ColorManager {
public:
    
    void setup(){
        ofxXmlSettings xml;
        if ( xml.load(ofToDataPath("../../../Shared/colors.xml", true)) ){
            xml.pushTag("settings");
            int sets = xml.getNumTags("set");
            for ( int i=0; i<sets; i++){
                xml.pushTag("set", i);
                int c = xml.getNumTags("color");
                colors.push_back( vector<ofColor>() );
                for ( int j=0; j<c; j++){
                    xml.pushTag("color", j);
                    colors[i].push_back(ofColor( xml.getValue("r", 0.0),  xml.getValue("g", 0.0),  xml.getValue("b", 0.0) ));
                    xml.popTag();
                }
                xml.popTag();
            }
            xml.popTag();
        }
    }
    
    ofColor getRandom(){
        int ind = floor( ofRandom( colors.size() ));
        return colors[ind][ floor( ofRandom(0, colors[ind].size()))];
    }
    
    vector<ofColor> getRandomPair(){
        vector<ofColor> ret;
        int ind = floor( ofRandom( colors.size() ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        ind = floor( ofRandom( colors.size() ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        return ret;
    }
    
    vector<ofColor> getHighLowPair(){
        vector<ofColor> ret;
        int ind = floor( ofRandom( 3 ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        ind = floor( ofRandom( 3, 6 ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        return ret;
    }
    
    vector<ofColor> getHighLowTriplet(){
        vector<ofColor> ret;
        int ind = floor( ofRandom( 2 ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        ind = floor( ofRandom( 3, 5 ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        ind = floor( ofRandom( 4, 6 ));
        ret.push_back( colors[ind][ floor( ofRandom(0, colors[ind].size()))]);
        return ret;
    }
    
    vector<vector<ofColor> > colors;
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
        cout << colorPairs.size() << endl;
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

