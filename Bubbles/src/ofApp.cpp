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

float density        = 1.0;
float lastdensity    = 1.0;
float noiseFactor    = 1.0;
float complexityF    = 1.0;

float contrast    = 1.0;
float lastContrast = 1.0;

using namespace rc;
using namespace lab;

float zRange        = -5;

FilterContrast  contrastFilter;

ofImage             toBind;
cv::Mat process;

float width = 0;
float height = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    logo.setup();
    
    imageManager.setup();
    colorManager.setup();
    
    // colors
    vector<ofColor> hl = colorManager.getHighLowPair();
    colorA = hl[0];//rc::ofRandomRCColor();
    colorB = hl[1];//rc::ofRandomRCColor();
    ofBackground(255);
    colors.addColorPair(colorA, colorB);
    //imageManager.addFilter(&colors);
    logo.setColor( colorB );
    
    particles.setupSquare(ofVec2f(ofGetWidth(), ofGetHeight()));
    particles.setOption(ParticleSystem::HORIZONTAL_WRAP, false);
    particles.setOption(ParticleSystem::VERTICAL_WRAP, false);
    particles.setOption(ParticleSystem::DETECT_COLLISIONS, true);
//    particles.addForce(const ofVec3f &force);
    
    gui = new ofxUICanvas(0,0, ofGetWidth()/2.0, ofGetHeight()/2.0);
    gui->addToggle("Animation", &bAnimate);
    gui->addToggle("Draw Logo", &bDrawLogo);
    gui->addToggle("Clear", &bClear);
    gui->addToggle("Color background", &bColorBg);
    gui->addToggle("Color bg with fg", &bBgFg);
    
    gui->addSlider("zRange", -100, 0, &zRange);
    gui->addSlider("density", 0.01, 1.00, &density);
    gui->addSlider("Noise factor", 0.01, 1.00, &noiseFactor);
    gui->addSlider("Shape complexity", 0.01, 1.00, &complexityF);
    
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

//--------------------------------------------------------------
void ofApp::update(){
    if ( bColorBg ) ofBackground(bBgFg ? colorB : colorA);
    else ofBackground(255);
    
    if ( bRandomizeColor ){
        lastContrast = -1;
        bRandomizeColor = false;
//        colorA = rc::ofRandomRCColor();
//        colorB = rc::ofRandomRCColor();
//
        
        vector<ofColor> hl = colorManager.getHighLowPair();
        colorA = hl[1];//rc::ofRandomRCColor();
        colorB = hl[0];//rc::ofRandomRCColor();
        colors.setColorPair(0,colorA, colorB);
        
        logo.setColors( colorA, colorB );
        //imageManager.runFilters();
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
        particles.clear();
    }
    ParticleSystem::const_Iterator it = particles.getParticles()->begin();
    ParticleSystem::const_Iterator it2 = particles.getParticles()->begin();
    ofVec3f center(ofGetWidth()/2.0, ofGetHeight()/2.0, fabs(sin( ofGetElapsedTimeMillis() * .0001 ))* zRange);
    
    if ( lastdensity != density ){
        lastdensity = density;
        int count = particles.getNumParticles() * density;
        int i = 0;
        for( it; it != particles.getParticles()->end(); ++it )
        {
            bool bL = ((SParticle*) it->second)->bLocked;
            if ( i < count && !bL ){
                ((SParticle*) it->second)->bTransparent = false;
            } else {
                ((SParticle*) it->second)->bTransparent = true;
            }
            if ( !bL) i++;
        }
    }
    
    for( it; it != particles.getParticles()->end(); ++it )
    {
        ((SParticle*) it->second)->noiseFactor = noiseFactor;
        ((SParticle*) it->second)->complexity = complexityF;
        
        it->second->attract(center);
        for( it2 = particles.getParticles()->begin(); it2 != particles.getParticles()->end(); ++it2 )
        {
            it->second->repel(*it2->second);
        }
    }
    if ( bAnimate) particles.update();
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
    }
    
    ofEnableDepthTest();
    float dir = 1;
    int i = 0;
    if ( bInd){
        toBind.bind();
    }
    particles.draw();
    if ( bInd ){
        toBind.unbind();
    }
    
    if ( bSave ){
        screen.endSave();
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
    SParticle * p = new SParticle(x,y);
    p->z = ofRandom(10,2);
    int ran = ofRandom(100);
    p->radius = ran > 90 ? ofRandom(10,300) : ofRandom(3,50);
    p->mass = ran < 90 ? p->radius : p->radius * .25;
    p->damping = .9;
    particles.addParticle(p);
    if ( ofGetKeyPressed( OF_KEY_SHIFT) ){
        p->bLocked = true;
        p->bTransparent = true;
        p->radius = 100;
    } else {
        p->res = ofRandom(3,50);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
