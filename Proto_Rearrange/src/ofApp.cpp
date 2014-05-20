#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    img.loadImage("photo.png");
    rearrange();
}

//--------------------------------------------------------------
void ofApp::rearrange(){
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    float width = img.width;
    float height = img.height;
    float rowCount = 10;//width / 16.0;
    float columnCount = 10;//height / 16.0;
    
    float x,y,z;
    
    int num = ofRandom(3, 6);
    
    vector<int> rows;
    vector<int> cols;
    
    for ( int i=0; i<num; i++){
        rows.push_back(ofRandom(10));
        cols.push_back(ofRandom(10));
    }
    
    for(int row=0; row < rowCount; row++){
        for (int col=0; col < columnCount; col++) {
            
            // set current x y z
            x = width * (col/(columnCount-1));
            y = height * (row/(rowCount-1));
            z = 0;
            
            mesh.addVertex(ofVec3f(x, y, z));
            if ( ofRandom(100) > 20 ){
                mesh.addTexCoord(ofVec2f( x,y ));
                mesh.addColor(ofFloatColor(1,1,1,1));
            } else {
                mesh.addTexCoord(ofVec2f( ofRandom(x-100,x+100), ofRandom(y-100,y+100) ));
                mesh.addColor(ofFloatColor(ofRandom(1.0),ofRandom(1.0),ofRandom(1.0),1));
            }//col/(columnCount-1), row/(rowCount-1)) );
        }
    }
    
    int n = 0;
    int colSteps = columnCount * 2;
    int rowSteps = rowCount - 1;
    vector<ofIndexType> indices;
    
    for ( int r = 0; r < rowSteps; r++ ) {
        for ( int c = 0; c < colSteps; c++ ) {
            int t = c + r * colSteps;
            if ( c == colSteps - 1 ) {
                indices.push_back( n );
            } else {
                indices.push_back( n );
                ( t%2 == 0 ) ? n += columnCount : (r%2 == 0) ? n -= (columnCount-1) : n -= (columnCount+1);
            }
        }
    }
    mesh.addIndices(indices);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    img.bind();
    mesh.draw();
    img.unbind();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'r' )
        rearrange();
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

}
