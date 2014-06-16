//
//  Logo.h
//  Knockout_openCV
//
//  Created by Brett Renfer on 5/29/14.
//
//

#pragma once
#include "ofxSvg.h"
#include "ofxRCUtils.h"

class Logo : public ofRectangle {
public:
    
    void setup( string file="../../../Shared/logo.svg"){
        scale = 1.0;
        drawCircle = false;
        logo.load( ofToDataPath(file, true) );
        width = logo.getWidth();
        height = logo.getHeight();
        
        bool bFront = true;
        
        for ( int i=0; i<logo.getNumPath(); i++ ){
            logoMeshes.push_back(ofMesh());
            zpos.push_back(0.0);
            logo.getPathAt(i).setPolyWindingMode(OF_POLY_WINDING_ODD);
            vector<ofPolyline> p = logo.getPathAt(i).getOutline();
            
            tess.tessellateToMesh(p, OF_POLY_WINDING_ODD, logoMeshes.back());
            
            for ( auto & i : logoMeshes.back().getIndices() ){
                auto v = logoMeshes.back().getVertex(i);
                logoMeshes.back().addTexCoord(v);
            }
            
            if ( bFront ) bFront = !bFront;
        }
        x = ofGetWidth()/2.0;
        y = ofGetHeight()/2.0;
        bDepthTest = true;
    }
    
    void draw(){
        float drawscale = scale;
        ofPushMatrix();
        if ( drawCircle ){
            if ( bDepthTest ) ofDisableDepthTest();
            ofSetCircleResolution(100);
            ofSetColor(circleColor);
            ofEllipse(x,y, (width * scale)/2.0, (width * scale)/2.0);
            ofSetColor(255);
            if ( bDepthTest ) ofEnableDepthTest();
            drawscale *= .6;
        }
        int i =0;
        
        for ( auto & m : logoMeshes ){
            ofVec2f center(x, y);
            center += ofVec2f(-width/2.0 * drawscale, -height/2.0 * drawscale);
            for ( auto & i : m.getIndices() ){
                auto v = m.getVertex(i);
                v += center;
                v *= drawscale;
                m.setTexCoord(i, v);
            }
            
            ofPushMatrix();
            //rc::ofTranslateCenter();
            ofTranslate(x, y);
            ofTranslate(-width/2.0 * drawscale, -height/2.0 * drawscale, zpos[i]);
            ofScale(drawscale, drawscale);
            m.draw();
            ofPopMatrix();
            i++;
        }
        ofPopMatrix();
    }
    
    void setColor( ofColor color ){
        for ( auto & m : logoMeshes ){
            rc::ofSetMeshColor(m, color);
        }
    }
    
    void setColors( ofColor colorA, ofColor colorB ){
        for ( auto & m : logoMeshes ){
            rc::ofSetMeshGradient(m,    ofFloatColor(colorA.r/255.0f, colorA.g/255.0f, colorA.b/255.0f),
                                  ofFloatColor(colorB.r/255.0f, colorB.g/255.0f, colorB.b/255.0f));
        }
    }
    
    void save(){
//        ofCairoRenderer
//        saver.beginEPS("logo_" + ofGetTimestampString() + ".eps");
    }
    
    float   scale;
    bool    drawCircle;
    vector<float>  zpos;
    ofColor circleColor;
    bool    bDepthTest;
    
protected:
    
    vector<ofMesh>  logoMeshes;
    ofxSVG logo;
    ofTessellator    tess;
};
