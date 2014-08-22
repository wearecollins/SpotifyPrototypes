//
//  ColorManager.h
//  Colorizer
//
//  Created by Brett Renfer on 6/2/14.
//
//

#pragma once

#include "ofxXmlSettings.h"

//#define CM_DRAW_OUTLINE

class ColorManager : public ofVec2f {
public:
    
    void setup( string path = "../../../Shared/colors.xml", float scale = 1.0){
        ofxXmlSettings xml;
        active = tempActive = -1;
        radius = 10;
        radius *= scale;
        int x = radius * 2;
        int y = radius * 2;
        
        if ( xml.load(ofToDataPath(path, true)) ){
            xml.pushTag("settings");
            int sets = xml.getNumTags("set");
            for ( int i=0; i<sets; i++){
                xml.pushTag("set", i);
                int c = xml.getNumTags("color");
                colors.push_back( vector<ofColor>() );
                positions.push_back( vector<ofVec2f>() );
                colorNames.push_back( vector<string>() );
                for ( int j=0; j<c; j++){
                    xml.pushTag("color", j);
                    if ( xml.getValue("r", 0.0) >= 0.0 ){
                        colorNames[i].push_back(xml.getValue("title", ""));
                        colors[i].push_back(ofColor( xml.getValue("r", 0.0),  xml.getValue("g", 0.0),  xml.getValue("b", 0.0) ));
                        positions[i].push_back(ofVec2f(x,y));
                    }
                    x += radius * 2;
                    
                    xml.popTag();
                }
                y += radius * 2;
                x = radius * 2;
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
    
    void draw(){
        ofPushMatrix();
        ofTranslate(x,y);
        ofPushStyle();
        ofFill();
        for ( int i=0; i<positions.size(); i++ ){
            for ( int j=0; j<positions[i].size(); j++ ){
#ifdef CM_DRAW_OUTLINE
                ofSetColor( 255 );
                ofCircle(positions[i][j], radius * .85);
#endif
                ofSetColor( colors[i][j] );
                ofCircle(positions[i][j], radius * .7);
                
                if ( (j + i * 10) == active ||
                    (j + i * 10) == tempActive ){
                    ofSetColor(255);
                    ofCircle(positions[i][j], radius * .3);
                }
            }
        }
        ofPopStyle();
        ofPopMatrix();
    }
    
    ofColor checkHit( int x, int y, bool bPressed, bool bForeground ){
        ofVec2f m(x,y);
        m -= *this;
        for ( int i=0; i<positions.size(); i++ ){
            for ( int j=0; j<positions[i].size(); j++ ){
                if ( abs(positions[i][j].distance(m)) < radius  ){
                    if ( bPressed ){
                        active = (j + i * 10);
                    } else {
                        tempActive = (j + i * 10);
                    }
                    return colors[i][j];
                }
            }
        }
        tempActive = -1;
    }
    
    ofColor getActive(){
        ofColor ret(0,0);
        for ( int i=0; i<colors.size(); i++ ){
            for ( int j=0; j<colors[i].size(); j++ ){
                if ( (j + i * 10) == active ){
                    ret.set(colors[i][j]);
                }
            }
        }
        return ret;
    }
    
    string getActiveName(){
        string ret = "";
        for ( int i=0; i<colorNames.size(); i++ ){
            for ( int j=0; j<colorNames[i].size(); j++ ){
                if ( (j + i * 10) == active ){
                    ret = (colorNames[i][j]);
                }
            }
        }
        return ret;
    }
    
    ofColor randomize(){
        ofColor ret(0,0);
        int ind = floor(ofRandom(colors.size()));
        active = ind * 10 + floor(ofRandom(colors[ind].size()));
        for ( int i=0; i<colors.size(); i++ ){
            for ( int j=0; j<colors[i].size(); j++ ){
                if ( (j + i * 10) == active ){
                    ret.set(colors[i][j]);
                }
            }
        }
        return ret;
    }
    
    vector<vector<ofColor> > colors;
    vector<vector<string> > colorNames;
    
protected:
    
    int radius;
    vector<vector<ofVec2f> > positions;
    int active;
    int tempActive;
};

