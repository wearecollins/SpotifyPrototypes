#include "ofApp.h"

int which       = 0;
int whichLogo   = 0;
int whichLogoCircle = 0;
float minArea   = 0;
float maxArea   = 1920 * 1080;

int numFinder   = 1;
int numIterations = 1;

bool bSave      = false;
bool bDrawPhoto = false;
bool bRandomoze = false;
bool bRotate = false;
bool bOffset = false;
bool bDrawLogo = false;
bool bUseGrad = true;
bool bColorBg = false;
bool bSlow = false;
bool bFast = false;

float rotateInc = 3.0;
float offset    = 10.0;

float contrast = 1.0;
float lastContrast = -1;

float separation = 500;
float separationBG = 500;
ofVec2f texCoordShift;

ofFbo imageFBO;

//--------------------------------------------------------------
void ofApp::setup(){
    float scale = ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale();
    if ( scale > 1.0 ){
        ofSetWindowShape(ofGetWidth() * 1.5, ofGetHeight() * 1.5);
    }
    
    gui.setup("fonts/CircularStd-Book.ttf");
    gui.attachFloat("contrast", &contrast);
    
    colors.resize(2);
    
    // setup gui
    ofBackground(255);
    int x = 0;
    int y = 0;
    for ( int i=0; i<numFinder; i++){
        ofxUICanvas * gui = new ofxUICanvas(x,y, ofGetWidth()/4, ofGetHeight() * .75);
        contourFinders.push_back( new ThreadedContourFinder());
        gui->addWidgetDown(new ofxUILabel("CONTOUR FINDER", OFX_UI_FONT_LARGE));
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
        gui->toggleVisible();
        
        contourFinders[i]->setup();
        guisAdvanced.push_back(gui);
        
        x += ofGetWidth()/4;
        if ( x + ofGetWidth()/4 > ofGetWidth() ){
            x = 0;
            y += ofGetHeight()/4;
        }
    }

    logo.setup("assets/logo.svg");
    screen.setup();
    colorMgr.setup("colors.xml");
    
    ofxUICanvas * guiAdvanced = new ofxUICanvas(x,y, ofGetWidth()/4, ofGetHeight() * .9);
    guiAdvanced->addWidgetDown(new ofxUILabel("ADVANCED SETTINGS", OFX_UI_FONT_LARGE));
    guiAdvanced->addMinimalSlider("Contrast", 0.01, 3.0, &contrast);
    guiAdvanced->addToggle("Draw photo", &bDrawPhoto);
    guiAdvanced->addToggle("Rotate parts", &bRotate);
    guiAdvanced->addMinimalSlider("Rotate increment", 0.01, 90.0, &rotateInc);
    guiAdvanced->addToggle("Offset part", &bOffset);
    guiAdvanced->addMinimalSlider("Offset increment", 0.01, 100.0, &offset);
    guiAdvanced->addIntSlider("Num draw iterations", 1, 100, &numIterations);
    guiAdvanced->addSpacer();
    guiAdvanced->addLabel("BUBBLES");
    guiAdvanced->addSlider("zRange", -100, 0, &bubbles.zRange);
    guiAdvanced->addSlider("density", 0.01, 1.00, &bubbles.density);
    guiAdvanced->addSlider("Noise factor", 0.01, 1.00, &bubbles.noiseFactor);
    guiAdvanced->addSlider("Shape complexity", 0.01, 1.00, &bubbles.complexity);
    guiAdvanced->addSlider("texCoordShift.x", -100.0, 100.0, &bubbles.texCoordShift.x);
    guiAdvanced->addSlider("texCoordShift.y", -100.0, 100.0, &bubbles.texCoordShift.y);
    
    guiAdvanced->addSpacer();
    guiAdvanced->addLabel("METABALLS");
    guiAdvanced->addSlider("separation", 0.0, 500, &separation);
    guiAdvanced->addSlider("force limit", 0.0, 100.0, &metaballs.forceLimit);
    guiAdvanced->addSlider("scale", 0.0, 100, &metaballs.iso);
    guiAdvanced->addSlider("separationBG", 0.0, 500, &separationBG);
    guiAdvanced->addSlider("force limit BG", 0.0, 100.0, &metaballsBG.forceLimit);
    guiAdvanced->addSlider("scale bg", 0.0, 100, &metaballsBG.iso);
    guiAdvanced->addSlider("texCoordShift.x", -100.0, 100.0, &texCoordShift.x);
    guiAdvanced->addSlider("texCoordShift.y", -100.0, 100.0, &texCoordShift.y);
    
    guiAdvanced->toggleVisible();
    guisAdvanced.push_back(guiAdvanced);
    
    x += ofGetWidth()/4;
    ofxUICanvas * guiAdvanced2 = new ofxUICanvas(x,y, ofGetWidth()/4, ofGetHeight() * .75);
    guiAdvanced2->addSpacer();
    guiAdvanced2->addLabel("LOGO");
    guiAdvanced2->addSlider("top.z", -10.0, 10.0, &logo.zpos[1]);
    guiAdvanced2->addSlider("middle.z", -10.0, 10.0, &logo.zpos[2]);
    guiAdvanced2->addSlider("bottom.z", -10.0, 10.0, &logo.zpos[0]);
    
    guiAdvanced2->toggleVisible();
    
    guiAdvanced2->addToggle("Draw Logo", &bDrawLogo);
    guiAdvanced2->addToggle("Draw Logo Container", &logo.drawCircle);
    guiAdvanced2->addToggle("Logo Gradient", &bUseGrad);
    
    guiAdvanced2->addToggle("Color Background", &bColorBg);
    guiAdvanced2->addIntSlider("Which BG color", 0, 1, &which);
    guiAdvanced2->addIntSlider("Which Logo color", 0, 1, &whichLogo);
    guiAdvanced2->addIntSlider("Which Logo container color", 0, 1, &whichLogoCircle);
    guiAdvanced2->addIntSlider("Draw Mode", 0, MODE_PHOTO, &mode);
    guiAdvanced2->addIntSlider("Background Mode", 0, MODE_PHOTO, &modeBG);
    guiAdvanced->addSpacer();
    guisAdvanced.push_back(guiAdvanced2);
    
    int i=0;
    for ( auto * gui : guis ){
        gui->loadSettings("settings_"+ofToString(i)+".xml");
        i++;
    }
    
    for ( auto * gui : guisAdvanced ){
        gui->loadSettings("settings_"+ofToString(i)+".xml");
        i++;
    }
    
    // butt ons
    saveButton.setup("fonts/CircularStd-Book.ttf");
    saveButton.x = ofGetWidth() - saveButton.width - 20;
    saveButton.y = ofGetHeight() - saveButton.height * 2 - 30;
    
    randomButton.setup("fonts/CircularStd-Book.ttf", "Randomize");
    randomButton.x = ofGetWidth() - saveButton.width - 20;
    randomButton.y = ofGetHeight() - saveButton.height - 20;
    
    gui.x = floor(ofGetWidth() - gui.getWidth()-20);
    gui.y = 20;
    
    ofAddListener(saveButton.onPressed, this, &ofApp::onSave);
    ofAddListener(randomButton.onPressed, this, &ofApp::onRandom);
    
    // images
    drawImg.resize(3);
    images.setup();
    ofAddListener(images.onLoaded, this, &ofApp::onImageLoaded);
    ofAddListener(images.onLoadedFile, this, &ofApp::onFileLoaded);
    
    // BUBBLES
    bubbles.setup();
    
    // METABALLS
    render.allocate(ofGetWidth(), ofGetHeight());
    imageFBO.allocate(ofGetWidth(), ofGetHeight());
    metaballProcessor.setMinAreaRadius(2);
    metaballs.setup();
    metaballs.color.set(1.0);
    metaballs.bInteractive = true;
    
    metaballsBG.setup();
    metaballsBG.color.set(1.0);
    
    // keys
    keys.add('p', &bDrawPhoto);
    keys.add('S', &bSlow);
    keys.add('F', &bFast);
    
    rc::KeyManager::Delegate d = tr1::bind(&ofApp::toggleAdvVisible, this );
    keys.addTrigger('g', d );
    
//    rc::KeyManager::Delegate D = tr1::bind(&ofApp::toggleAdvVisible, this );
//    keys.addTrigger('G', D );
    
    rc::KeyManager::Delegate s = tr1::bind(&ofApp::saveGui, this );
    keys.addTrigger('s', s );
    
    randomizeColors();
}

//--------------------------------------------------------------
void ofApp::randomizeColors(){
    gui.randomize();
    colorsProcessor.setColorPair(0, gui.getActive()[1], gui.getActive()[0]);
    bubbles.burst();
    lastContrast = -1;
    
    for (int i=0; i<numFinder; i++){
        contourFinders[i]->threshold *= ofRandom(.9,1.1);
    }
    
    metaballs.setup( ofGetHeight() * .7 );
    metaballs.color.set(1.0);
    metaballs.update();
    
    metaballsBG.setup( ofGetHeight() * .8 );
    metaballsBG.color.set(1.0);
    metaballsBG.update();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(),3));
    
    if ( bRandomoze ){
        randomizeColors();
        bRandomoze = false;
    }
    
    if ( bSlow && !bFast ){
        separation *= .95;
        separationBG *= .95;
        if ( separation < 5.0 && separationBG < 5.0 ) bSlow = false;
    } else if ( bFast ){
        bSlow = false;
        separation = separation * .95 + 300 * .05;
        separationBG = separationBG * .95 + 200 * .05;
        if ( 300 - separation < 1.0 && 200 - separationBG < 1.0 ) bFast = false;
    }
    
    if ( screen.mode == 3){
        logo.scale = (ofGetWidth() * .7) / logo.width;
    } else if ( screen.mode == 4 ){
        logo.scale = (ofGetWidth() * 4.5) / logo.width;
    } else {
        logo.scale = (ofGetWidth() * .8) / logo.width;
    }
    
    colorsProcessor.setColorPair(0, gui.getActive()[1], gui.getActive()[0]);
    
    if ( colors[0] != gui.getActive()[1] ||
        colors[1] != gui.getActive()[0]){
        colors[0].set(gui.getActive()[1]);
        colors[1].set(gui.getActive()[0]);
        lastContrast = -1;
    }
    
    int which = 0;
    if ( images.size() > 0 ){
        cv::Mat cv = ofxCv::toCv(images.getRawImage(0));
        cv::Mat dest;
        if ( contrast != lastContrast ){
            cv.convertTo(dest, -1,contrast,0);
            ofxCv::toOf(dest, drawImg[which]);
            drawImg[which].update();
            setupImage(drawImg[which]);
            for (int i=0; i<numFinder; i++){
                contourFinders[i]->loadPixels(drawImg[which]);
            }
            colorsProcessor.process(drawImg[which]);
        }
    }
    lastContrast = contrast;
    
    if ( mode == MODE_META ){
        metaballs.separation = separation;
        metaballs.update();
    }
    if ( modeBG == MODE_META ){
        metaballsBG.separation = separationBG;
        metaballsBG.update();
    }
}

//--------------------------------------------------------------
void ofApp::setupImage( ofImage & img ){

    float w, h;
    float rw = (float) ofGetWidth()/img.width;
    float rh = (float) ofGetHeight()/img.height;
//    if ( rw > rh ){
//        w = ofGetWidth();
//        h = img.height * w/img.width;
//    } else {
        h = ofGetHeight();
        w = img.width * h/img.height;
//    }
    img.resize(w, h);
    
    imageFBO.begin();
    ofClear(0,0);
    img.draw(ofGetWidth()/2.0-w/2.0, ofGetHeight()/2.0 - h/2.0);
    imageFBO.end();
    static ofPixels fboReadPixels;
    imageFBO.readToPixels(fboReadPixels);
    img.setFromPixels(fboReadPixels);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // LOGO + BG Setup
    if ( bUseGrad ){
        int a = whichLogo;
        int b = whichLogo+2;
        if ( b > 2 ) b = 0;
        logo.setColors(gui.getActive()[a], gui.getActive()[b]);
    } else {
        logo.setColor(gui.getActive()[whichLogo]);
    }
    logo.circleColor = gui.getActive()[whichLogoCircle];
    
    ofColor bg;
    if ( bColorBg ) bg.set(gui.getActive()[which]);
    else  bg.set(255);
    
    string name;
    if ( bSave ){
        string directory = ofGetTimestampString();
        ofDirectory dir(directory);
        dir.create();
        name = directory + "/particles_" + ofGetTimestampString();
        screen.startSave( name + "_bg.png", bg);
        bubbles.resize(ofGetWidth() * screen.saveScale, ofGetHeight() * screen.saveScale);
    }
    
    if ( bColorBg ){
        ofBackground(bg);
    } else {
        ofBackground(bg);
    }
    
    if ( bSave ){
        screen.endSave();
        bubbles.resize(ofGetWidth(), ofGetHeight());
    }
    
    if ( bSave ){
        screen.startSave( name + "_layer_1.png", bg);
        bubbles.resize(ofGetWidth() * screen.saveScale, ofGetHeight() * screen.saveScale);
    }
    drawLayerOne();
    
    if ( bSave ){
        screen.endSave();
        bubbles.resize(ofGetWidth(), ofGetHeight());
    }
    
    //
    if ( bSave ){
        screen.startSave( name + "_layer_2.png", bg);
        bubbles.resize(ofGetWidth() * screen.saveScale, ofGetHeight() * screen.saveScale);
    }
    
    drawLayerTwo();
    
    if ( bSave ){
        screen.endSave();
        bubbles.resize(ofGetWidth(), ofGetHeight());
    }
    
    //
    if ( bSave ){
        screen.startSave( name + "_layer_3.png", bg);
        bubbles.resize(ofGetWidth() * screen.saveScale, ofGetHeight() * screen.saveScale);
    }
    
    drawLayerThree();
    
    if ( bSave ){
        screen.endSave();
        bubbles.resize(ofGetWidth(), ofGetHeight());
    }
    
    //
    if ( bSave ){
        screen.startSave( name + "_comp.png", bg);
        bubbles.resize(ofGetWidth() * screen.saveScale, ofGetHeight() * screen.saveScale);
        render.allocate(ofGetWidth() * screen.saveScale, ofGetHeight() * screen.saveScale);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        ofClear(bg);
        if ( bColorBg ){
            ofBackground(bg);
        } else {
            ofBackground(bg);
        }
        drawLayerOne( true );
        drawLayerTwo();
        drawLayerThree( true );
//        glDisable(GL_BLEND);
        glPopAttrib();
        screen.endSave();
        bubbles.resize(ofGetWidth(), ofGetHeight());
        render.allocate(ofGetWidth(), ofGetHeight());
    }
    
    bSave = false;
    
    if ( guisAdvanced[0]->isVisible() ){
        ofDisableDepthTest();
        int i=0;
        for ( auto * f : contourFinders ){
            ofSetColor(255);
            f->draw(ofGetWidth()/2.0, i * 300, 300);
            i++;
        }
    }
    
    ofSetColor(255);
    ofDisableDepthTest();
}


//--------------------------------------------------------------
void ofApp::onSave( bool & b ){
    bSave = true;
}

//--------------------------------------------------------------
void ofApp::onRandom( bool & b ){
    bRandomoze = true;
}

//--------------------------------------------------------------
void ofApp::drawLayerOne( bool bRender ){
    ofSetColor(255);
    ofDisableDepthTest();
    
    if ( bDrawPhoto && drawImg[0].isAllocated()) drawImg[0].draw(0,0);
    
    switch (modeBG) {
        case MODE_BUBBLES:
        case MODE_META:
            metaballsBG.center.set(ofGetWidth() * .5, ofGetHeight() * .5);
            drawMesh.clear();
            if ( bRender ){
                render.begin();
                ofClear(0,0);
                metaballsBG.draw();
                render.end();
                render.readToPixels(pix);
                metaballProcessor.findContours(pix);
            }
            
            static ofPath p;
            ofSetColor(255);
            //ofEnableBlendMode(OF_BLENDMODE_ADD);
            for ( int i=0; i<metaballProcessor.size(); i++){
                p.clear();
//                int cnt = ofMap(mouseY, 0, ofGetHeight(), 3, metaballProcessor.getPolyline(i).size() / 4.0);
//                ofPolyline pl = metaballProcessor.getPolyline(i).getResampledByCount( cnt );
                tess.tessellateToMesh(metaballProcessor.getPolyline(i), OF_POLY_WINDING_ODD, drawMesh);
                ofColor c = gui.getActive()[0];
                for ( auto & vec : drawMesh.getVertices() ){
                    drawMesh.addTexCoord(vec + texCoordShift);
                    drawMesh.addColor( ofFloatColor( c.r / 255.0, c.g/ 255.0, c.b/ 255.0) );
                    vec.z = -5 + 0;
                }
                drawMesh.draw();
            }
            break;
            
        case MODE_PHOTO:
            for ( int j=0; j<numIterations; j++){
                int i=0;
                for ( auto * f : contourFinders ){
                    f->update();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0, -3 + 2 * i);
                    if ( bRotate ) ofRotate(j*rotateInc);
                    ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
                    if (bOffset) ofTranslate(j*offset, j*offset);
                    if ( f->bTint ) ofSetColor(gui.getActive()[i]);
                    else ofSetColor(255);
                    if ( f->bMapImage ) drawImg[i].bind();
                    f->getMesh().draw();
                    if ( f->bMapImage ) drawImg[i].unbind();
                    ofPopMatrix();
                    
                    i++;
                }
            }
            break;
    }
    
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::drawLayerTwo(){
    ofSetColor(255);
    if ( bDrawLogo ){
        logo.draw();
    }
}

//--------------------------------------------------------------
void ofApp::drawLayerThree( bool bRender ){
    bool bInd = drawImg.size() > 0 && drawImg[0].isAllocated();
    
    if ( bInd && mode == MODE_BUBBLES){
        drawImg[0].bind();
    }
    
    if ( mode == MODE_BUBBLES ){
        bubbles.draw();
    } else if ( mode == MODE_META ){
//        if ( screen.mode == 3){
//            metaballs.center.set(ofGetWidth() * .25, ofGetHeight()/2.0);
//        } else if ( screen.mode == 4 ){
//            metaballs.center.set(ofGetWidth() * .5, ofGetHeight() * .75);
//        } else {
//            metaballs.center.set(ofGetWidth() * .5, ofGetHeight() * .5);
//        }
        metaballs.center.set(ofGetWidth() * .5, ofGetHeight() * .5);
        drawMesh.clear();
        if ( bRender ){
            render.begin();
            ofClear(0,0);
            metaballs.draw();
            render.end();
            render.readToPixels(pix);
            metaballProcessor.findContours(pix);
        }
        
        static ofPath p;
        ofSetColor(255);
        //ofEnableBlendMode(OF_BLENDMODE_ADD);
        for ( int i=0; i<metaballProcessor.size(); i++){
            p.clear();
            tess.tessellateToMesh(metaballProcessor.getPolyline(i), OF_POLY_WINDING_ODD, drawMesh);
            for ( auto & vec : drawMesh.getVertices() ){
                drawMesh.addTexCoord(vec + texCoordShift);
                if ( bInd ) drawMesh.addColor( ofFloatColor( metaballs.color.r, metaballs.color.g, metaballs.color.b) );
                else drawMesh.addColor( ofFloatColor( 1.0 ) );
                vec.z = -5 + 0;
            }
            if ( bInd ){
                drawImg[0].bind();
            }
            drawMesh.draw();
            if ( bInd ){
                drawImg[0].unbind();
            }
        }
    } else if ( mode == MODE_PHOTO ){
        ofPushMatrix();
        ofTranslate(0, 0, -5);
        drawImg[0].draw(0,0);
        ofPopMatrix();
    }

    if ( bInd && mode == MODE_BUBBLES){
        drawImg[0].unbind();
    }
}

//--------------------------------------------------------------
void ofApp::toggleGuiVisible(){
    
    for ( auto * gui : guis ){
        gui->toggleVisible();
    }
}

//--------------------------------------------------------------
void ofApp::toggleAdvVisible(){
    for ( auto * gui : guisAdvanced ){
        gui->toggleVisible();
    }
}
//--------------------------------------------------------------
void ofApp::saveGui(){
    int i=0;
    for ( auto * gui : guis ){
        gui->saveSettings("settings_"+ofToString(i)+".xml");
        i++;
    }
    
    for ( auto * gui : guisAdvanced ){
        gui->saveSettings("settings_"+ofToString(i)+".xml");
        i++;
    }
}

//--------------------------------------------------------------
void ofApp::onImageLoaded( ofImage & img ){
    ofImage temp; temp.clone(img);
    setupImage(temp);
    for (int i=0; i<numFinder; i++){
        contourFinders[i]->loadPixels(temp);
    }
    lastContrast = -1;
    randomizeColors();
    bubbles.burst();
}


//--------------------------------------------------------------
void ofApp::onFileLoaded( string & img ){
    gui.setTitle(img);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::windowResized(int w, int h){
    bubbles.resize(w,h);
    randomizeColors();
    render.allocate(ofGetWidth(), ofGetHeight());
    imageFBO.allocate(ofGetWidth(), ofGetHeight());
    
    saveButton.x = ofGetWidth() - saveButton.width - 20;
    saveButton.y = ofGetHeight() - saveButton.height * 2 - 30;
    
    randomButton.x = ofGetWidth() - saveButton.width - 20;
    randomButton.y = ofGetHeight() - saveButton.height - 20;
    
    gui.x = fmax(20, floor(ofGetWidth() - gui.getWidth()-20));
    gui.y = 20;
}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
