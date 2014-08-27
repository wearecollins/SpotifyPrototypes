#include "ofApp.h"
#include "ofAppGLFWWindow.h"

bool bSaving        = false;        // cue to start saving
bool bOpenAferSave  = false;        // placeholder
bool bSaveProcess   = false;        // save thread is running? false = OK to save again

ofImage drawImage;                  // image drawing to screen
float windowScale   = 1.0;          // variable for retina rendering

float contrast    = 1.0;            // current contrast
float lastContrast    = 1.0;        // last setting (i.e. "do we need to process contrast?"
ofColor lastColorFg, lastColorBg;   // last color settings

ofTrueTypeFont fontMedium;          // "Drag Here..." font
ofImage logo;                       // GUI logo

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowPosition(0, 0);
    
    // are we retina or not?
    windowScale = ((ofAppGLFWWindow*) ofGetWindowPtr())->getPixelScreenCoordScale();
    if ( windowScale != 1.0 ){
        ofSetWindowShape(1024 * windowScale,768 * windowScale );
    }
    ofShowCursor();
    
    // OS X bundles all assets into app. Windows isn't so lucky!
#ifdef TARGET_OSX
    ofSetDataPathRoot("./");
#endif
    
    ofSetVerticalSync(true);
    manager.setup();
    
    ofAddListener(manager.onLoaded, this, &ofApp::onNewImage);
    ofAddListener(manager.onLoadedFile, this, &ofApp::onFileLoaded);
    ofAddListener(saveButton.onPressed, this, &ofApp::saveImage);
    ofAddListener(saver.onComplete, this, &ofApp::onSavingComplete);
    ofBackground(100);
    
    // build GUI
    gui.setup("assets/fonts/CircularStd-Book.ttf", windowScale );
    gui.x = floor(ofGetWidth() - gui.getWidth()-20);
    gui.y = 20;
    gui.attachFloat("contrast", &contrast);
    
    saveButton.setup("assets/fonts/CircularStd-Book.ttf", "Save", windowScale);
    saveButton.x = ofGetWidth() - saveButton.width-20;
    saveButton.y = ofGetHeight() - saveButton.height-20;
    saveButton.enabled = false;
    
    fontMedium.loadFont("assets/fonts/CircularStd-Medium.ttf", 30 * windowScale);
    fontMedium.setLetterSpacing(.95);
    fontMedium.setSpaceSize(.6);
    logo.loadImage( windowScale == 1.0 ? "assets/logo.png" : "assets/logo_retina.png" );
}

//--------------------------------------------------------------
void ofApp::update(){
    // are there images?
    if ( manager.size() == 0 ){
        return;
    }
    
    // save/save as button enabled/disabled
    if ( drawImage.isAllocated() && !bSaveProcess){
        saveButton.enabled = true;
    }
    
    // do we need to process our image / show on screen?
    
    vector<ofColor> colors = gui.getActive();
    
    bool process = ((colors.size() == 2 && (lastColorFg != colors[0] || lastColorBg != colors[1])) || lastContrast != contrast);
    
    if ( process ){
        // scale to screen, run preview contrast
        float scale = fmin( (float) ofGetWidth()/manager.getRawImage(0).width, (float) ofGetHeight()/manager.getRawImage(0).height );
        manager.getImage(0).resize(manager.getRawImage(0).width * scale, manager.getRawImage(0).height * scale);
        drawImage.clone(manager.getImage(0));
        static cv::Mat mat;
        static cv::Mat p;
        mat = ofxCv::toCv(drawImage);
        mat.convertTo(p, -1,contrast,0);
        ofxCv::toOf(p, drawImage);
        drawImage.update();
        lastContrast = contrast;
    }
    
    if ( colors.size() < 2 ) return;
    
    // process live colors
    if ( process ){
        filter.setColorPair(0, colors[1], colors[0]);
        filter.process(drawImage);
        lastColorFg.set(colors[0]);
        lastColorBg.set(colors[1]);
    }
    
    // save / save ass
    if ( bSaving ){
        saveButton.enabled = false;
        bSaveProcess = true;
        saver.saveToolTip = false;
        
        ofFileDialogResult folder = ofSystemSaveDialog("image_"+ofGetTimestampString()+"_"+ofToString(floor(ofRandom(100))) + ".png",
                                                       "Set name and directory of file. Save as .png or .jpg.");
        if (folder.bSuccess ){
            string filePath = folder.getPath();
            saver.save(manager.getRawImage(0), filter, contrast, filePath);
        }
        bSaving = false;
    }
    
    //setApplicationIconImage <- for another day
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofShowCursor();
    
    // draw preview image?
    if ( drawImage.isAllocated() ){
        float scale = MAX( (float) ofGetWidth()/ drawImage.width, (float) ofGetHeight() / drawImage.height);
        drawImage.draw(ofGetWidth()/2.0 - drawImage.width * scale / 2.0, ofGetHeight()/2.0 - drawImage.height * scale/2.0, drawImage.width * scale, drawImage.height * scale);
    
    // currently loading image
    } else if ( manager.size() > 0 ){
        string str = "Loading";
        int count = fabs(sin(ofGetElapsedTimeMillis() * .01 )) * 3;
        for ( int i=0; i<count; i++){
            str += ".";
        }
        float w = fontMedium.stringWidth("Loading...");
        float h = fontMedium.stringHeight("Loading...");
        fontMedium.drawString(str, ofGetWidth()/2.0 - w/2.0, ofGetHeight()/2.0 - h/2.0 + fontMedium.getSize());
    
    // no image, hook me up!
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
void ofApp::keyPressed( int k ){
//    else if (k =='s') bSaving = true;
    if ( k == 's' && ofGetKeyPressed(OF_KEY_COMMAND)){
        bSaving = true;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h ){
    gui.x = fmax(20, floor(ofGetWidth() - gui.getWidth()-20));
    gui.y = 20;
    
    saveButton.x = fmax(20, ofGetWidth() - saveButton.width-20);
    saveButton.y = fmax(gui.y + gui.getHeight() + saveButton.height + 40, ofGetHeight() - saveButton.height-20);
}

//--------------------------------------------------------------
void ofApp::onNewImage( ofImage & img ){
    lastContrast = -1;
    lastColorFg.set(255);
    lastColorBg.set(255);
}

//--------------------------------------------------------------
void ofApp::onFileLoaded( string & img ){
    gui.setTitle(img);
}

//--------------------------------------------------------------
void ofApp::onSavingComplete(){
    bSaveProcess = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}