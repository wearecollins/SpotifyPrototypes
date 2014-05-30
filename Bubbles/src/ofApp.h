#pragma once

#include "ofMain.h"

#include "Logo.h"
#include "ofxRCUtils.h"
#include "ImageManager.h"
#include "ofxLabFlexParticleSystem.h"
#include "ofxUI.h"
#include "CvFilters.h"

class SParticle : public lab::Particle {
public:
    SParticle( float x, float y ) :
    Particle(x,y){
        bCreated = false;
        bTransparent = false;
        bLocked = false;
        setDefaults();
        set(x,y);
        pos.set(x,y);
        res = 100;
        noiseFactor = 1.0;
        complexity = 1.0;
    }
    
    void draw(){
        mesh.clear();
        if ( bLocked ) set(pos);
        mesh = rc::ofMeshCircle::circle(radius * 1.3 * (2.0 - 1.0 * complexity) , x, y, res * complexity );
        
        for ( auto & i : mesh.getIndices()){
            ofVec3f v = mesh.getVertex(i);
            v.x += ofSignedNoise( (v.x + v.y) * .01 + ofGetElapsedTimeMillis() * .002) * (radius * noiseFactor);
            v.y += ofSignedNoise( (v.x + v.y) * .02 + ofGetElapsedTimeMillis() * .001) * (radius * noiseFactor);
            mesh.setVertex(i, v);
            mesh.setTexCoord(i,v);
        }
        
        ofPushMatrix();
        ofTranslate(0,0,z);
        if ( !bTransparent ){
            mesh.draw();
        }
        ofPopMatrix();
    }
    bool bTransparent, bLocked;
    
    int res;
    
    float complexity;
    
    float noiseFactor;
    
protected:
    ofVec3f pos;
    ofMesh mesh;
    bool bCreated;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        void toggleGuiVisible();
    
        Logo logo;
        vector<ofMesh>  logoMeshes;
        ImageManager        imageManager;
        ColorManager        colorManager;
        rc::KeyManager      keyManager;
        ScreenManager       screen;
    
        ColorFilter     colors;
        ofColor colorA, colorB;
    
        // particles
        lab::ParticleSystem particles;
    
        // gui
        ofxUICanvas * gui;
};
