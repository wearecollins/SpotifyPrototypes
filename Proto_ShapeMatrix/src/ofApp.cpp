#include "ofApp.h"

int numPoints = 10;
int numShapes = 1;
int numColors = 1;

using namespace rc;

//--------------------------------------------------------------
void ofApp::setup(){
    regen();
}

//--------------------------------------------------------------
void ofApp::update(){
    int tn = ofMap(mouseX, 0, ofGetWidth(), 3, 20, true);
    int tc = ofMap(mouseY, 0, ofGetHeight(), 1, 100, true);
    if ( tn != numPoints || tc != numShapes){
        numPoints = tn;
        numShapes = tc;
        numColors = ceil(tc / 3.0);
        regen();
    }
}

//--------------------------------------------------------------
void ofApp::regen(){
    shapes.clear();
    colors.clear();
    
    for (int i=0; i<numShapes; i++){
        int index = i;
        if ( i < numColors ){
            colors.push_back(ofRandomRCColor());
        } else {
            index = i % colors.size();
        }
        float rad = ofRandom(10,100);
        shapes.push_back(ofMeshCircle::circle(rad, numPoints));
        ofVec2f center(ofGetWidth()/2.0 + ofRandom(-rad * 3.0, rad * 3.0),
                       ofGetHeight()/2.0 + ofRandom(-rad * 3.0, rad * 3.0));
        ofVec2f sc(ofRandom(1,2), 1.0);
        float rot = ofRandom(360);
        for ( auto & v : shapes.back().getVertices()){
//            if ( numPoints == 3 ) v *= sc;
            v.rotate(rot, ofVec2f(0,0));
            v += center;
            shapes.back().addColor(colors[index]);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    for ( auto & shape : shapes ){
        shape.draw();
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

}
