#include "ofApp.h"

bool bSaving      = false;
bool bDestructive = false;
bool bOpenAferSave = false;
ofImage temp;

float contrast    = 1.0;
float lastContrast    = 1.0;
ofColor c, d;

ofTrueTypeFont fontMedium;
ofImage logo;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot("./");
    ofSetVerticalSync(true);
    manager.setup();
    ofAddListener(manager.onLoaded, this, &ofApp::onNewImage);
    ofAddListener(manager.onLoadedFile, this, &ofApp::onFileLoaded);
    ofAddListener(saveButton.onPressed, this, &ofApp::saveImage);
    ofBackground(100);
    
    gui.setup("assets/fonts/CircularStd-Book.ttf");
    gui.x = floor(ofGetWidth() - gui.getWidth()-20);
    gui.y = 20;
    gui.attachFloat("contrast", &contrast);
    
    saveButton.setup("assets/fonts/CircularStd-Book.ttf");
    saveButton.x = ofGetWidth() - saveButton.width-20;
    saveButton.y = ofGetHeight() - saveButton.height-20;
    
    fontMedium.loadFont("assets/fonts/CircularStd-Medium.ttf", 30);
    logo.loadImage("assets/logo.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( manager.size() == 0 ){
        return;
    }
    vector<ofColor> colors = gui.getActive();
    
    bool process = ((colors.size() == 2 && (c != colors[0] || d != colors[1])) || lastContrast != contrast);
    
    if ( process ){
        temp.clone(manager.getImage(0));
        static cv::Mat mat;
        static cv::Mat p;
        mat = ofxCv::toCv(temp);
        mat.convertTo(p, -1,contrast,0);
        ofxCv::toOf(p, temp);
        temp.update();
        lastContrast = contrast;
    }
    
    if ( colors.size() < 2 ) return;
    if ( process ){
        filter.setColorPair(0, colors[1], colors[0]);
        filter.process(temp);
        c.set(colors[0]);
        d.set(colors[1]);
    }
    
    if ( bSaving ){
        string str = "image_"+ofGetTimestampString()+".png";
        temp.saveImage( "../../../" + str);
        bSaving = false;
        string cmd = "open "+ofToString(ofToDataPath(str, true));
        if (bOpenAferSave) system(cmd.c_str());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( temp.isAllocated() ){
        temp.draw(ofGetWidth()/2.0 - temp.width/2.0, ofGetHeight()/2.0 - temp.height/2.0);
    } else if ( manager.size() > 0 ){
        manager.getImage(0).draw(ofGetWidth()/2.0 - manager.getImage(0).width/2.0, ofGetHeight()/2.0 - manager.getImage(0).height/2.0);
    } else {
        float w = fontMedium.stringWidth("Drag image here");
        float h = fontMedium.stringHeight("Drag image here");
        fontMedium.drawString("Drag image here", ofGetWidth()/2.0 - w/2.0, ofGetHeight()/2.0 - h/2.0 + fontMedium.getSize());
    }
    ofSetColor(255);
    logo.draw(20,20);
}

//--------------------------------------------------------------
void ofApp::saveImage( bool & b ){
    bSaving = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed( int k ){
    if ( k == 'd' ) bDestructive = !bDestructive;
//    else if (k =='s') bSaving = true;
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
void ofApp::windowResized( int w, int h ){
    
    gui.x = floor(ofGetWidth() - gui.getWidth()-20);
    gui.y = 20;
    
    saveButton.x = ofGetWidth() - saveButton.width-20;
    saveButton.y = ofGetHeight() - saveButton.height-20;
}

//--------------------------------------------------------------
void ofApp::onNewImage( ofImage & img ){
    lastContrast = -1;
    c.set(255);
    d.set(255);
}

//--------------------------------------------------------------
void ofApp::onFileLoaded( string & img ){
    gui.setTitle(img);
}
