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
    
    void setup(){
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
        
        ofVec2f center(ofGetWidth()/2.0 + ofRandom(-ofGetWidth()/10.0, ofGetWidth()/10.0),
                       ofGetHeight()/2.0 + ofRandom(-ofGetHeight()/10.0, ofGetHeight()/10.0));
        
        for ( int i=0; i<numBalls; i++){
            radiuses[i].x = starts[i].x = center.x + (ofRandom(-300,300));
            radiuses[i].y = starts[i].y = center.y + (ofRandom(-300, 300));
            radiuses[i].z = (ofRandom(100., 600.0));
        }
    }
    
    void update(){
        for ( int i=0; i<numBalls; i++){
            radiuses[i].x = starts[i].x + ofSignedNoise(ofGetElapsedTimeMillis() * .0001 + i * .1) * 200.0;
            radiuses[i].y = starts[i].y + ofSignedNoise(ofGetElapsedTimeMillis() * .0002 + i * .2) * 200.0;
        }
    }
    
    void draw(){
        ofPushMatrix();
        metaball.begin();
        metaball.setUniform3fv("balls", (float*) radiuses, numBalls);
        metaball.setUniform4f("color", color.r, color.g, color.b, color.a);
        mesh.draw();
        metaball.end();
        ofPopMatrix();
    }
    
    ofFloatColor color;
    
protected:
    ofMesh mesh;
    ofShader metaball;
    
    ofVec3f radiuses[50];
    ofVec3f starts[50];
    int     numBalls, ballSub;
};
