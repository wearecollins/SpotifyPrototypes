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
    
    Metaballs(){
        ofAddListener(ofEvents().mousePressed, this, &Metaballs::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &Metaballs::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &Metaballs::mouseReleased);
        bInteractive = false;
    }
    
    void setup( float rad = 300, float sep = 200.0 ){
        bDragging = false;
        center.set(ofGetWidth()/2.0, ofGetHeight()/2.0);
        separation = sep;
        numBalls = 10;
        color.set(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0));
        color.setSaturation(.5);
        metaball.load("shaders/metaball");
        mesh.clear();
        mesh.addVertex(ofVec2f(0,0));
        mesh.addVertex(ofVec2f(ofGetWidth(),0));
        mesh.addVertex(ofVec2f(ofGetWidth(),ofGetHeight()));
        mesh.addVertex(ofVec2f(0,ofGetHeight()));
        
        mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
        mesh.addIndex(0); mesh.addIndex(3); mesh.addIndex(2);
        
        baseRadius = rad;
        maxRadThresh = -99999;
//        
//        ofVec2f center(ofRandom(-ofGetWidth()/10.0, ofGetWidth()/10.0),
//                       ofRandom(-ofGetHeight()/10.0, ofGetHeight()/10.0));
//        
        int cnt = 0;
        int max = 2;
        for ( int i=0; i<numBalls; i++){
            radiuses[i].z = (ofRandom(rad, rad * 1.1));
            if ( ofRandom(100) > 80 && cnt < max){
                radiuses[i].z = ofRandom(rad * 2.0, rad * 4.0);
                cnt++;
            };
            if ( radiuses[i].z > maxRadThresh ) maxRadThresh = radiuses[i].z;
            radiuses[i].x = starts[i].x = center.x + (ofRandom(-radiuses[i].z/2.0, radiuses[i].z/2.0));
            radiuses[i].y = starts[i].y = center.y + (ofRandom(-radiuses[i].z/2.0, radiuses[i].z/2.0));
        }
        
        if ( cnt < max ){
            for ( int i = cnt; i<max; i++){
                int ind = (int) floor(ofRandom(numBalls));
                radiuses[ind].z = ofRandom(rad * 2.0, rad * 4.0);
                if ( radiuses[ind].z > maxRadThresh ) maxRadThresh = radiuses[ind].z;
            }
        }
//        maxRadThresh *= .5;
    }
    
    void update(){
        float hRatio = 1.0;//(float)  ofGetHeight()/ofGetWidth();
        float separationX = ofMap(separation, 0.0, 500.0, 0.0, ofGetWidth()/2.0);
        float separationY = ofMap(separation, 0.0, 500.0, 0.0, ofGetHeight()/2.0);
        float separationInvX = ofMap(separation, 0.0, 500.0, ofGetWidth()/2.0, 0.0);
        float separationInvY = ofMap(separation, 0.0, 500.0, ofGetHeight()/2.0, 0.0);
        float mass = 3.0;
        
        for ( int i=0; i<numBalls; i++){
            radiuses[i].x = separationInvX + starts[i].x * separation/500.0 +
                                                     ofSignedNoise(ofGetElapsedTimeMillis() * .0001 + i * .1) * separationX;
            radiuses[i].y = separationInvY + starts[i].y * separation/500.0 +
                                                      ofSignedNoise(ofGetElapsedTimeMillis() * .0002 + i * .2) * separationY;
            
            if (false){
                ofVec2f acc;
                ofVec2f c(ofGetWidth()/2.0, ofGetHeight()/2.0);
                
                if ( radiuses[i].z < maxRadThresh ){
                    for ( int j=0; j<numBalls; j++){
                        if ( radiuses[j].z >= maxRadThresh ){
                            ofVec2f force = ofVec2f(radiuses[i].x, radiuses[i].y) - ofVec2f(radiuses[j].x, radiuses[j].y);
                            float distance = force.length();
                            distance = ofSign(distance) * ofClamp(fabs(distance), 0.0, 25.0); // hm
                            force.normalize();
                            force *= ( (forceLimit * forceLimit) / (distance * distance) );
                            acc.x -= force.x;
                            acc.y -= force.y;
                        }
                    }
                } else {
                    ofVec2f force = ofVec2f(radiuses[i].x, radiuses[i].y) - c;
                    float distance = force.length();
                    distance = ofSign(distance) * ofClamp(fabs(distance), 0.0, 25.0); // hm
                    force.normalize();
                    force *= ( (forceLimit * forceLimit) / (distance * distance) );
                    acc.x -= force.x;
                    acc.y -= force.y;
                }
                
                //acc.x = ofSign(acc.x) * ofClamp(fabs(acc.x), 0, forceLimit);
                //acc.y = ofSign(acc.y) * ofClamp(fabs(acc.y), 0, forceLimit);
                
    //            radiuses[i].x -= acc.x;//radiuses[i].x * .7 + (radiuses[i].x + acc.x) * .3;
    //            radiuses[i].y -= acc.y;// radiuses[i].y * .7 + (radiuses[i].y + acc.y) * .3;;
                
                radiuses[i].x = fmin( ofGetWidth(), fmax(0, radiuses[i].x + acc.x));
                radiuses[i].y = fmin( ofGetHeight(), fmax(0,radiuses[i].y + acc.y));
            }
        }
    }
    
    void draw(){
        ofPushMatrix();
        metaball.begin();
        metaball.setUniform1f("iso", iso);
        metaball.setUniform1i("numBalls", numBalls);
        metaball.setUniform3fv("balls", (float*) radiuses, numBalls);
        metaball.setUniform4f("color", color.r, color.g, color.b, color.a);
        mesh.draw();
        metaball.end();
        ofPopMatrix();
    }
    
    void mousePressed( ofMouseEventArgs & m ){
        lastRadius = 0;
//        which = 0;
        bDragging = bInteractive && ofGetKeyPressed( OF_KEY_SHIFT );
        if ( bDragging ){
            radiuses[which].z = (ofRandom(baseRadius, baseRadius * 1.1));
            if ( ofRandom(100) > 80){
                radiuses[which].z = ofRandom(baseRadius * 2.0, baseRadius * 4.0);
            };
            radiuses[which].x = starts[which].x = m.x;
            radiuses[which].y = starts[which].y = m.y;
            lastRadius = radiuses[which].z;
            which++;
            which = which % 10;
        }
    }
    
    void mouseDragged( ofMouseEventArgs & m ){
        
    }
    
    void mouseReleased( ofMouseEventArgs & m ){
        bDragging = false;
    }
    
    ofFloatColor color;
    float separation;
    float forceLimit;
    float iso;
    bool bInteractive;
    
    ofVec2f center;
    
protected:
    float maxRadThresh;
    ofMesh mesh;
    ofShader metaball;
    
    float baseRadius;
    
    bool bDragging;
    int  which, lastRadius;
    ofVec2f lastMouse;
    
    ofVec3f radiuses[100];
    ofVec3f starts[100];
    int     numBalls, ballSub;
};
