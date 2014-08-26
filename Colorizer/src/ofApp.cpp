#include "ofApp.h"
#include "ofAppGLFWWindow.h"

bool bSaving        = false;    // cue to start saving
bool bSaveAs        = false;    // cue to "save as"
bool bOpenAferSave  = false;    // placeholder
bool bCurrentSaved  = false;    // current color config has been saved?
bool bSaveProcess   = false;    // save thread is running? false = OK to save again

ofImage temp;
float windowScale   = 1.0;

float contrast    = 1.0;
float lastContrast    = 1.0;
ofColor c, d;

ofTrueTypeFont fontMedium;
ofImage logo;

string filePath = "../../../";

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowPosition(0, 0);
    windowScale = ((ofAppGLFWWindow*) ofGetWindowPtr())->getPixelScreenCoordScale();
    if ( windowScale != 1.0 ){
        ofSetWindowShape(1024 * windowScale,768 * windowScale );
    }
    ofShowCursor();
#ifdef TARGET_OSX
    ofSetDataPathRoot("./");
#endif
    
    ofSetVerticalSync(true);
    manager.setup();
    ofAddListener(manager.onLoaded, this, &ofApp::onNewImage);
    ofAddListener(manager.onLoadedFile, this, &ofApp::onFileLoaded);
    ofAddListener(saveButton.onPressed, this, &ofApp::saveImage);
    ofAddListener(saveAsButton.onPressed, this, &ofApp::saveAs);
    ofAddListener(saver.onComplete, this, &ofApp::onSavingComplete);
    ofBackground(100);
    
    gui.setup("assets/fonts/CircularStd-Book.ttf", windowScale );
    gui.x = floor(ofGetWidth() - gui.getWidth()-20);
    gui.y = 20;
    gui.attachFloat("contrast", &contrast);
    
    saveButton.setup("assets/fonts/CircularStd-Book.ttf", "Save", windowScale);
    saveButton.x = ofGetWidth() - saveButton.width-20;
    saveButton.y = ofGetHeight() - saveButton.height-20;
    saveButton.enabled = false;
    
//    
    saveAsButton.setup("assets/fonts/CircularStd-Book.ttf", "Save As", windowScale);
    saveAsButton.x = ofGetWidth() - saveButton.width-20;
    saveAsButton.y = ofGetHeight() - saveButton.height - saveButton.height-40;
    saveAsButton.enabled = false;
    
    fontMedium.loadFont("assets/fonts/CircularStd-Medium.ttf", 30 * windowScale);
    fontMedium.setLetterSpacing(.95);
    fontMedium.setSpaceSize(.6);
    logo.loadImage( windowScale == 1.0 ? "assets/logo.png" : "assets/logo_retina.png" );
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( manager.size() == 0 ){
        return;
    }
    
    if ( temp.isAllocated() && !bSaveProcess){
        saveAsButton.enabled = true;
        bool colorsNew = gui.isFrameNew();
        if ( bCurrentSaved ) bCurrentSaved = !colorsNew;
        
        if ( !colorsNew && bCurrentSaved ){
            saveButton.enabled = false;
        } else {
            saveButton.enabled = true;
        }
    }
    
    vector<ofColor> colors = gui.getActive();
    
    bool process = ((colors.size() == 2 && (c != colors[0] || d != colors[1])) || lastContrast != contrast);
    
    if ( process ){
        float scale = fmin( (float) ofGetWidth()/manager.getRawImage(0).width, (float) ofGetHeight()/manager.getRawImage(0).height );
        manager.getImage(0).resize(manager.getRawImage(0).width * scale, manager.getRawImage(0).height * scale);
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
        saveAsButton.enabled = saveButton.enabled = false;
        bCurrentSaved = true;
        bSaveProcess = true;
        saver.saveToolTip = false;
        saver.save(manager.getRawImage(0), filter, contrast, filePath);
        bSaving = false;
    }
    if ( bSaveAs ){
        saveAsButton.enabled = saveButton.enabled = false;
        saver.saveToolTip = false;
        bSaveProcess = true;
        ofFileDialogResult folder = ofSystemLoadDialog("Choose destination", true);
        if (folder.bSuccess ){
            filePath = folder.getPath();
            saver.save(manager.getRawImage(0), filter, contrast, filePath);
        }
        bSaveAs = false;
    }
    
    //setApplicationIconImage
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofShowCursor();
    
    if ( temp.isAllocated() ){
        float scale = MAX( (float) ofGetWidth()/ temp.width, (float) ofGetHeight() / temp.height);
        temp.draw(ofGetWidth()/2.0 - temp.width * scale / 2.0, ofGetHeight()/2.0 - temp.height * scale/2.0, temp.width * scale, temp.height * scale);
    } else if ( manager.size() > 0 ){
        //manager.getImage(0).draw(ofGetWidth()/2.0 - manager.getImage(0).width/2.0, ofGetHeight()/2.0 - manager.getImage(0).height/2.0);
        string str = "Loading";
        int count = fabs(sin(ofGetElapsedTimeMillis() * .01 )) * 3;
        for ( int i=0; i<count; i++){
            str += ".";
        }
        float w = fontMedium.stringWidth("Loading...");
        float h = fontMedium.stringHeight("Loading...");
        fontMedium.drawString(str, ofGetWidth()/2.0 - w/2.0, ofGetHeight()/2.0 - h/2.0 + fontMedium.getSize());
    } else {
        float w = fontMedium.stringWidth("Drag image here");
        float h = fontMedium.stringHeight("Drag image here");
        fontMedium.drawString("Drag image here", ofGetWidth()/2.0 - w/2.0, ofGetHeight()/2.0 - h/2.0 + fontMedium.getSize());
    }
    ofSetColor(255);
    logo.draw(20,27);
}

//--------------------------------------------------------------
void ofApp::saveImage( bool & b ){
    bSaving = true;
}

//--------------------------------------------------------------
void ofApp::saveAs( bool & b ){
    bSaveAs = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed( int k ){
//    else if (k =='s') bSaving = true;
    if ( k == 's' && ofGetKeyPressed(OF_KEY_COMMAND)){
        if ( ofGetKeyPressed(OF_KEY_SHIFT)){
            bSaveAs = true;
        } else {
            bSaving = true;
        }
    }
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
    gui.x = fmax(20, floor(ofGetWidth() - gui.getWidth()-20));
    gui.y = 20;
    
    saveButton.x = fmax(20, ofGetWidth() - saveButton.width-20);
    saveButton.y = fmax(gui.y + gui.getHeight() + saveButton.height + 40, ofGetHeight() - saveButton.height-20);
    saveAsButton.x = fmax(20, ofGetWidth() - saveButton.width-20);
    saveAsButton.y = fmax(gui.y + gui.getHeight() + 20, ofGetHeight() - saveButton.height - saveButton.height-40);
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

//--------------------------------------------------------------
void ofApp::onSavingComplete(){
    bSaveProcess = false;
}