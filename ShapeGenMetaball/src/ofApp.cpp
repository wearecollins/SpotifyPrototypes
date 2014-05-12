#include "ofApp.h"

bool bRandomize = false;
ofPixels pix;

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofBackground(255);
    ofEnableAlphaBlending();
    render.allocate(ofGetWidth(), ofGetHeight());
    for ( int i=0; i<3; i++){
        metaballs.push_back(Metaballs());
        metaballs.back().setup();
    }
    contourFinder.setMinAreaRadius(20);
//    contourFinder.setMaxAreaRadius(1000);
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( bRandomize){
        metaballs.clear();
        for ( int i=0; i<3; i++){
            metaballs.push_back(Metaballs());
            metaballs.back().setup();
        }
        bRandomize = false;
    }
    for ( auto & m : metaballs ){
        m.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));
    ofPath p;
    
    for ( auto & m : metaballs ){
        render.begin();
        ofClear(0,0);
        m.draw();
        render.end();
        render.readToPixels(pix);
        contourFinder.findContours(pix);
        
        ofSetColor(255);
        for ( int i=0; i<contourFinder.size(); i++){
            p.clear();
            int cnt = ofMap(mouseY, 0, ofGetHeight(), fmax(3,contourFinder.getPolyline(i).size() / 100.0), contourFinder.getPolyline(i).size() / 2.0);
            ofPolyline pl = contourFinder.getPolyline(i).getResampledByCount( cnt );
            for ( int j=0; j<pl.size(); j++){
                p.lineTo(pl[j]);
            }
            p.setColor( ofColor(m.color.r * 255.0f, m.color.g * 255.0f, m.color.b * 255.0f, 150.0f) );
            p.draw();
        }
    }
    
    //render.draw(0,0);
    //contourFinder.draw();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') bRandomize = true;
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
