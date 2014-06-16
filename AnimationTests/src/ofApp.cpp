#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    currentShape = &bubbs;
    currentShape->generate();
    logo.setup();
    logo.x = ofGetWidth()/2.0;
    logo.y = ofGetHeight() * .75;
    logo.scale = (float)240.0/logo.width; //hm
    logo.drawCircle = true;
    logo.bDepthTest = false;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(currentShape->getCurrentColors()[2]);
    logo.setColor(currentShape->getCurrentColors()[0]);
    logo.circleColor = currentShape->getCurrentColors()[1];
    currentShape->draw();
    logo.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    mutex.lock();
    if ( key == 'g' ){
        int ran= floor(ofRandom(3));
        if ( ran == 0){
            currentShape = &spikes;
        } else if ( ran == 1 ){
            currentShape = &bubbs;
        } else {
            currentShape = &squares;
        }
        currentShape->generate();
    }
    mutex.unlock();
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
