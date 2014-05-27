#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxEasingFunc.h"

static ofVec2f getCircleVec( float angle, float rad ){
    float x = cos( ofDegToRad(angle) ) * rad;
    float y = -sin( ofDegToRad(angle)) * rad;
    return ofVec2f(x,y);
}

class ofMeshCircle : public ofMesh {
public: void generate( float radius, float res = 100 ){
        clear();
        float inc = (float) 360.0 / res;
        
        int i = 0;
        
        ofVec2f center(0,0);
        ofFloatColor color(ofRandomuf(),ofRandomuf(),ofRandomuf());
        color.setBrightness(1.0);
        color.setSaturation(0.0);
        
        for ( float a = 0; a < 360.0; a+= inc ){
            ofVec2f vert(getCircleVec(a, radius));
            addTexCoord(vert);
            addVertex(vert);
            addIndex(i);
            //addColor(color);
            i++;
            
            vert.set(getCircleVec(a + inc, radius));
            
            addTexCoord(vert);
            addVertex( vert );
            addIndex(i);
            //addColor(color);
            i++;
            
            addTexCoord(center);
            addVertex(center);
            addIndex(i);
            //addColor(color);
            i++;
        }
    }
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
		
        ofxUICanvas * gui;
    
        ofxEasingFunc::Sine easingCubic;
        ofxEasingFunc::Linear easingLinear;
        ofxEasingFunc::Expo easingQuad;
    
        vector<ofMeshCircle> circles;
        ofImage overlay;
    string toLoadOverlay;
    
        vector<ofImage> images;
        vector<ofColor> colors;
    
        vector<string> toLoad;
};
