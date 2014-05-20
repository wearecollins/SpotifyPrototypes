#include "ofApp.h"

bool bDrawEdges = false;
ofVec2f center(130, 690);

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
    ofBackground(255);
    
    // Box2d
	box2d.init();
	box2d.setGravity(0.0, 0.0);
	box2d.createBounds(ofRectangle(0,0,ofGetWidth(), ofGetHeight()));
	box2d.setFPS(30.0);
	
	
	// load the lines we saved...
	/*ifstream f;
	f.open(ofToDataPath("lines.txt").c_str());
	vector <string> strLines;
	while (!f.eof()) {
		string ptStr;
		getline(f, ptStr);
		strLines.push_back(ptStr);
	}
	f.close();
	
	for (int i=0; i<strLines.size(); i++) {
		vector <string> pts = ofSplitString(strLines[i], ",");
		if(pts.size() > 0) {
			ofPtr <ofxBox2dEdge> edge = ofPtr<ofxBox2dEdge>(new ofxBox2dEdge);
			for (int j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]);
					float y = ofToFloat(pts[j+1]);
					edge.get()->addVertex(x, y);
				}
			}
			edge.get()->create(box2d.getWorld());
			edges.push_back(edge);
		}
	}
    */
    
    ofPtr <ofxBox2dEdge> edge1 = ofPtr<ofxBox2dEdge>(new ofxBox2dEdge);
    edge1.get()->addVertex(center.x, center.y);
    edge1.get()->addVertex(445, 0);
    edge1.get()->create(box2d.getWorld());
    edges.push_back(edge1);
    
    ofPtr <ofxBox2dEdge> edge2 = ofPtr<ofxBox2dEdge>(new ofxBox2dEdge);
    edge2.get()->addVertex(center.x, center.y);
    edge2.get()->addVertex(ofGetWidth(), 420);
    edge2.get()->create(box2d.getWorld());
    edges.push_back(edge2);
    
    // create *numCircles* circles at the origin
    int numCircles = 500;
    
    for ( int i=0; i<numCircles; i++){
        ofPtr<TextureCircle> c = ofPtr<TextureCircle>(new TextureCircle);
		c.get()->setPhysics(0.2, 0.2, 0.002);
		c.get()->setup(box2d.getWorld(), center.x + 100, center.y - 100, ofRandom(.1, 20));
        c.get()->setVelocity(ofRandomf() * 10.0, ofRandomf() * 10.0);
		circles.push_back(c);
    }
    
    trails.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 3.0));
	box2d.update();
    
    if ( toLoad != "" ){
        toBind.loadImage(toLoad);
        toLoad = "";
        
        for (int i=0; i<circles.size(); i++) {
            circles[i].get()->setPosition(center.x + 100, center.y - 100);
            circles[i].get()->setVelocity(ofRandomf() * 10.0, ofRandomf() * 10.0);
            circles[i].get()->setFriction(0.0);
            circles[i].get()->setDamping(1.0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    trails.begin();
    ofClear(255);
    //ofSetColor(255,10);
    //ofRect(0,0,ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    for (int i=0; i<circles.size(); i++) {
		ofFill();
        if ( toBind.isAllocated() ) toBind.bind();
		circles[i].get()->draw();
        if ( toBind.isAllocated() ) toBind.unbind();
	}
    trails.end();
    if ( toBind.isAllocated() ) toBind.draw(0,0);
    
    trails.draw(0,0);
    
    if ( bDrawEdges ){
        ofNoFill();
        for (int i=0; i<lines.size(); i++) {
            lines[i].draw();
        }
        for (int i=0; i<edges.size(); i++) {
            edges[i].get()->draw();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
        toLoad = f;
        break;
    }
}
