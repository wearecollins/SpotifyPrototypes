#include "ofApp.h"

ofTessellator tess;

bool bRandomizeColor = false;
bool bAnimate        = true;
bool bDrawLogo       = true;
bool bClear         = true;
bool bColorBg         = false;
bool bBgFg          = false;
bool bTexLogo       = false;
bool bInvert        = false;

bool bSave          = false;

float contrast    = 1.0;
float lastContrast = 1.0;

using namespace rc;

FilterContrast  contrastFilter;

ofImage             toBind;
cv::Mat process;

float width = 0;
float height = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    logo.setup();
    
    imageManager.setup();
    ofAddListener(imageManager.onLoaded, this, &ofApp::onImageLoaded );
    colorManager.setup();
    
    bubbs.setup();
    
    // colors
    vector<ofColor> hl = colorManager.getHighLowPair();
    colorA = hl[0];//rc::ofRandomRCColor();
    colorB = hl[1];//rc::ofRandomRCColor();
    ofBackground(255);
    colors.addColorPair(colorA, colorB);
    //imageManager.addFilter(&colors);
    logo.setColor( colorB );
    
    gui = new ofxUICanvas(0,0, ofGetWidth()/2.0, ofGetHeight()/2.0);
    gui->addToggle("Animation", &bAnimate);
    gui->addToggle("Draw Logo", &bDrawLogo);
    gui->addToggle("Clear", &bClear);
    gui->addToggle("Color background", &bColorBg);
    gui->addToggle("Color bg with fg", &bBgFg);
    
    gui->addSlider("zRange", -100, 0, &bubbs.zRange);
    gui->addSlider("density", 0.01, 1.00, &bubbs.density);
    gui->addSlider("Noise factor", 0.01, 1.00, &bubbs.noiseFactor);
    gui->addSlider("Shape complexity", 0.01, 1.00, &bubbs.complexity);
    
    gui->addToggle("Texture logo", &bTexLogo);
    gui->addSlider("Logo Scale", .5, 10.00, &logo.scale);
    gui->addSlider("Contrast", 0.5, 5.00, &contrast);
    gui->addToggle("Invert", &bInvert);
    
    // keys
    keyManager.add('r', &bRandomizeColor);
    keyManager.add(' ', &bAnimate);
    keyManager.add('s', &bSave);
    
    KeyManager::Delegate d = tr1::bind(&ofApp::toggleGuiVisible, this );
    keyManager.addTrigger('g', d );
    
    screen.setup();
}


//--------------------------------------------------------------
void ofApp::toggleGuiVisible(){
    gui->toggleVisible();
}

void ofApp::onImageLoaded( ofImage & img ){
    bubbs.burst();
    bRandomizeColor = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( bColorBg ) ofBackground(bBgFg ? colorB : colorA);
    else ofBackground(255);
    
    if ( bRandomizeColor ){
        lastContrast = -1;
        bRandomizeColor = false;
        
        vector<ofColor> hl = colorManager.getHighLowPair();
        colorA = hl[1];
        colorB = hl[0];
        colors.setColorPair(0,colorA, colorB);
        
        logo.setColors( colorA, colorB );
    }
    
    bool bInd = imageManager.size() > 0;
    
    if ( bInd && contrast != lastContrast){
        lastContrast = contrast;
        process = ofxCv::toCv(imageManager.getRawImage(0));
        if ( bInvert ) ofxCv::invert(process);
        contrastFilter.amount = contrast;
        contrastFilter.process(process);
        toBind.clear();
        ofxCv::toOf(process, toBind);
        toBind.update();
        colors.process(toBind);
    }
    
    if ( bClear ){
        bClear = false;
        bubbs.clear();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    bool bInd = toBind.isAllocated();
    string name;
    
    ofColor bg;
    if ( bColorBg ) bg.set(bBgFg ? colorB : colorA);
    else  bg.set(255);
    
    if ( bSave ){
        name = "particles_" + ofGetTimestampString();
        screen.startSave( name + ".png", bg);
        bubbs.resize(ofGetWidth() * 3.0, ofGetHeight() * 3.0);
    }
    
    ofEnableDepthTest();
    float dir = 1;
    int i = 0;
    if ( bInd){
        toBind.bind();
    }
    bubbs.draw();
    if ( bInd ){
        toBind.unbind();
    }
    
    if ( bSave ){
        screen.endSave();
        bubbs.resize(ofGetWidth(), ofGetHeight());
    }
    
    if ( bSave ){
        screen.startSave( name + "_fg.png", bg);
    }
    
    if ( bInd && bTexLogo){
        toBind.bind();
    }
    if ( bDrawLogo ){
        logo.draw();
    }if ( bInd && bTexLogo){
        toBind.unbind();
    }
    
    if ( bSave ){
        screen.endSave();
        bSave = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bubbs.addParticle(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
