#include "ofApp.h"

vector<ofColor> colors;
float minArea   = 0;
float maxArea   = 1920 * 1080;

int numFinder   = 3;

bool bDrawPhoto = true;

float contrast = 1.0;
float lastContrast = -1;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    gui = new ofxUICanvas(0,0, ofGetWidth()/4, ofGetHeight());
    for ( int i=0; i<numFinder; i++){
        contourFinders.push_back( new ThreadedContourFinder());
        gui->addMinimalSlider ("threshold_"+ofToString(i), 0, 255, &contourFinders[i]->threshold);
        gui->addMinimalSlider("smoothing_"+ofToString(i), 0, 255, &contourFinders[i]->polylineSimplify);
        gui->addMinimalSlider("minArea", 0, ofGetWidth() * ofGetHeight(), &contourFinders[i]->minArea);
        gui->addMinimalSlider("maxArea", 0, ofGetWidth() * ofGetHeight(), &contourFinders[i]->maxArea);
        gui->addMinimalSlider("contrast", .1, 3.0, &contourFinders[i]->contrast);
        gui->addToggle("invert", &contourFinders[i]->bInvert);
        ofColor c(ofRandom(255),ofRandom(255),ofRandom(255));
        c.setSaturation(100);
        c.setBrightness(255);
        colors.push_back(c);
        
        contourFinders[i]->setup();
    }
    gui->addSpacer();
    gui->addMinimalSlider("Contrast", 0.01, 3.0, &contrast);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(),3));
    if ( toLoad != "" ){
        img.loadImage(toLoad);
        toLoad = "";
        lastContrast = -1;
        for (int i=0; i<numFinder; i++){
            colors[i].set(ofRandom(255),ofRandom(255),ofRandom(255));
            colors[i].setSaturation(100);
            colors[i].setBrightness(255);
            contourFinders[i]->loadPixels(img);
        }
    }
    
    if ( img.isAllocated()){
        cv::Mat cv = ofxCv::toCv(img);
        cv::Mat dest;
        if ( contrast != lastContrast ){
            cv.convertTo(dest, -1,contrast,0);
            ofxCv::toOf(dest, drawImg);
            drawImg.update();
        }
        lastContrast = contrast;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    if ( bDrawPhoto && drawImg.isAllocated()) drawImg.draw(0,0);
    int i=0;
    for ( auto * f : contourFinders ){
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
        //ofRotate(i*5);
        ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
        //ofTranslate(i*10.0, i*10.0);
        ofSetColor(colors[i]);
        drawImg.bind();
        f->getMesh().draw();
        drawImg.unbind();
        ofPopMatrix();
        i++;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ) gui->toggleVisible();
    else if ( key == 'p' ) bDrawPhoto = !bDrawPhoto;
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
