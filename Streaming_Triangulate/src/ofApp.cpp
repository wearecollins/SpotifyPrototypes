#include "ofApp.h"

float lastThresh = -1;
float lastDensity = -1;
float radius = 0.0;

bool bUseShader = false;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    
    triangulator.density = 30.0;
    
    gui = new ofxUICanvas(0,0, ofGetWidth()/3.0, ofGetHeight());
    gui->addSlider("triangulator.threshold", .1, 300.0, &triangulator.threshold);
    gui->addSlider("blur radius", 0, 100, &radius);
    gui->addSlider("triangle density", 0, 100, &triangulator.density);
    
    blurFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    blurHoriz.load("", "shaders/horizontalBlur.frag");
    blurVert.load("", "shaders/verticalBlur.frag");
    
#ifdef USE_LIVE_VIDEO
    cam.initGrabber(640, 480);
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
    bool bTriangulate = false;
    
    if ( lastThresh != triangulator.threshold || lastDensity != triangulator.density){
        lastThresh = triangulator.threshold;
        lastDensity = triangulator.density;
        bTriangulate = true;
    }
    
#ifdef USE_LIVE_VIDEO
    cam.update();
    if ( cam.isFrameNew() ){
        bTriangulate = false;
        toTriangulate.setFromPixels(cam.getPixelsRef());
        triangulator.process( toTriangulate );
    }
#else
    if ( load != "" ){
        bTriangulate = false;
        toTriangulate.loadImage(load);
        triangulator.process( toTriangulate );
        toTriangulate.setImageType(OF_IMAGE_GRAYSCALE);
        load = "";
    }
#endif
    
    if ( bTriangulate ){
        triangulator.reprocess();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (toTriangulate.isAllocated()){
        
        if ( bUseShader ){
            blurHoriz.begin();
            blurHoriz.setUniform1f("radius", radius);
            blurFbo.begin();
            ofClear(255,0);
            toTriangulate.draw(0,0);
            blurFbo.end();
            blurHoriz.end();
            
            blurVert.begin();
            blurVert.setUniform1f("radius", radius);
            blurFbo.draw(0,0);
            blurVert.end();
        } else {
            toBlur.setFromPixels(toTriangulate.getPixelsRef());
            
            cv::Mat mat = ofxCv::toCv(toBlur);
            int b = ((int) radius * 2 % 2 == 0 ? radius * 2 +1 : radius * 2);
            cv::blur(mat, mat, cv::Size(b,b));
            ofxCv::toOf(mat, toDraw);
            toDraw.update();
            toDraw.draw(0,0);
        }
        
        //toTriangulate.bind();
        triangulator.draw();
        //toTriangulate.unbind();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ){
        gui->toggleVisible();
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
    for ( auto & f : dragInfo.files ){
        load = f;
        break;
    }
}
