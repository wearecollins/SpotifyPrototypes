//
//  Bursting.h
//  AnimationTests
//
//  Created by Brett Renfer on 6/16/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxRCUtils.h"
#include "ColorManager.h"

#define BURST_START_ANGLE -60
#define BURST_END_ANGLE 60

class BurstMesh : public ofMesh {
public:
    BurstMesh(){
        animationTime = ofRandom(10,40);
        animationDelay = ofRandom(0,60);
        oscillateAmount = ofRandom(10, 100);
    }
    
    float animationDelay;
    int animationTime;
    float oscillateAmount;
    
};

class BurstShape {
public:
    
    BurstShape() :
    startAngle(BURST_START_ANGLE),
    endAngle(BURST_END_ANGLE),
    radius(60),
    maxHeight(600), //fix!
    frame(0)
    {
        source.set(ofGetWidth()/2.0, ofGetHeight() * .75);
        colors.setup();
    }
    
    virtual void generate() = 0;
    virtual void draw(){
        for (auto & m : meshes){
            m.draw();
        }
    }
    virtual void clear(){
        meshes.clear();
        frame = 0;
        colorPair = colors.getHighLowTriplet();
    }
    
    vector<BurstMesh> & getMeshes(){
        return meshes;
    }
    
    vector<ofColor> & getCurrentColors(){
        return colorPair;
    }
    
protected:
    float   startAngle, endAngle;
    float   maxHeight;
    
    float   frame;
    ofVec2f source;
    float   radius;
    vector<BurstMesh> meshes;
    vector<ofColor> colorPair;
    ColorManager    colors;
    
    // whatever
    float easeIn(const float t){
        return t * t;
    }
    
    float easeOut(const float t){
        return 1. - easeIn(1. - t);
    }
};

class Spike : public BurstShape {
public:

    void generate(){
        clear();
        
        float ranNum = ofRandom(3,5);
        float rows   = floor( ofRandom(1, 3));
        float realMax = maxHeight;
        
        float div = (float) (endAngle-startAngle)/ranNum;
        ofVec2f left(source); left.x -= radius;
        ofVec2f right(source); right.x += radius;
        
        for ( int j=0; j<rows;j++){
            float maxHeight = ofMap(j, rows, 0, (float) realMax * .1, realMax);
            for ( int i=0; i<ranNum; i++){
                ofVec2f peak(source);
                // fix
                float min = ofMap(i, 0, ranNum, 0, 1.0);
                float max = ofMap(i, 0, ranNum, 0, 1.0);
                
                if ( min < .25 ){
                    min = ofMap(i, 0, ranNum * .25, maxHeight * .1, maxHeight * .2);
                    max = ofMap(i, 0, ranNum * .25, maxHeight * .2, maxHeight * .4);
                } else if ( min >=.25 && min <= .75){
                    min = ofMap(i, ranNum * .25, ranNum * .75, maxHeight * .4, maxHeight * .6);
                    max = ofMap(i, ranNum * .25, ranNum * .75, maxHeight * .6, maxHeight * .9);
                } else {
                    min = ofMap(i, ranNum * .75, ranNum, maxHeight * .1, maxHeight * .2);
                    max = ofMap(i, ranNum * .75, ranNum, maxHeight * .2, maxHeight * .4);
                }
                
                float ranDist = ofRandom(min, max);
                
                peak.y -= ranDist;
                peak.rotate(startAngle + (div * (float) i) * ofRandom(.7, 1.1), source);
                
                BurstMesh triangle;
                triangle.addVertex(peak);
                triangle.addVertex(left);
                triangle.addVertex(right);
                triangle.setupIndicesAuto();
                
                ofFloatColor c;
                ofColor d = colorPair[ j % colorPair.size() ];
                c.set(d.r/255.0f,d.g/255.0f,d.b/255.0f);
                
                for ( auto & ind : triangle.getIndices()){
                    triangle.addColor(c);
                }
                
                meshes.push_back(triangle);
            }
        }
    }
    
    void draw(){
        for (auto m : meshes){
            float x = m.getVertices()[0].x;
            float y = m.getVertices()[0].y;
            float eased = easeOut(ofMap(frame, m.animationDelay, m.animationDelay + m.animationTime, 0, 1.0, true));
            m.getVertices()[0].x = ofMap(eased, 0, 1.0, source.x, x, true);
            m.getVertices()[0].y = ofMap(eased, 0, 1.0, source.x, y, true);
            m.getVertices()[0].x += ofSignedNoise(ofGetElapsedTimeMillis() * .0001 + y * .001) * m.oscillateAmount;
            m.getVertices()[0].y += ofSignedNoise(ofGetElapsedTimeMillis() * .0002 + x * .003) * m.oscillateAmount;
            m.draw();
        }
        frame++;
    }
    
};

class Bubbles : public BurstShape {
public:
    
    void generate(){
        clear();
        
        float ranNum = ofRandom(3,5);
        float rows   = floor( ofRandom(1, 3));
        float realMax = maxHeight;
        
        float div = (float) (endAngle-startAngle)/ranNum;
        ofVec2f left(source); left.x -= radius;
        ofVec2f right(source); right.x += radius;
        
        for ( int j=0; j<rows;j++){
            float maxHeight = ofMap(j, rows, 0, (float) realMax * .1, realMax);
            for ( int i=0; i<ranNum; i++){
                ofVec2f peak(source);
                // fix
                float min = ofMap(i, 0, ranNum, 0, 1.0);
                float max = ofMap(i, 0, ranNum, 0, 1.0);
                
                if ( min < .25 ){
                    min = ofMap(i, 0, ranNum * .25, maxHeight * .1, maxHeight * .2);
                    max = ofMap(i, 0, ranNum * .25, maxHeight * .2, maxHeight * .4);
                } else if ( min >=.25 && min <= .75){
                    min = ofMap(i, ranNum * .25, ranNum * .75, maxHeight * .4, maxHeight * .6);
                    max = ofMap(i, ranNum * .25, ranNum * .75, maxHeight * .6, maxHeight * .9);
                } else {
                    min = ofMap(i, ranNum * .75, ranNum, maxHeight * .1, maxHeight * .2);
                    max = ofMap(i, ranNum * .75, ranNum, maxHeight * .2, maxHeight * .4);
                }
                
                float ranDist = ofRandom(min, max);
                
                peak.y -= ranDist;
                peak.rotate(startAngle + (div * (float) i) * ofRandom(.7, 1.1), source);
                
                
                int num = ofRandom(2,5);
                for ( int k=0; k<num; k++){
                    BurstMesh triangle;
                    triangle.animationTime *= 3.0;
                    float width = ofMap(k, 0, num, ranDist * .2, ranDist * .5);
                    float x = ofMap(k, 0, num, source.x, peak.x) + sin(k * .001) * width * 2.0;
                    float y = ofMap(k, 0, num, source.y - (radius), peak.y);
                    y -= width * 2.0;
                    ofMesh m = rc::ofMeshCircle::circle(width, x * ofRandom(.9, 1.1), y  * ofRandom(.75, 1.25));
                    triangle.append(m);
                    
                    ofFloatColor c;
                    ofColor d = colorPair[ j % colorPair.size() ];
                    c.set(d.r/255.0f,d.g/255.0f,d.b/255.0f);
                    
                    for ( auto & ind : triangle.getIndices()){
                        triangle.addColor(c);
                    }
                    meshes.push_back(triangle);
                }
            }
        }
    }
    
    void draw(){
        for (auto m : meshes){
            float osc = sin(ofGetElapsedTimeMillis() * .001 + m.animationTime) * m.oscillateAmount/2.0;
            float oscY = cos(ofGetElapsedTimeMillis() * .002 + m.animationTime) * m.oscillateAmount/10.0;
            for ( auto i : m.getIndices() ){
                float x = m.getVertices()[i].x;
                float y = m.getVertices()[i].y;
                float eased = easeOut(ofMap(frame, m.animationDelay, m.animationDelay + m.animationTime, 0, 1.0, true));
                m.getVertices()[i].x = ofMap(eased, 0, 1.0, source.x, x, true) + oscY;
                m.getVertices()[i].y = ofMap(eased, 0, 1.0, source.y, y, true) + osc;
            }
            m.draw();
        }
        frame++;
    }
};

class Squares : public BurstShape {
public:
    
    void generate(){
        clear();
        
        float ranNum = ofRandom(3,5);
        float rows   = floor( ofRandom(1, 3));
        float realMax = maxHeight;
        
        float div = (float) (endAngle-startAngle)/ranNum;
        ofVec2f left(source); left.x -= radius;
        ofVec2f right(source); right.x += radius;
        
        for ( int j=0; j<rows;j++){
            float maxHeight = ofMap(j, rows, 0, (float) realMax * .1, realMax);
            for ( int i=0; i<ranNum; i++){
                ofVec2f peak(source);
                float min = ofMap(i, 0, ranNum, 0, 1.0);
                float max = ofMap(i, 0, ranNum, 0, 1.0);
                
                if ( min < .25 ){
                    min = ofMap(i, 0, ranNum * .25, maxHeight * .1, maxHeight * .2);
                    max = ofMap(i, 0, ranNum * .25, maxHeight * .2, maxHeight * .4);
                } else if ( min >=.25 && min <= .75){
                    min = ofMap(i, ranNum * .25, ranNum * .75, maxHeight * .4, maxHeight * .6);
                    max = ofMap(i, ranNum * .25, ranNum * .75, maxHeight * .6, maxHeight * .9);
                } else {
                    min = ofMap(i, ranNum * .75, ranNum, maxHeight * .1, maxHeight * .2);
                    max = ofMap(i, ranNum * .75, ranNum, maxHeight * .2, maxHeight * .4);
                }
                
                float ranDist = ofRandom(min, max);
                
                peak.y -= ranDist;
                float ang = startAngle + (div * (float) i) * ofRandom(.7, 1.1);
                float width = ranDist * .1;
                
                ofVec2f minus(peak); minus.x -= width;
                ofVec2f plus(peak); plus.x += width;
                ofVec2f sourceM(source); sourceM.x -= width;
                ofVec2f sourceP(source); sourceP.x += width;
                
                minus.rotate(ang, source);
                plus.rotate(ang, source);
                sourceM.rotate(ang, source);
                sourceP.rotate(ang, source);
                
                float randomOffset = ofRandom(-width, width);
                minus.x += randomOffset; plus.x += randomOffset;
                sourceM.x += randomOffset; sourceP.x += randomOffset;
                
                BurstMesh triangle;
                triangle.addVertex(minus);
                triangle.addVertex(sourceM);
                triangle.addVertex(sourceP);
                
                triangle.addVertex(plus);
                triangle.addVertex(sourceM);
                triangle.addVertex(sourceP);
                
                triangle.addIndex(0); triangle.addIndex(1); triangle.addIndex(2);
                triangle.addIndex(2); triangle.addIndex(3); triangle.addIndex(0);
                
                ofFloatColor c;
                ofColor d = colorPair[ j % colorPair.size() ];
                c.set(d.r/255.0f,d.g/255.0f,d.b/255.0f);
                
                for ( auto & ind : triangle.getIndices()){
                    triangle.addColor(c);
                }
                
                meshes.push_back(triangle);
            }
        }
    }
    
    void draw(){
        vector<int> anInd;
        anInd.push_back(0);
        anInd.push_back(3);
        for (auto m : meshes){
            for ( auto i : anInd ){
                float x = m.getVertices()[i].x;
                float y = m.getVertices()[i].y;
                float eased = easeOut(ofMap(frame, m.animationDelay, m.animationDelay + m.animationTime, 0, 1.0, true));
                m.getVertices()[i].x = ofMap(eased, 0, 1.0, source.x, x, true);
                m.getVertices()[i].y = ofMap(eased, 0, 1.0, source.y, y, true);
            }
            m.draw();
        }
        frame++;
    }
    
};