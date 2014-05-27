#include "ofApp.h"

int numCircles = 10;
int numColors = 2;
float startRadius = 100;
float endRadius = 1000;
int easeType = 0;
int lastEaseType = -1;
float lastStart, lastEnd;

ofVec2f center, lastCenter;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetCircleResolution(100);
    gui = new ofxUICanvas(0,0,ofGetWidth()/4.0, ofGetHeight());
    gui->addIntSlider("num circles", 1, 100, &numCircles);
    gui->addIntSlider("num colors", 1, 100, &numColors);
    gui->addIntSlider("easing type", 0, 3, &easeType);
    gui->addSlider("start radius", 10, 10000, &startRadius);
    gui->addSlider("end radius", 100, 10000, &endRadius);
    gui->addSlider("origin: x", -1000, 1000, &center.x);
    gui->addSlider("origin: y", -1000, 1000, &center.y);
    gui->toggleVisible();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( toLoad.size() != 0 ){
        for ( string & s : toLoad ){
            images.push_back(ofImage());
            images.back().loadImage(s);
            
            float sc = fmax ( (float) ofGetWidth()/images.back().width, (float) ofGetHeight()/images.back().height);
            images.back().resize(images.back().width * sc, images.back().height * sc);
        }
        toLoad.clear();
    }
    
    if ( toLoadOverlay != "" ){
        overlay.loadImage(toLoadOverlay);
        toLoadOverlay = "";
    }
    
    
    if ( colors.size() != numColors ){
        colors.clear();
        for ( int i=0; i<numColors; i++){
            colors.push_back(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
            colors.back().setSaturation(100);
            colors.back().setBrightness(200);
        }
    }
    
    if ( circles.size() != numCircles || lastStart != startRadius || lastEnd != endRadius
        || lastEaseType != easeType){
        lastStart = startRadius;
        lastEnd = endRadius;
        lastEaseType = easeType;
        
        circles.clear();
        for ( int i=0; i<numCircles; i++){
            float radius = 0;
            float t = ofMap(i, 0, numCircles, 1.0, 0.0);
            switch (easeType) {
                case 0:
                    radius = ofMap(easingLinear.easeOut(t), 0.0, 1.0, startRadius, endRadius);
                    break;
                case 1:
                    radius = ofMap(easingCubic.easeOut(t), 0.0, 1.0, startRadius, endRadius);
                    break;
                case 2:
                    radius = ofMap(easingCubic.easeIn(t), 0.0, 1.0, startRadius, endRadius);
                    break;
                case 3:
                    radius = ofMap(easingQuad.easeIn(t), 0.0, 1.0, startRadius, endRadius);
                    break;
            }
            circles.push_back(ofMeshCircle());
            circles.back().generate(radius);
        }
    }
    
    
    if ( lastCenter != center ){
        lastCenter.set(center);
        
        for ( auto & c : circles ){
            for ( auto & t : c.getIndices() ){
                c.setTexCoord(t, c.getVertex(t) + center );
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( int i=0; i<numCircles; i++){
        int index = -1;
        if ( images.size() > 0 ){
            if ( images.size() == 1 ){
                if ( i % 2 == 0 ) index = -1;
                    else index = 0;
            } else {
                index = i % images.size();
            }
        }
        ofSetColor(colors[i % colors.size()]);
        if ( index != -1 ) images[index].bind();
        ofPushMatrix();
        ofTranslate(center);
        circles[i].draw();
        ofPopMatrix();
        if ( index != -1 ) images[index].unbind();
    }
    if ( overlay.isAllocated() ) overlay.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key =='g') gui->toggleVisible();
    else if ( key == 'c' ){
        colors.clear();
        images.clear();
        overlay.clear();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    if ( dragInfo.position.x > ofGetWidth() * 3./4.){
        toLoadOverlay = dragInfo.files[0];
    } else {
        toLoad.swap(dragInfo.files);
    }
}
