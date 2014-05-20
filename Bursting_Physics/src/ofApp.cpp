#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( 10, 10, 10);
    
    //camera.enableOrtho();
	camera.setPosition(ofVec3f(-ofGetWidth(), -ofGetHeight(), -100.f));
    
	world.setup();
	world.enableGrabbing();
	world.enableDebugDraw();
	world.setCamera(&camera);
//    world.setGravity(ofVec3f(-1,-1,-1));
	
    for ( int i=0; i<100; i++){
        ofxBulletSphere * sphere = new ofxBulletSphere();
        sphere->create(world.world, ofVec3f(0, 5, 0), 1.0, .25);
        sphere->setFriction(0.0);
        sphere->setRestitution(1.5);
        sphere->add();
        spheres.push_back(sphere);
    }
    
    sphere = ofMesh::sphere(spheres[0]->getRadius());
	
	ground.create( world.world, ofVec3f(0., 5.5, 0.), 0., 100.f, 1.f, 100.f );
	ground.setProperties(.25, .95);
	ground.add();
}

//--------------------------------------------------------------
void ofApp::update(){
	world.update();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
    camera.setPosition(ofVec3f(-1, -2, 0.0));
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    
    camera.begin();
	
	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);
	//world.drawDebug();
	
	ofSetColor(100, 100, 100);
	ground.draw();
	
	ofSetColor(225, 225, 225);
	
    for ( auto * bsphere : spheres ){
        bsphere->transformGL();
        sphere.draw();
        bsphere->restoreTramsformGL();
        //sphere->draw();
    }
    
    camera.end();
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

}
