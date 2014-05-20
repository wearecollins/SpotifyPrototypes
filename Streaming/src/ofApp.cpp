#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    colorA.setHue(ofRandomuf() * 255);
    colorA.setSaturation(200);
    colorA.setHue(200);
    
    colorB.setHue(100);
    colorB.setSaturation(200);
    colorB.setHue(200);
    ofSetVerticalSync(true);
    toLoad = "";
    
    float lineWidth = 10;
    
    float lineCountH = ofGetWidth() / lineWidth;
    float lineCountV = ofGetHeight() / lineWidth;
    
    float y = 0;
    float x = 0;
    bool b = true;
    for (x =0; x<ofGetWidth(); x+= lineWidth){
        patternA.addVertex(ofVec2f(x,y));
        patternB.addVertex(ofVec2f(x,y));
        patternA.addColor(colorA);
        patternB.addColor(colorB);
    }
    cout << patternA.getNumVertices() << endl;
    
    b = true;
    x = ofGetWidth();
    for (float y =0; y<ofGetHeight(); y+= lineWidth){
        patternA.addVertex(ofVec2f(x,y));
        patternB.addVertex(ofVec2f(x,y));
        patternA.addColor(colorA);
        patternB.addColor(colorB);
    }
    
    b = true;
    x = 0;
    for (float y =0; y<ofGetHeight(); y+= lineWidth){
        patternA.addVertex(ofVec2f(x,y));
        patternB.addVertex(ofVec2f(x,y));
        patternA.addColor(colorA);
        patternB.addColor(colorB);
    }
    
    b = true;
    y = ofGetHeight();
    for (x =0; x<ofGetWidth(); x+= lineWidth){
        patternA.addVertex(ofVec2f(x,y));
        patternB.addVertex(ofVec2f(x,y));
        patternA.addColor(colorA);
        patternB.addColor(colorB);
    }
    
    b = true;
    for ( int i=0; i<lineCountH + lineCountV - 2; i++){
        if ( b ){
            patternA.addIndex(i);
            patternA.addIndex(i+1);
            patternA.addIndex(i+(lineCountH + lineCountV));
        } else {
            patternA.addIndex(i);
            patternA.addIndex(i+(lineCountH + lineCountV));
            patternA.addIndex(i+(lineCountH + lineCountV)-1);
        }
        b = !b;
    }
    for ( int i=1; i<lineCountH + lineCountV - 2; i++){
        if ( b ){
            patternB.addIndex(i);
            patternB.addIndex(i+1);
            patternB.addIndex(i+(lineCountH + lineCountV));
        } else {
            patternB.addIndex(i);
            patternB.addIndex(i+(lineCountH + lineCountV));
            patternB.addIndex(i+(lineCountH + lineCountV)-1);
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    bool b = true;
    for (int i=0; i<patternA.getNumVertices(); i++){
        patternA.setColor(i, colorA);
    }
    for (int i=0; i<patternB.getNumVertices(); i++){
        patternB.setColor(i, colorB);
    }
    
    bool bNeedToEdit = lastColor != colorA ? true : false;
    
    if ( toLoad != "" ){
        ofFile f(toLoad);
        string output = f.getEnclosingDirectory();
        string name   = f.getBaseName() + "_colored";
        string ext    = f.getExtension();
        if ( img.loadImage(toLoad) ){
            unedited.setFromPixels(img.getPixels(), img.width, img.height, img.getPixelsRef().getImageType());
            unedited.setImageType(OF_IMAGE_COLOR_ALPHA);
            
            bNeedToEdit = true;
        }
        toLoad = "";
    }
    
    if ( bNeedToEdit && img.isAllocated() ){
        ofPixels ref(unedited);
        
        for ( int i=0; i<unedited.size(); i+=unedited.getNumChannels() ){
            //p[i + 0] = ofLerp(colorA.r, colorB.r, 1.0 - p[i + 0]/255.0);
            //p[i + 1] = ofLerp(colorA.g, colorB.g, 1.0 - p[i + 1]/255.0);
            //p[i + 2] = ofLerp(colorA.b, colorB.b, 1.0 - p[i + 2]/255.0);
            ofColor c(unedited[i],unedited[i+1],unedited[i+2]);
            
            float bright = c.getHue();
            float bright2 = colorA.getHue();
            ref[i+3] = ofMap(abs( bright - bright2), 0, 100, 0, 255, true) ;
        }
        img.setFromPixels(ref);
    }
    lastColor.set(colorA);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("Drag an image here to colorize!", 20, 20);
    ofDrawBitmapString("Click and drag LR to change tint", 20, 40);
    ofDrawBitmapString("(Hold shift to change which color)", 20, 60);
    
    patternA.draw();
    patternB.draw();
    if ( img.bAllocated() ) img.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::drawPattern(){
    
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
    if ( ofGetKeyPressed(OF_KEY_SHIFT)){
        colorA.setHue((float) x / ofGetWidth() * 255.0 );
    } else {
        colorB.setHue((float) x / ofGetWidth() * 255.0 );
    }
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
