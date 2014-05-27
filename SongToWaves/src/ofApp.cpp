#include "ofApp.h"

int numpoints = 10000;

//--------------------------------------------------------------
void ofApp::setup(){
    bool bMeshExists = false;
    string fileName = "";
    ofDirectory dir;
    for ( int i=0; i<dir.listDir("songs"); i++){
        mesh.load(dir.getPath(0), numpoints);
        break;
    }
}

float lastX = -1;
float lastY = -1;

//--------------------------------------------------------------
void ofApp::update(){
    
    if ( mouseX != lastX || mouseY != lastY ){
        lastX = mouseX;
        lastY = mouseY;
        
        float smooth = ofMap(mouseX, 0, ofGetWidth(), 0.0, 500.0);
        float points = ofMap(mouseY, 0, ofGetHeight(), 10.0, numpoints);
        mesh.smooth(smooth, points);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    mesh.draw(0.0, ofGetHeight()/2.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

}
