#include "ofApp.h"

bool bInvert = true;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    screen.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 8);
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( toLoad != "" ){
        ofFile f(toLoad);
        string output = f.getEnclosingDirectory();
        string name   = f.getBaseName() + "_colored";
        string ext    = f.getExtension();
        img.clear();
        
        if ( img.loadImage(toLoad) ){
            // do stuff
            
            currentMode = (TriMode) floor(ofRandom(0, MODE_RGB+1));
            
            screen.begin();
            ofColor c;
            c.setSaturation(100);
            c.setBrightness(200);
            c.setHue(ofRandom(255));
            ofSetColor(c.r, c.g, c.b);
            ofRect(0,0,screen.getWidth(), screen.getHeight());
            float ran = ofRandom(-.2,.2);
            int mult = ofRandom(1, 10);
            int num = 3 * mult;
            float scaleMult = ofRandom(.1,.9);
            ofColor tint;
            tint.setHue(ofRandom(255));
            tint.setSaturation(100);
            tint.setBrightness(255);
//            tint.a = 200;
            ofPoint offset(ofRandom(-.1,.1), ofRandom(-.1,.1));
            
            cv::Mat cv = ofxCv::toCv(img);
            cv.convertTo(cv, -1,2,0);
            if ( bInvert ) ofxCv::invert(cv);
//            ofxCv::threshold(cv, 150);
            ofxCv::toOf(cv,img);
            img.update();
            
            switch (currentMode){
                case MODE_CONCENTRIC:
                    for (int i=0; i<num; i++){
                        float maxs = (float) ofGetWidth()/img.width;
                        float s = ofMap(i,0,num-1,maxs, scaleMult * maxs);
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
                        ofScale(s, s);
                        img.draw(-img.width/2.0,-img.height/2.0);
                        ofPopMatrix();
                    }
                    break;
                    
                case MODE_ROTATE:
                    for (int i=0; i<num; i++){
                        float maxs = (float) ofGetWidth()/img.width;
                        float s = ofMap(i,0,num-1,maxs, scaleMult * maxs);
                        float r = ofMap(i,0,num-1,0, ran * 360.0f);
                        if ( i % mult == 0 ){
                            tint.setHue(ofRandom(255));
                        }
                        offset.set(ofSignedNoise(i * .001),ofSignedNoise(i * .001));
                        //offset.set(ofRandom(-.1,.1), ofRandom(-.1,.1));
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
                        ofScale(s, s);
                        ofRotate(r, 0, 0, 1);
                        ofSetColor(tint);
                        img.draw(-img.width/2.0 + offset.x * img.width,-img.height/2.0 + offset.y * img.height);
                        ofPopMatrix();
                    }
                    break;
                    
                case MODE_STRIPS:
                {
                    for ( int i=0; i<num; i++){
                        float max = ofMap(i, 0, num, 1.0, .1);
                        float x = ofRandom(0,img.width/2.0);
                        float y = ofRandom(0,img.height/2.0);
                        float w = ofRandom((img.width * max - x)/2.0, img.width * max - x);
                        float h = ofRandom(((img.height * max) - y)/2.0, (img.height * max) - y);
                        ofImage temp(img);
                        temp.crop(x,y, w, h);
                        temp.draw(ofRandom(0,ofGetWidth() - (w)), ofRandom(0, ofGetHeight() - (h)));
                    }
                }
                    break;
                    
                case MODE_RGB:
                    
                    ofPixels r = img.getPixelsRef().getChannel(0);
                    ofPixels g = img.getPixelsRef().getChannel(1);
                    ofPixels b = img.getPixelsRef().getChannel(2);
                    
                    r.setImageType(OF_IMAGE_COLOR_ALPHA);
                    g.setImageType(OF_IMAGE_COLOR_ALPHA);
                    b.setImageType(OF_IMAGE_COLOR_ALPHA);
                    
                    
                    ofColor red, gre, blu;
                    red.setSaturation(100);
                    red.setBrightness(200);
                    gre.setSaturation(100);
                    gre.setBrightness(200);
                    blu.setSaturation(100);
                    blu.setBrightness(200);
                    red.setHue(ofRandom(255));
                    gre.setHue(ofRandom(255));
                    blu.setHue(ofRandom(255));
                    
                    for ( int i=0; i<r.size(); i+= r.getNumChannels()){
//                        r[i+0] = ofMap(r[i], 0, 255, 255, 0);
//                        g[i+1] = ofMap(g[i+1], 0, 255, 255, 0);
//                        b[i+2] = ofMap(b[i+2], 0, 255, 255, 0);
                        //
                        r[i+3] = r[i];//ofMap(r[i], 0, 255, 0, 0);;
                        g[i+3] = g[i+1];//ofMap(g[i+1], 0, 255, 0, 0);;
                        b[i+3] = b[i+2];//ofMap(b[i+2], 0, 255, 0, 0);
//                        
//                        r[i+1] = r[i+2] = 0;
//                        g[i+0] = g[i+2] = 0;
//                        b[i+0] = b[i+1] = 0;

                        r[i]    = r[i+3]/255.0 * red.r;
                        r[i+1]  = r[i+3]/255.0 * red.g;
                        r[i+2]  = r[i+3]/255.0 * red.b;
                        
                        g[i]    = g[i+3]/255.0 * gre.r;
                        g[i+1]  = g[i+3]/255.0 * gre.g;
                        g[i+2]  = g[i+3]/255.0 * gre.b;
                        
                        b[i]    = b[i+3]/255.0 * blu.r;
                        b[i+1]  = b[i+3]/255.0 * blu.g;
                        b[i+2]  = b[i+3]/255.0 * blu.b;
                    }
                    
                    ofImage i1, i2, i3;
                    i1.setFromPixels(r);
                    i2.setFromPixels(g);
                    i3.setFromPixels(b);
                    
                    ofSetColor(255);
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
                    ofScale(.75,.75);
                    i1.draw(-img.width/2.0, -img.height/2.0);
                    i2.draw(-img.width/2.0 + img.width/64.0, -img.height/2.0 + img.width/64.0);
                    i3.draw(-img.width/2.0 + img.width/32.0,-img.height/2.0 + img.width/32.0);
                    ofPopMatrix();
                    ofEnableAlphaBlending();
                    break;
            }
            screen.end();
        }
        toLoad = "";
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    screen.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key =='b') bInvert = !bInvert;
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
        toLoad = s;
        break;
    }
}
