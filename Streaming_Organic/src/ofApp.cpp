#include "ofApp.h"

vector<ofColor> colors;
int which       = 0;
int whichLogo   = 0;
float minArea   = 0;
float maxArea   = 1920 * 1080;

int numFinder   = 3;
int numIterations = 1;

bool bDrawPhoto = true;
bool bRandomoze = false;
bool bRotate = false;
bool bOffset = false;
bool bDrawLogo = false;
bool bColorBg = false;

float rotateInc = 3.0;
float offset    = 10.0;

float contrast = 1.0;
float lastContrast = -1;

//--------------------------------------------------------------
void ofApp::setup(){
    float scale = ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale();
    if ( scale > 1.0 ){
        ofSetWindowShape(ofGetWidth() * 1.5, ofGetHeight() * 1.5);
    }
    
    ofBackground(255);
    int x = 0;
    int y = 0;
    for ( int i=0; i<numFinder; i++){
        ofxUICanvas * gui = new ofxUICanvas(x,y, ofGetWidth()/4, ofGetHeight() * .75);
        contourFinders.push_back( new ThreadedContourFinder());
        gui->addMinimalSlider ("threshold_"+ofToString(i), 0, 255, &contourFinders[i]->threshold);
        gui->addMinimalSlider("smoothing_"+ofToString(i), 0, 255, &contourFinders[i]->polylineSimplify);
        gui->addMinimalSlider("minArea", 0, ofGetWidth() * ofGetHeight(), &contourFinders[i]->minArea);
        gui->addMinimalSlider("maxArea", 0, ofGetWidth() * ofGetHeight(), &contourFinders[i]->maxArea);
        gui->addMinimalSlider("contrast", .1, 3.0, &contourFinders[i]->contrast);
        gui->addMinimalSlider("scale", 1.0, 3.0, &contourFinders[i]->scale);
        gui->addToggle("invert", &contourFinders[i]->bInvert);
        gui->addToggle("warp image", &contourFinders[i]->bDoWarp);
        gui->addToggle("snap to edge", &contourFinders[i]->bEdge);
        gui->addToggle("map image", &contourFinders[i]->bMapImage);
        gui->addToggle("tint", &contourFinders[i]->bTint);
        gui->addToggle("Filter: gaussian", &contourFinders[i]->filters[0]->bActive);
        gui->addMinimalSlider("Filter: gaussian (amount)", 1.0, 11.0, &contourFinders[i]->filters[0]->amount);
        gui->addToggle("Filter: laplacian", &contourFinders[i]->filters[1]->bActive);
        gui->addMinimalSlider("Filter: laplacian (amount)", 1.0, 11.0, &contourFinders[i]->filters[1]->amount);
        gui->addToggle("Filter: dilate", &contourFinders[i]->filters[2]->bActive);
        gui->addMinimalSlider("Filter: dilate (amount)", 1.0, 11.0, &contourFinders[i]->filters[2]->amount);
        
        gui->addIntSlider("windingMode", OF_POLY_WINDING_ODD, OF_POLY_WINDING_ABS_GEQ_TWO, &contourFinders[i]->windingMode);
        gui->addSpacer();
        
        
        contourFinders[i]->setup();
        guis.push_back(gui);
        
        x += ofGetWidth()/4;
        if ( x + ofGetWidth()/4 > ofGetWidth() ){
            x = 0;
            y += ofGetHeight()/4;
        }
    }
    
    colorMgr.setup();
    randomizeColors();
    
    ofxUICanvas * gui = new ofxUICanvas(x,y, ofGetWidth()/4, ofGetHeight()/2);
    gui->addSpacer();
    gui->addMinimalSlider("Contrast", 0.01, 3.0, &contrast);
    gui->addToggle("Draw phot", &bDrawPhoto);
    gui->addToggle("Randomize Colors", &bRandomoze);
    gui->addToggle("Rotate parts", &bRotate);
    gui->addMinimalSlider("Rotate increment", 0.01, 90.0, &rotateInc);
    gui->addToggle("Offset part", &bOffset);
    gui->addMinimalSlider("Offset increment", 0.01, 100.0, &offset);
    gui->addIntSlider("Num draw iterations", 1, 100, &numIterations);
    gui->addToggle("Draw Logo", &bDrawLogo);
    gui->addToggle("Color BG", &bColorBg);
    gui->addIntSlider("Which BG color", 0, 2, &which);
    gui->addIntSlider("Which Logo color", 0, 2, &whichLogo);
    
    guis.push_back(gui);
    
    
    int i=0;
    for ( auto * gui : guis ){
        gui->loadSettings("settings_"+ofToString(i)+".xml");
        i++;
    }
    
    drawImg.resize(3);
    imgs.resize(3);
    
    // LOGO
    logo.setup();
    screen.setup();
}

//--------------------------------------------------------------
void ofApp::randomizeColors(){
    colors.clear();
    colors = colorMgr.getHighLowTriplet();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(),3));
    if ( toLoad != "" ){
        imgs[whichToLoad].loadImage(toLoad);
        toLoad = "";
        lastContrast = -1;
        if ( whichToLoad == 0 ){
            for (int i=0; i<numFinder; i++){
                contourFinders[i]->loadPixels(imgs[whichToLoad]);
            }
        }
    }
    
    if ( bRandomoze ){
        randomizeColors();
        bRandomoze = false;
    }
    
    if ( screen.mode == 3){
        logo.scale = (ofGetWidth() * .7) / logo.width;
    } else if ( screen.mode == 4 ){
        logo.scale = (ofGetWidth() * .9) / logo.width;
    } else {
        logo.scale = (ofGetWidth() * .8) / logo.width;
    }
    
    int which = 0;
    for ( auto & img : imgs ){
        if ( img.isAllocated()){
            cv::Mat cv = ofxCv::toCv(img);
            cv::Mat dest;
            if ( contrast != lastContrast ){
                cv.convertTo(dest, -1,contrast,0);
                ofxCv::toOf(dest, drawImg[which]);
                drawImg[which].update();
            }
        }
        which++;
    }
    lastContrast = contrast;
}

//--------------------------------------------------------------
void ofApp::draw(){
    logo.setColor(colors[whichLogo]);
    if ( bColorBg ){
        ofBackground(colors[which]);
    } else {
        ofBackground(255);
    }
    
    ofSetColor(255);
    ofDisableDepthTest();
    if ( bDrawPhoto && drawImg[0].isAllocated()) drawImg[0].draw(0,0);
    ofEnableDepthTest();
    
    for ( int j=0; j<numIterations; j++){
        int i=0;
        for ( auto * f : contourFinders ){
            f->update();
            ofPushMatrix();
            ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0, -3 + 2 * i);
            if ( bRotate ) ofRotate(j*rotateInc);
            ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
            if (bOffset) ofTranslate(j*offset, j*offset);
            if ( f->bTint ) ofSetColor(colors[i]);
            else ofSetColor(255);
            if ( f->bMapImage ) drawImg[i].bind();
            f->getMesh().draw();
            if ( f->bMapImage ) drawImg[i].unbind();
            ofPopMatrix();
            
            i++;
        }
    }
    
    ofSetColor(255);
    if ( bDrawLogo ){
        logo.draw();
    }
    
    if ( guis[0]->isVisible() ){
        ofDisableDepthTest();
        int i=0;
        for ( auto * f : contourFinders ){
            ofSetColor(255);
            f->draw(0, i * 300, 300);
            i++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ){
        for ( auto * gui : guis ){
            gui->toggleVisible();
        }
    } else if ( key == 's' ){
        int i=0;
        for ( auto * gui : guis ){
            gui->saveSettings("settings_"+ofToString(i)+".xml");
            i++;
        }
    }
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
    whichToLoad = dragInfo.position.x < ofGetWidth()/3.0 ? 0 : (dragInfo.position.x < ofGetWidth() * 2/3.0 ? 1 : 2);
    for ( auto & s : dragInfo.files ){
        toLoad = s;
        break;
    }
}
