#include "ofApp.h"

int numpoints = 10000;
vector<ofColor> colors;
vector<ofMesh> meshes;
int num = 10;

bool bDebug = false;
int easingType = 2;
int bufferSize = 1024;
float pos = 0.0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    bool bMeshExists = false;
    string fileName = "";
    ofDirectory dir; dir.allowExt("mp3");
    for ( int i=0; i<dir.listDir("songs"); i++){
        mesh.load(dir.getPath(i), numpoints, true);
        break;
    }
    
    for ( int i=0; i<num; i++){
        colors.push_back(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
        colors.back().setSaturation(100);
        colors.back().setBrightness(200);
        
        float y = ofMap(i, 0, num, 0.0, ofGetHeight());
        float smooth = ofMap(i, 0, num, 1000.0, 50.0);
        float points = numpoints;//ofMap(i, 0, 100, numpoints/2.0, numpoints);
        
        float t = ofMap(i, 0, num, 0.0, 1.0);
        
        switch (easingType) {
            case 0:
                y = ofMap(easingLinear.easeOut(t), 0.0, 1.0, 0.0, ofGetHeight());
                smooth = ofMap(easingLinear.easeOut(t), 0.0, 1.0, 1000.0, 50.0);
                break;
            case 1:
                y = ofMap(easingCubic.easeOut(t), 0.0, 1.0, 0.0, ofGetHeight());
                smooth = ofMap(easingCubic.easeOut(t), 0.0, 1.0, 1000.0, 50.0);
                break;
            case 2:
                y = ofMap(easingCubic.easeIn(t), 0.0, 1.0, 0.0, ofGetHeight());
                smooth = ofMap(easingCubic.easeIn(t), 0.0, 1.0, 1000.0, 50.0);
                break;
            case 3:
                y = ofMap(easingQuad.easeIn(t), 0.0, 1.0, 0.0, ofGetHeight());
                smooth = ofMap(easingQuad.easeIn(t), 0.0, 1.0, 1000.0, 50.0);
                break;
        }
        
        mesh.smooth(smooth, points);
        
        ofMesh m( mesh.getMesh() );
        
        for ( auto & v : m.getVertices() ){
            v *= ofVec2f( ofGetWidth() + 200, ofGetHeight()/2.0);
            
            v.x += -100;
            v.y += y;
        }
        for ( auto & index : m.getIndices() ){
            ofVec3f v = m.getVertex(index);
            m.setTexCoord(index, v);
        }
        meshes.push_back(m);
    }
    
    // FFT
	bufferSize = 512;
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    
	audioBuffer.resize(bufferSize);
	audioBins.resize(fft->getBinSize());
}

float lastX = -1;
float lastY = -1;

//--------------------------------------------------------------
void ofApp::updateFFT(){
    memcpy(&audioBuffer[0], mesh.getSnippet(bufferSize, .5), sizeof(float) * bufferSize);
    
    float maxValue = 0;
    for(int i = 0; i < bufferSize; i++) {
        if(abs(audioBuffer[i]) > maxValue) {
            maxValue = abs(audioBuffer[i]);
        }
    }
    for(int i = 0; i < bufferSize; i++) {
        audioBuffer[i] /= maxValue;
    }
    
    fft->setSignal( &audioBuffer[0] );
    
    float* curFft = fft->getAmplitude();
	memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
    
	maxValue = 0;
	for(int i = 0; i < fft->getBinSize(); i++) {
		if(abs(audioBins[i]) > maxValue) {
			maxValue = abs(audioBins[i]);
		}
	}
	for(int i = 0; i < fft->getBinSize(); i++) {
		audioBins[i] /= maxValue;
	}
    pos += .001;
    if ( pos > 1.0 ) pos = 0.0;
}

//--------------------------------------------------------------
void ofApp::update(){
    updateFFT();
    if ( toLoad.size() != 0 ){
        for ( string & s : toLoad ){
            images.push_back(ofImage());
            images.back().loadImage(s);
            
            float sc = fmax ( (float) ofGetWidth()/images.back().width, (float) ofGetHeight()/images.back().height);
            images.back().resize(images.back().width * sc, images.back().height * sc);
        }
        toLoad.clear();
    }
    
    if ( bDebug ){
        if ( mouseX != lastX || mouseY != lastY ){
            lastX = mouseX;
            lastY = mouseY;
            
            float smooth = ofMap(mouseX, 0, ofGetWidth(), 0.0, 500.0);
            float points = ofMap(mouseY, 0, ofGetHeight(), 10.0, numpoints);
            mesh.smooth(smooth, points);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int i =0;
    bool bMap = images.size() == 1 ? false : true;
    for ( auto & m : meshes ){
        int index = -1;
        if ( images.size() != 0 ) index = i % images.size();
        ofSetColor(colors[i]);
        if ( index != -1 && bMap) images[index].bind();
        m.draw();
        if ( index != -1 && bMap) images[index].unbind();
        i++;
        if ( images.size() == 1 ) bMap = !bMap;
    }
    
    if ( bDebug ) mesh.draw(-100, ofGetHeight()/2.0, ( ofGetWidth() + 200) );
    plot(audioBuffer, 128.0 / 2, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == '=' ){
        easingType++;
        if ( easingType > 3 ) easingType = 0;
    } else if ( key =='-'){
        easingType--;
        if ( easingType < 0 ) easingType = 3;
    } else if ( key == 'c' ){
        images.clear();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    toLoad.swap(dragInfo.files);
}

//--------------------------------------------------------------
void ofApp::plot(vector<float>& buffer, float scale, float offset) {
	ofNoFill();
    ofSetColor(255);
	
	int n = buffer.size();
	ofRect(0, 0, n, 128);
	glPushMatrix();
	glTranslatef(0, 128.0 / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < n; i++) {
		ofVertex(i, buffer[i] * scale);
	}
	ofEndShape();
	glPopMatrix();
}
