#pragma once

#include "ofMain.h"
#include "ofxBox2D.h"

static ofVec2f getCircleVec( float angle, float rad ){
    float x = cos( ofDegToRad(angle) ) * rad;
    float y = -sin( ofDegToRad(angle)) * rad;
    return ofVec2f(x,y);
}

class TextureCircle : public ofxBox2dCircle {
public:
    
    TextureCircle():ofxBox2dCircle(){
        bMeshCreated = false;
    }
    
    bool bMeshCreated;
    ofMesh m;
    
    virtual void draw(){
        if ( !bMeshCreated ){
            float res = 100;
            float inc = (float) 360.0 / res;
            
            int i = 0;
            
            ofVec2f center(0,0);
            ofFloatColor color(ofRandomuf(),ofRandomuf(),ofRandomuf());
            color.setBrightness(1.0);
            color.setSaturation(0.0);
            
            for ( float a = 0; a < 360.0; a+= inc ){
                ofVec2f vert(getCircleVec(a, getRadius()));
                m.addTexCoord(vert);
                m.addVertex(vert);
                m.addIndex(i);
                m.addColor(color);
                i++;
                
                vert.set(getCircleVec(a + inc, getRadius()));
                
                m.addTexCoord(vert);
                m.addVertex( vert );
                m.addIndex(i);
                m.addColor(color);
                i++;
                
                m.addTexCoord(center);
                m.addVertex(center);
                m.addIndex(i);
                m.addColor(color);
                i++;
            }
            bMeshCreated = true;
        }
        
        if(!isBody()) return;
        
        ofPushMatrix();
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofRotate(getRotation(), 0, 0, 1);
        
        for ( auto & ind : m.getIndices() ){
            ofVec2f vert = m.getVertex(ind);
            ofVec2f coord = m.getTexCoord(ind);
            m.setTexCoord( ind, vert + getPosition());
        }
        
        m.draw();
        ofPopMatrix();
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
		
        vector <ofPolyline>                 lines;
        ofxBox2d                            box2d;
        vector <ofPtr<TextureCircle> >		circles;
        vector <ofPtr<ofxBox2dEdge> >       edges;
    
        ofImage toBind;
        string toLoad;
    
        ofFbo trails;
};
