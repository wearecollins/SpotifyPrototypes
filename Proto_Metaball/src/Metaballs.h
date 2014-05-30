//
//  Metaballs.h
//  ShapeGenMetaball
//
//  Created by Brett Renfer on 5/2/14.
//
//

#pragma once

#include "ofMain.h"

class Metaballs {
public:
    
    void setup( float rad = 300, float sep = 200.0 ){
        center.set(ofGetWidth()/2.0, ofGetHeight()/2.0);
        separation = sep;
        numBalls = 20;
        color.set(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0));
        color.setSaturation(.5);
        metaball.load("metaball");
        mesh.addVertex(ofVec2f(0,0));
        mesh.addVertex(ofVec2f(ofGetWidth(),0));
        mesh.addVertex(ofVec2f(ofGetWidth(),ofGetHeight()));
        mesh.addVertex(ofVec2f(0,ofGetHeight()));
        
        mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
        mesh.addIndex(0); mesh.addIndex(3); mesh.addIndex(2);
        
        ofVec2f center(ofRandom(-ofGetWidth()/10.0, ofGetWidth()/10.0),
                       ofRandom(-ofGetHeight()/10.0, ofGetHeight()/10.0));
        
        for ( int i=0; i<numBalls; i++){
            radiuses[i].x = starts[i].x = center.x + (ofRandom(-rad, rad));
            radiuses[i].y = starts[i].y = center.y + (ofRandom(-rad, rad));
            radiuses[i].z = (ofRandom(rad / 3.0, rad * 2.0));
        }
    }
    
    void update(){
        float hRatio = (float)  ofGetHeight()/ofGetWidth();
        for ( int i=0; i<numBalls; i++){
            radiuses[i].x = center.x + starts[i].x * separation/500.0 + ofSignedNoise(ofGetElapsedTimeMillis() * .0001 + i * .1) * separation;
            radiuses[i].y = center.y + starts[i].y * separation/500.0 + ofSignedNoise(ofGetElapsedTimeMillis() * .0002 + i * .2) * (separation * hRatio);
        }
    }
    
    void draw(){
        ofPushMatrix();
        metaball.begin();
        metaball.setUniform1i("numBalls", numBalls);
        metaball.setUniform3fv("balls", (float*) radiuses, numBalls);
        metaball.setUniform4f("color", color.r, color.g, color.b, color.a);
        mesh.draw();
        metaball.end();
        ofPopMatrix();
    }
    
    ofFloatColor color;
    float separation;
    
    ofVec2f center;
    
protected:
    ofMesh mesh;
    ofShader metaball;
    
    ofVec3f radiuses[100];
    ofVec3f starts[100];
    int     numBalls, ballSub;
};
