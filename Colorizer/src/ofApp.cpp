#include "ofApp.h"

bool bSaving      = false;
bool bDestructive = false;
bool bOpenAferSave = false;
ofImage temp;

float contrast    = 1.0;
float lastContrast    = 1.0;
ofColor c, d;

//--------------------------------------------------------------
void ofApp::setup(){
    manager.setup();
    colorManager.setup("colors.xml");
    ofAddListener(manager.onLoaded, this, &ofApp::onNewImage);
    ofBackground(255);
    
    string draw = "Drag an image to the window to process.";
    string draw2 = "Click to set WHITE. Hold SHIFT to set Black.";
//    draw += "\nPress 's' to save.";
    gui = new ofxUICanvas( ofGetWidth() - 200, 0, 200, 200);
    gui->addTextArea("text", draw);
    gui->addSpacer();
    gui->addTextArea("text2", draw2);
    gui->addSpacer();
    gui->addSlider("Contrast", 1.0, 5.0, &contrast);
    
    gui->addToggle("Open after save", &bOpenAferSave);
    gui->addToggle("Save", &bSaving);
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( manager.size() == 0 ){
        return;
    }
    vector<ofColor> colors = colorManager.getActive();
    
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
        temp.saveImage(str);
        bSaving = false;
        string cmd = "open "+ofToString(ofToDataPath(str, true));
        if (bOpenAferSave) system(cmd.c_str());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( temp.isAllocated() ){
        temp.draw(0,0);
    } else if ( manager.size() > 0 ){
        manager.getImage(0).draw(0,0);
    }
    
    colorManager.draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed( int k ){
    if ( k == 'd' ) bDestructive = !bDestructive;
//    else if (k =='s') bSaving = true;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    colorManager.checkHit(x, y, false, false);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    colorManager.checkHit(x, y, true, !ofGetKeyPressed(OF_KEY_SHIFT));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::onNewImage( ofImage & img ){
    lastContrast = -1;
    c.set(255);
    d.set(255);
}
