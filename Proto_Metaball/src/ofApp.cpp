#include "ofApp.h"

bool bRandomize = false;
bool bAnimating = false;
ofPixels pix;

string loadme = "";
ofTessellator tess;
ofMesh drawMesh;

int numMeta = 1;

ofColor colorA, colorB;

bool bRandomizeColor = false;

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
    
    randomizeColor();
}

//--------------------------------------------------------------
void ofApp::randomizeColor(){
    colorA = rc::ofRandomRCColor();
    colorB = rc::ofRandomRCColor();
    imageProcessor.setColorPair(0, colorA, colorB);
    ofBackground(colorB);
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( bRandomizeColor ){
        bRandomizeColor = false;
        randomizeColor();
        
        for ( int i=0; i<images.size(); i++){
            imagesProcessed[i] = ofImage(images[i]);
            imageProcessor.process(imagesProcessed[i]);
        }
        
    }
    if ( loadme != "" ){
        images.push_back(ofImage());
        images.back().loadImage(loadme);
        
        imagesProcessed.push_back(ofImage());
        imagesProcessed.back().loadImage(loadme);
        imageProcessor.process(imagesProcessed.back());
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
        }
    }
    ofPath p;
    
    bool bMap = true;
    int j = 0;
    
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
                drawMesh.addTexCoord(ind);
                drawMesh.addColor( ofFloatColor( m.color.r, m.color.g, m.color.b, .5) );
            }
            int index = -1;
            
            if ( imagesProcessed.size() != 0){
                index = bMap ? j % imagesProcessed.size() : -1;
            }
            if ( index != -1 ) imagesProcessed[index].bind();
            drawMesh.draw();
            if ( index != -1 ) imagesProcessed[index].unbind();
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
