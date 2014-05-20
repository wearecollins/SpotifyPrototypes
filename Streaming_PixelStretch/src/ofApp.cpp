#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    xOffset = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( toLoad != "" ){
        ofFile f(toLoad);
        string output = f.getEnclosingDirectory();
        string name   = f.getBaseName() + "_colored";
        string ext    = f.getExtension();
        if ( img.loadImage(toLoad) ){
            float x = ofRandom(0, img.width/2.0);
            width = ofRandom(x/4.0,x);
            left.setFromPixels(img.getPixelsRef());
            center.setFromPixels(img.getPixelsRef());
            right.setFromPixels(img.getPixelsRef());
            
            left.crop(0, 0, x, img.height);
            center.crop(x, 0, 1, img.height);
            right.crop(x+1, 0, img.width-x, img.height);
            xOffset = x;
        }
        toLoad = "";
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( img.isAllocated() ){
        ofPushMatrix();
        //ofTranslate(-xOffset, 0);
        left.draw(0,0);
        ofTranslate(left.width, 0);
        center.draw(0,0,xOffset,center.height);
        ofTranslate(xOffset, 0);
        right.draw(0,0);
        ofPopMatrix();
    }
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
    for ( auto & s : dragInfo.files ){
        toLoad = s;
        break;
    }
}
