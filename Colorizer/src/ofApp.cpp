#include "ofApp.h"
#include "ofAppGLFWWindow.h"

bool bSaving      = false;
bool bSaveAll     = false;
bool bDestructive = false;
bool bOpenAferSave = false;
ofImage temp;
float windowScale   = 1.0;

float contrast    = 1.0;
float lastContrast    = 1.0;
ofColor c, d;

ofTrueTypeFont fontMedium;
ofImage logo;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowPosition(0, 0);
    windowScale = ((ofAppGLFWWindow*) ofGetWindowPtr())->getPixelScreenCoordScale();
    if ( windowScale != 1.0 ){
        ofSetWindowShape(1024 * windowScale,768 * windowScale );
    }
    ofShowCursor();
    ofSetDataPathRoot("./");
    ofSetVerticalSync(true);
    manager.setup();
    ofAddListener(manager.onLoaded, this, &ofApp::onNewImage);
    ofAddListener(manager.onLoadedFile, this, &ofApp::onFileLoaded);
    ofAddListener(saveButton.onPressed, this, &ofApp::saveImage);
    ofAddListener(saveAllButton.onPressed, this, &ofApp::saveAll);
    ofBackground(100);
    
    gui.setup("assets/fonts/CircularStd-Book.ttf", windowScale );
    gui.x = floor(ofGetWidth() - gui.getWidth()-20);
    gui.y = 20;
    gui.attachFloat("contrast", &contrast);
    
    saveButton.setup("assets/fonts/CircularStd-Book.ttf", "Save", windowScale);
    saveButton.x = ofGetWidth() - saveButton.width-20;
    saveButton.y = ofGetHeight() - saveButton.height-20;
//    
//    saveAllButton.setup("assets/fonts/CircularStd-Book.ttf", "Save all");
//    saveAllButton.x = ofGetWidth() - saveButton.width-20;
//    saveAllButton.y = ofGetHeight() - saveButton.height - saveButton.height-40;
    
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
    vector<ofColor> colors = gui.getActive();
    
    bool process = ((colors.size() == 2 && (c != colors[0] || d != colors[1])) || lastContrast != contrast);
    
    if ( process ){
        float scale = fmin( (float) ofGetWidth()/manager.getRawImage(0).width, (float) ofGetHeight()/manager.getRawImage(0).height );
        manager.getImage(0).resize(manager.getRawImage(0).width * scale, manager.getRawImage(0).height * scale);
        temp.clone(manager.getImage(0));
        cout << temp.width * scale << ":" << temp.height * scale << endl;
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
        saver.saveToolTip = false;
        saver.save(manager.getRawImage(0), filter, contrast);
        bSaving = false;
    }
    if ( bSaveAll ){
        ColorManager cb = gui.getColorBlack();
        ColorManager cw = gui.getColorWhite();
        saver.saveToolTip = true;
        map<string, bool> stuff;
        for ( auto & set : cb.colors){
            for ( auto & colorB : set ){
                for ( auto & setW : cw.colors){
                    for ( auto & colorW : setW ){
                        string c = ofToString(colorB.r) + ":" + ofToString(colorB.g) + ":"+ofToString(colorB.b);
                        c += ":"+ofToString(colorW.r) + ":" + ofToString(colorW.g) + ":"+ofToString(colorW.b);
                        
                        if ( stuff.count(c) == 0 ){
                            stuff[c] = true;
                            filter.setColorPair(0, colorB, colorW);
                            saver.save(manager.getRawImage(0), filter, contrast, false);
                        }
                    }
                }
            }
        }
        saver.saveToolTip = false;
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofShowCursor();
    
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
    logo.draw(20,27);
}

//--------------------------------------------------------------
void ofApp::saveImage( bool & b ){
    bSaving = true;
}

//--------------------------------------------------------------
void ofApp::saveAll( bool & b ){
    bSaveAll = true;
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
    
    gui.x = fmax(20, floor(ofGetWidth() - gui.getWidth()-20));
    gui.y = 20;
    
    saveButton.x = fmax(20, ofGetWidth() - saveButton.width-20);
    saveButton.y = fmax(gui.y + gui.getHeight() + 20, ofGetHeight() - saveButton.height-20);
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
