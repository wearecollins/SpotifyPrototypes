#include "ofApp.h"

int nColors = 5;
bool bHideGui = false;

//--------------------------------------------------------------
void ofApp::setup(){
    for (int i=0; i<nColors; i++){
        
        if ( i % 2 == 0 ){
            guis.push_back( new ofxUICanvas(ofGetWidth()/6.0 * ((i) / 2), 0, ofGetWidth()/6.0, ofGetHeight()) );
            guis.back()->setColorBack(ofColor(255,0));
        }
        ofColor c, d;
        c.set(ofRandom(255.0),ofRandom(255.0),ofRandom(255.0));
        d.set(ofRandom(255.0),ofRandom(255.0),ofRandom(255.0));
        colorFilter.addColorPair(c, d);
        
        hues.push_back( c.getHue());
        sats.push_back( c.getSaturation());
        brights.push_back( c.getBrightness());
        
        if ( i % 2 == 0 ) guis.back()->addSpacer(0,20);
        guis.back()->addLabel("Color A: "+ofToString(i));
        guis.back()->addSlider("Hue", 0, 255.0, &hues.back());
        guis.back()->addSlider("Saturation", 0, 255.0, &sats.back());
        guis.back()->addSlider("Brightness", 0, 255.0, &brights.back());
        
        hues.push_back( d.getHue());
        sats.push_back( d.getSaturation());
        brights.push_back( d.getBrightness());
        
        guis.back()->addSpacer(0,30);
        guis.back()->addLabel("Color B: "+ofToString(i));
        guis.back()->addSlider("Hue", 0, 255.0, &hues[(i * 2 + 1)]);
        guis.back()->addSlider("Saturation", 0, 255.0, &sats[(i * 2 + 1)]);
        guis.back()->addSlider("Brightness", 0, 255.0, &brights[(i * 2 + 1)]);
        guis.back()->addSpacer(0,20);
        
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i=0; i<10; i++){
        vector<ofColor> * c = colorFilter.getColorPair(i);
        c->at(0).setHue(hues[i * 2]);
        c->at(0).setSaturation(sats[i * 2]);
        c->at(0).setBrightness(brights[i * 2]);
        
        c->at(1).setHue(hues[i * 2 + 1]);
        c->at(1).setSaturation(sats[i * 2 + 1]);
        c->at(1).setBrightness(brights[i * 2 + 1]);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( int i=0; i<nColors; i++){
        int x = ofGetWidth()/6.0 * ((i) / 2);
        int y = ( i % 2 == 0 ? 0 : 300);
        
        ofSetColor(colorFilter.getColorPair(i)->at(0));
        ofRect(x, y, ofGetWidth()/8.0, 15);
        ofSetColor(colorFilter.getColorPair(i)->at(1));
        ofRect(x, y + 150, ofGetWidth()/8.0, 15);
    }
    ofSetColor(255);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ){
        bHideGui = !bHideGui;
        for ( auto * g : guis ){
            g->setVisible(bHideGui);
        }
    }
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
        colorFilter.loadAndProcess(s);
        break;
    }
}
