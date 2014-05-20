#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( toLoad.size() != 0){
        vector<ofImage> images;
        vector<float> xes;
        for ( auto & f : toLoad ){
            images.push_back(ofImage());
            images.back().loadImage(f);
            xes.push_back(0);
        }
        toLoad.clear();
    
        float columnWidth = 3;
        float xInc = ofRandom(.01,10.0);
        float rate = ofRandom(.0001, .1);
        float wave = ofRandom(10,100.0);
        
        bool bRandom = false;
        
        ofPixels pix;
        pix.allocate(ofGetWidth(), ofGetHeight(), 3);
        float index = 0;
        bool bHorizontal = false;
        
        if ( bHorizontal ){
            for ( int x=0; x<ofGetWidth(); x++){
                index += 1.0/columnWidth;
                
                int indexInd = (int) floor(index) % images.size();
                
                float offSet = bRandom ? ofRandom(-100,100) : ofSignedNoise(ofMap(x, 0, ofGetWidth(), 0, ofGetWidth() * rate) + ofGetElapsedTimeMillis() * rate) * wave;
                
                for ( int y=0; y<ofGetHeight(); y++){
                    int colorY = ofMap(y, 0, ofGetHeight(), 0, images[indexInd].height);
                    pix.setColor(x, y, images[indexInd].getColor(xes[indexInd], ofWrap(colorY + offSet, 0, images[indexInd].height) ));
                }
                xes[indexInd] += xInc;
                while ( xes[indexInd] > images[indexInd].width ){
                    xes[indexInd] = (int) xes[indexInd] % (int) images[indexInd].width;
                }
            }
        } else {
            for ( int y=0; y<ofGetHeight(); y++){
                index += 1.0/columnWidth;
                
                int indexInd = (int) floor(index) % images.size();
                
                float offSet = bRandom ? ofRandom(-100,100) : ofSignedNoise(ofMap(y, 0, ofGetHeight(), 0, ofGetHeight() * rate) + ofGetElapsedTimeMillis() * rate) * wave;
            
                for ( int x=0; x<ofGetWidth(); x++){
                    int colorX = ofMap(x, 0, ofGetWidth(), 0, images[indexInd].width);
                    pix.setColor(x, y, images[indexInd].getColor( ofWrap(colorX + offSet, 0, images[indexInd].width), xes[indexInd]) );
                }
                xes[indexInd] += xInc;
                while ( xes[indexInd] > images[indexInd].height ){
                    xes[indexInd] = (int) xes[indexInd] % (int) images[indexInd].height;
                }
            }
        }
        screen.setFromPixels(pix);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    screen.draw(0,0, ofGetWidth(), ofGetHeight());
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
    toLoad.swap(dragInfo.files);
}
