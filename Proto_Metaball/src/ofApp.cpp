#include "ofApp.h"

bool bRandomize = false;
bool bAnimating = false;
ofPixels pix;

string loadme = "";
ofTessellator tess;
ofMesh drawMesh;

int numMeta = 3;

ofColor colorA, colorB;

bool bRandomizeColor = false;
bool bDrawLogo = true;

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    render.allocate(ofGetWidth(), ofGetHeight());
    for ( int i=0; i<numMeta; i++){
        metaballs.push_back(Metaballs());
        metaballs.back().setup();
        metaballs.back().color.set(1.0);
    }
    contourFinder.setMinAreaRadius(20);
//    contourFinder.setMaxAreaRadius(1000);
    
    colors.setup();
    logo.setup();
    screen.setup();
    
    randomizeColor();
}

//--------------------------------------------------------------
void ofApp::randomizeColor(){
    vector<ofColor> pair = colors.getHighLowPair();
    colorA = pair[0];
    colorB = pair[1];
    imageProcessor.setColorPair(0, colorB, colorA);
    ofBackground(colorB);
}

//--------------------------------------------------------------
void ofApp::setupImage( ofImage & img ){
    float w, h;
    if ( ofGetWidth() > ofGetHeight() ){
        w = ofGetWidth();
        h = img.height * w/img.width;
    } else {
        h = ofGetHeight();
        w = img.width * h/img.height;
    }
    img.resize(w, h);
    
    // crop
    img.crop((w - ofGetWidth())/2.0, (h-ofGetHeight())/2.0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( bRandomizeColor ){
        bRandomizeColor = false;
        randomizeColor();
        
        for ( int i=0; i<images.size(); i++){
            images[i].loadImage(imagesProcessed[i]);
            setupImage(images[i]);
            
            imageProcessor.process(images[i]);
        }
    }
    
    if ( loadme != "" ){
        images.push_back(ofImage());
        images.back().loadImage(loadme);
        
        imagesProcessed.push_back(loadme);
        
        setupImage(images.back());
        
        imageProcessor.process(images.back());
        loadme = "";
    }
    if ( bRandomize){
        metaballs.clear();
        int num = ofMap(mouseY, 0, ofGetHeight(), 1, 10);
        for ( int i=0; i<numMeta; i++){
            metaballs.push_back(Metaballs());
            metaballs.back().setup(ofMap(mouseY, 0, ofGetHeight(), 10, 500, true));
            metaballs.back().color.set(1.0);
            metaballs.back().update();
        }
        bRandomize = false;
    }
    for ( auto & m : metaballs ){
        if ( bAnimating) m.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( ofGetKeyPressed(OF_KEY_SHIFT)){
        //contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255, true));
    } else {
        for ( auto & m : metaballs ){
            m.separation = ofMap(mouseX, 0, ofGetWidth(), 0, 500, true);
            if ( screen.mode == 3){
                m.center.set(ofGetWidth() * .25, ofGetHeight()/2.0);
                logo.scale = (ofGetWidth() * .7) / logo.width;
            } else if ( screen.mode == 4 ){
                m.center.set(ofGetWidth() * .5, ofGetHeight() * .75);
                logo.scale = (ofGetWidth() * .9) / logo.width;
            } else {
                m.center.set(ofGetWidth() * .5, ofGetHeight() * .5);
                logo.scale = (ofGetWidth() * .8) / logo.width;
            }
        }
    }
    ofPath p;
    
    bool bMap = true;
    int j = 0;
    
    if ( bDrawLogo ) logo.setColor(colorB);
    else logo.setColor(colorA);
    //if ( bDrawLogo ){
        ofEnableDepthTest();
    ofPushMatrix();
    if ( screen.mode == 3){
        ofTranslate(-ofGetWidth() * .25, 0);
    } else if ( screen.mode == 4 ){
        ofTranslate(0, ofGetHeight()* .25);
    }
        logo.draw();
    ofPopMatrix();
    //}
    
    ofVec2f offset(0,0);
    if ( screen.mode == 3){
        offset.x = ofGetWidth() * .25;
    } else if ( screen.mode == 4 ){
    } else {
    }
    
    for ( auto & m : metaballs ){
        render.begin();
        ofClear(0,0);
        m.draw();
        render.end();
        render.readToPixels(pix);
        contourFinder.findContours(pix);
        
        ofSetColor(255);
        //ofEnableBlendMode(OF_BLENDMODE_ADD);
        for ( int i=0; i<contourFinder.size(); i++){
            p.clear();
            int cnt = ofMap(mouseY, 0, ofGetHeight(), 3, contourFinder.getPolyline(i).size() / 4.0);
            ofPolyline pl = contourFinder.getPolyline(i).getResampledByCount( cnt );
            tess.tessellateToMesh(pl, OF_POLY_WINDING_ODD, drawMesh);
            for ( auto & ind : drawMesh.getVertices() ){
                drawMesh.addTexCoord(ind + offset);
                if ( bMap ) drawMesh.addColor( ofFloatColor( m.color.r, m.color.g, m.color.b) );
                else drawMesh.addColor( ofFloatColor( colorB.r/255.0f, colorB.g/255.0f, colorB.b/255.0f) );
                ind.z = -3 + j;
            }
            int index = -1;
            
            if ( images.size() != 0){
                index = bMap ? j % images.size() : -1;
            }
            if ( index != -1 ) images[index].bind();
            drawMesh.draw();
            if ( index != -1 ) images[index].unbind();
        }
        if ( imagesProcessed.size() == 1) bMap = !bMap;
        j++;
    }
    
    //render.draw(0,0);
    //contourFinder.draw();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'r') bRandomize = true;
    else if (key =='a') bAnimating = !bAnimating;
    else if (key == ' ' ) bRandomizeColor = !bRandomizeColor;
    else if ( key == 'c' ){
        images.clear();
        imagesProcessed.clear();
    } else if ( key == 'l' ){
        bDrawLogo = !bDrawLogo;
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
        loadme = s;
        break;
    }
}
