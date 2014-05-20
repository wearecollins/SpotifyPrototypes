#include "ofApp.h"

float num   = 20;
float a     = 360.0 / num;

float rad   = 50;

float modAmount = 10;

ofPath p;

bool compX(const ofVec3f& a, const ofVec3f& b) {  return a.x > b.x; };
bool compXmin(const ofVec3f& a, const ofVec3f& b) {  return a.x < b.x; };
bool compY(const ofVec3f& a, const ofVec3f& b) {  return a.y > b.y; };
bool compYmin(const ofVec3f& a, const ofVec3f& b) {  return a.y < b.y; };

bool compareSorts( const Sortable& a, const Sortable& b ){ return a.max < b.max; };


ofColor c(ofRandom(255), ofRandom(255), ofRandom(255));

//--------------------------------------------------------------
void ofApp::setup(){
    c.setSaturation(150);
    generateMeshes(10);
}

//--------------------------------------------------------------
void ofApp::generateMeshes( int num ){
    
    vector<Sortable> sorts;
    vector<ofMesh> temp;
    
    float x = ofGetWidth()/2.0 + ofRandom(-ofGetWidth()/5.0, ofGetWidth()/5.0);
    float y = ofGetHeight()/2.0 + ofRandom(-ofGetHeight()/5.0, ofGetHeight()/5.0 );
    
    for ( int i=0; i<num; i++){
        temp.push_back( generate( x, y) );
        
        
        if ( ofRandom(10.0) > 8 ){
            c.set(ofRandom(255), ofRandom(255), ofRandom(255));
            c.setSaturation(150);
            x = ofGetWidth()/2.0 + ofRandom(-ofGetWidth()/5.0, ofGetWidth()/5.0);
            y = ofGetHeight()/2.0 + ofRandom(-ofGetHeight()/5.0, ofGetHeight()/5.0 );
        } else {
            x += ofRandom(-rad * 2.0, rad * 2.0);
            y += ofRandom(-rad * 2.0, rad * 2.0);
        }
        
        Sortable s;
        auto maxX = std::max_element(temp.back().getVertices().begin(), temp.back().getVertices().end(), compX);
        auto maxY = std::max_element(temp.back().getVertices().begin(), temp.back().getVertices().end(), compY);
        auto minX = std::max_element(temp.back().getVertices().begin(), temp.back().getVertices().end(), compXmin);
        auto minY = std::max_element(temp.back().getVertices().begin(), temp.back().getVertices().end(), compYmin);

        s.max = fmax( (*maxX).x - (*minX).x, (*maxY).y - (*minY).y );
        s.index = i;
        sorts.push_back(s);
    }
    
    std::sort(sorts.begin(), sorts.end(), compareSorts);
    sources.clear();
    draws.clear();
    
    for ( auto s : sorts ){
        cout << s.max << endl;
        sources.push_back(temp[s.index]);
        draws.push_back(temp[s.index]);
    }
}

//--------------------------------------------------------------
ofMesh ofApp::generate( float x, float y ){
    num   = ofRandom(5,20);
    
    if ( (int) num % 2 == 0 ) num+=1;
    
    a     = 360.0 / num;
    ofMesh cached;
    
    p.clear();
    
    ofVec3f center(x,y);
    
    //p.moveTo(center);
    
    float min = ofRandom(1.0);
    
    float scaleX = ofRandom(.5 * min, 4.0 * min );
    float scaleY = ofRandom(.5 * min, 4.0 * min );
    
    float liveRad = rad;
    
    float offset = ofRandom(5.0);
    double mod    = 1.0;// / num;
    
    for (float i=a; i<360; i+=a){
        liveRad = rad + ofSignedNoise(offset +  ((i/360.0f) * mod)) * ofRandom(rad * .1, rad * 1.5);
        ofVec3f v;
        v.x = cos(ofDegToRad(i)) * liveRad * scaleX;
        v.y = -sin(ofDegToRad(i)) * liveRad * scaleY;
        p.lineTo( center + v );
    }
    
    // close @ 360
    liveRad = rad + ofSignedNoise((offset + 360.0) * .02) * ofRandom(rad * .1, rad * 1.5);
    ofVec3f v;
    v.x = cos(ofDegToRad(360.0)) * liveRad * scaleX;
    v.y = -sin(ofDegToRad(360.0)) * liveRad * scaleY;
    p.lineTo( center + v );
    
    p.close();
    
    cached = ofMesh( p.getTessellation() );
    for ( int i=0; i<cached.getNumVertices(); i++ ){
        if ( cached.getNumColors() > i ) cached.setColor(i, c);
        else cached.addColor(c);
        
        ofVec3f v = cached.getVertex(i);
//        v.x *= scaleX;
//        v.y *= scaleY;
        cached.setVertex(i, v);
    }
    
    bRegen = false;
    return cached;
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( bRegen ){
        generateMeshes(10);
    }
    
    int index = 0;
    for ( index; index<sources.size(); index++){
        for ( int i=0; i<sources[index].getNumVertices(); i++ ){
            float x = sources[index].getVertex(i).x;
            float y = sources[index].getVertex(i).y;
            ofVec3f v(sources[index].getVertex(i));
            v.x += ofSignedNoise(x + ofGetElapsedTimeMillis() * .0001, y + ofGetElapsedTimeMillis() * .0002) * modAmount;
            v.y += ofSignedNoise(x + ofGetElapsedTimeMillis() * .0003, y + ofGetElapsedTimeMillis() * .0005) * modAmount;
            draws[index].setVertex(i, v);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( auto & m : draws ){
        m.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'r' ) bRegen = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    modAmount = x;
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

}
