#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	k.setup(1024,768);
	grabber.initGrabber(320, 240);
	ofHideCursor();
	ofToggleFullscreen();
    k.spinStep = 0.5;
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
    if ( grabber.isFrameNew() ){
        camera.setFromPixels(grabber.getPixels(), 320,240, OF_IMAGE_COLOR, true);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	k.render(ofGetWidth(), ofGetHeight(), camera);
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
