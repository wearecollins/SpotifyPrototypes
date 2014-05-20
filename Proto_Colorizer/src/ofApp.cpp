#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    colorA.setHue(ofRandom(255));
    colorA.setSaturation(200);
    colorA.setHue(200);
    
    colorB.setHue(ofRandom(255));
    colorB.setSaturation(200);
    colorB.setHue(200);
    ofSetVerticalSync(true);
    toLoad = "";
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( toLoad != "" ){
        ofFile f(toLoad);
        string output = f.getEnclosingDirectory();
        string name   = f.getBaseName() + "_colored";
        string ext    = f.getExtension();
        if ( img.loadImage(toLoad) ){
            img.setImageType(OF_IMAGE_GRAYSCALE);
//            img.setImageType(OF_IMAGE_COLOR);
            ofPixelsRef & p = img.getPixelsRef();
            p.setImageType(OF_IMAGE_COLOR);
            for ( int i=0; i<p.size(); i+=p.getNumChannels() ){
                p[i + 0] = ofLerp(colorA.r, colorB.r, 1.0 - p[i + 0]/255.0);
                p[i + 1] = ofLerp(colorA.g, colorB.g, 1.0 - p[i + 1]/255.0);
                p[i + 2] = ofLerp(colorA.b, colorB.b, 1.0 - p[i + 2]/255.0);
                
//                p[i + 0] = ofClamp(p[i + 0], 0, 255);
//                p[i + 1] = ofClamp(p[i + 1], 0, 255);
//                p[i + 2] = ofClamp(p[i + 2], 0, 255);
            }
            img.setFromPixels(p);
            img.saveImage( output + "/" + name + "." + ext);
        }
        toLoad = "";
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(colorA, colorB, OF_GRADIENT_LINEAR);
    ofDrawBitmapString("Drag an image here to colorize!", 20, 20);
    ofDrawBitmapString("Click and drag LR to change tint", 20, 40);
    ofDrawBitmapString("(Hold shift to change which color)", 20, 60);
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
