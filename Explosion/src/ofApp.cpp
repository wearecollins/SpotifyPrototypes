#include "ofApp.h"

ofColor start;
ofColor end;

bool bDrawLines = false;
int mode = 2;
ofTessellator tess;
vector<int> modes;

int moveType = 0;

bool bUseShader     = true;
bool bReloadShader  = false;
bool bLines         = false;

ofFloatColor color;
ofFloatColor center;

float pointSize = 10.0;

float numSprites = 16;
float lineWidth  = 10.0;

bool bNeedToRandomize = false;

float intensity = 0;
float mag = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    render.allocate(ofGetWidth() * 2.0, ofGetHeight() * 2.0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    charles.loadImage("charles.png");
    path.load("charles.svg");

    int n = path.getNumPath();
    for ( auto i=0; i<n; i++){
        ofPath & p = path.getPathAt(i);
        p.setColor(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
        p.setFilled(true);
        ofPolyline outline = p.getOutline()[0].getResampledBySpacing(5.0);
        vector<ofPolyline> lines; lines.push_back(outline);
        tess.tessellateToMesh(lines, OF_POLY_WINDING_ODD, pathMesh);
        
//        p.flagShapeChanged();
//        p.setPolyWindingMode(OF_POLY_WINDING_POSITIVE);
//        pathMesh = ofMesh(p.getTessellation());
    }
//    pathMesh.setMode(OF_PRIMITIVE_POINTS);
    
    if ( pathMesh.getNumColors() == 0 ){
        for ( int i=0; i<pathMesh.getNumVertices(); i++){
            pathMesh.addColor(ofFloatColor(1.0));
            rands.push_back(ofRandom(100));
        }
    }
    
    sourceMesh = ofMesh(pathMesh);
    
    start.setSaturation(200); start.setBrightness(200);
    start.setHue(ofRandomuf() * 255.0);
    
    end.setSaturation(200); end.setBrightness(200);
    end.setHue(ofRandomuf() * 255.0);
    
    easing = &quad;
    
    // setup shader
    bReloadShader = true;
    randomize();
    sprites.loadImage("sprites/sprites.png");
    raySprites.loadImage("sprites/raysprites.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( bReloadShader ){
        bReloadShader = false;
        if ( shader.isLoaded() ){
            shader.unload();
        }
        shader.setGeometryInputType(GL_POINTS);
        shader.setGeometryOutputType(GL_TRIANGLES);
        shader.setGeometryOutputCount(3 * 6);
//        shader.setGeometryOutputCount(shader.getGeometryMaxOutputCount());
        shader.load("shaders/basic.vert", "shaders/basic.frag", "shaders/basic.geom");
        
        if ( spriteShader.isLoaded() ){
            spriteShader.unload();
        }
        spriteShader.load("shaders/spriteShader");
        
        if ( lineShader.isLoaded() ){
            lineShader.unload();
        }
        lineShader.setGeometryInputType(GL_LINES);
        lineShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
        lineShader.setGeometryOutputCount(4);
        lineShader.load("shaders/lines.vert", "shaders/lines.frag", "shaders/lines.geom");
        
        if ( circleShader.isLoaded() ){
            circleShader.unload();
        }
        
        circleShader.setGeometryInputType(GL_POINTS);
        circleShader.setGeometryOutputType(GL_TRIANGLES);
        circleShader.setGeometryOutputCount(3 * 12);
        circleShader.load("shaders/circles.vert", "shaders/circles.frag", "shaders/circles.geom");
    }
    
    if ( bNeedToRandomize ){
        bNeedToRandomize = false;
        randomize();
    }
    
    for ( int i=0; i<burstMesh.getNumVertices(); i++){
        ofVec3f v = burstMesh.getVertex(i);
        v.set(burstSource.getVertex(i));
        switch (moveType) {
            case 0:
                v.x += ofSignedNoise(v.x/(intensity) + ofGetElapsedTimeMillis() * .001) * mag;
                break;
                
            case 1:
                v.x += ofSignedNoise(v.y/(intensity)+ ofGetElapsedTimeMillis() * .001) * mag;
                v.y += ofSignedNoise(v.y/(intensity)+ ofGetElapsedTimeMillis() * .001) * mag;
                break;
                
            case 2:
                v.z += ofSignedNoise(v.x/(intensity), v.y/(intensity) + ofGetElapsedTimeMillis() * .001) * mag;
                break;
        }
        burstMesh.setVertex(i, v);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();

    float max = 10;
    float dist = -10;
    
    ofEnableAlphaBlending();
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    if ( mode == 0 ){
        
        for (int i=max; i>=0; i--){
            ofPushMatrix();
            float pos = ofMap(i, 0, max, 0.0, 1.0);
            float s = 1.0;//ofMap(i, 0, max, 1.0, 2.0);
            ofScale(s * s, s);
            
            ofColor c;
            c.set(start);
            c.lerp(end, pos);
            c.a = 150.;
            
            if ( bDrawLines ){
            
                ofTranslate((-path.getWidth()/2.0), (-path.getHeight()/2.0  * s) + easing->easeIn(pos, 0, dist, 1.0));
                
                int n = path.getNumPath();
                for ( auto j=0; j<n; j++){
                    path.getPathAt(j).setColor(c);
                    path.getPathAt(j).setStrokeWidth(easing->easeIn(pos, 5.0, -4.9, 1.0));
                }
                
                path.draw();
            } else {
                //ofTranslate((-path.getWidth()/2.0), (-path.getHeight() * .5) * s + easing->easeIn(pos, 0, dist, 1.0));
                for ( auto & co : pathMesh.getColors() ){
                    co.set(c.r/255.0f, c.g/255.0f, c.b/255.0f);//, c.a/255.0f);
                }
                
                s = ofMap(i, 0, max, 1.0, 2.0);
                
                for ( int k=0; k<pathMesh.getNumVertices(); k++){
                    ofVec3f v = sourceMesh.getVertex(k);
                    v.x *= s;
                    v.x += (-path.getWidth()/2.0 * s);
                    
                    v.y *= s;
                    v.y += (-path.getHeight() * .5) * s + easing->easeIn(pos, 0, dist, 1.0);
                    v.y += ofSignedNoise(v.x/path.getWidth() + pos * .1 + ofGetElapsedTimeMillis() * .001) * 500.0;
                    v.y = ofClamp(v.y, -ofGetHeight() * .65, path.getHeight() * .25);
    //                v.y += ofSignedNoise(v.x + ofGetElapsedTimeMillis() * .0001) * 100.0;
                    pathMesh.setVertex(k, v);
                }
                if ( rands[i] > 50){
                    pathMesh.drawWireframe();
                } else if ( rands[i] > 60){
                    pathMesh.drawVertices();
                } else {
                    pathMesh.draw();
                }
                
            }
            ofPopMatrix();
        }
    } else {
        ofPushMatrix();
        render.begin();
        ofClear(255,255,255,0);
        //ofClear(center.r * 255, center.g * 255, center.b * 255, 0);
        
        if ( bUseShader ){
            burstMesh.setMode(OF_PRIMITIVE_POINTS);
            if ( mode == 1 ){
                shader.begin();
                shader.setUniform1f("pointSize", pointSize);
                shader.setUniform1f("screenSize", ofGetWidth());
                shader.setUniformTexture("charles", charles.getTextureReference(), 0);
            } else if ( mode == 2){
                ofEnablePointSprites();
                spriteShader.begin();
                spriteShader.setUniformTexture("inTex", sprites.getTextureReference(), 0);
                spriteShader.setUniformTexture("colorTex", raySprites.getTextureReference(), 1);
                spriteShader.setAttribute1fv("texIndex", &texIndices[0], sizeof(float));
                spriteShader.setAttribute1fv("texScale", &texScales[0], sizeof(float));
            }
        } else {
            
            ofDisableAlphaBlending();
            ofDisablePointSprites();
            if ( bLines ){
                burstMesh.setMode(OF_PRIMITIVE_LINES);
                lineShader.begin();
                lineShader.setUniform1f("thickness", lineWidth);
            } else {
                circleShader.begin();
                circleShader.setAttribute1fv("circleScale", &circleScales[0], sizeof(float));
                burstMesh.setMode(OF_PRIMITIVE_POINTS);
//                glPointSize(pointSize/2.0);
            }
        }
        
        ofScale(2.0, 2.0);
        burstMesh.draw();
        if ( bUseShader ){
            if ( mode == 1 ){
                shader.end();
            } else if ( mode == 2 ){
                spriteShader.end();
                ofDisablePointSprites();
            }
        } else {
            if ( bLines ){
                lineShader.end();
            } else {
                circleShader.end();
            }
        }
        render.end();
        ofEnableAlphaBlending();
        render.draw(0,0, ofGetWidth(), ofGetHeight());
        ofPopMatrix();
    }
    
    ofEnableAlphaBlending();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0, ofGetHeight() * .65);
    ofTranslate(-charles.width/2.0, -charles.height/2.0);
    charles.draw(0,0);
    
    ofPopMatrix();
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::onGui( ofxUIEventArgs & e ){
    
}

//--------------------------------------------------------------
void ofApp::randomize(){
    bUseShader = floor(ofRandom(2));
    lineWidth = ofRandom(0.5, 5.0);
    texIndices.clear();
    texScales.clear();
    circleScales.clear();
    modes.clear();
    burstMesh.clear();
    // setup burst
    ofVec2f source(ofGetWidth()/2.0, ofGetHeight() * .85);
    ofVec2f endPoint(ofGetWidth()/2.0 + ofRandom(-300,300), 0);
    
    float start = ofRandom(30, 80); float end = ofRandom(100, 180);
    float num = ofRandom(3, 1000);
    float inc = (end - start) / num;
    float rad = 1000;
    
    bool bParallel = ofRandom(100) > 60 ? false : true;
    bLines = false;
    
    float maxPts = bParallel ? ofRandom(5, 20) : ofRandom(5, 100);
    
    pointSize = ofMap(maxPts, 5, 100, 20, 5.0);
    
    color.setSaturation(.5);
    color.setBrightness(.9);
    color.setHue(ofRandomuf());
    
    center.setSaturation(.5);
    center.setBrightness(.9);
    center.setHue(ofRandomuf());
    
    ofBackground(255);//center * .1);
    
    float density = ofRandom(0, 99);
    intensity = ofRandom(0, path.getWidth() * 2.0);
    mag = ofRandom(0, 500);
    
    int index = 0;
    
    bLines = ofRandom(100) > 60 ? false : true;
    moveType = floor( ofRandom(0, 3));
    
    if ( bParallel ){
        if (!bLines) maxPts *= ofGetHeight()/(pointSize * 20);
        start = 0;
        end = num = 75/lineWidth;
        inc = 1;
        bUseShader = ofRandom(100) > 80 ? true : false;
        for ( float a = start; a<end; a += inc){
            float liveRad = 0;
            for ( int i=0; i<maxPts; i++){
                ofFloatColor c(color);
                liveRad = ofMap(i, 0, maxPts, 1.0, rad);
                float mult = 1.0;
                if ( a > start + (end-start)/2.0){
                    mult = ofMap(a, start + (end-start)/2.0, end, 1.3, 1.0);
                } else {
                    mult = ofMap(a, start, start + (end-start)/2.0, 1.0, 1.3);
                }
                
                //        burstMesh.addVertex(source);
                ofVec3f v;
                v.x = ofMap(i, 0, maxPts, source.x, endPoint.x);
                v.x += ofMap(a, start, end, -(lineWidth * (num) * 2.0), (lineWidth * (num) * 2.0));
                v.y = ofMap(i, 0, maxPts, source.y, endPoint.y);
                
                c.lerp(center, ofMap(abs(v.distance(source)), 0, ofGetWidth()/2.0, 0.0, 1.0));
                //c.setHue(c.getHue() + ofSignedNoise(v.x + i ) * .75);
                c.a = bUseShader ? ofNoise( -sin( ofDegToRad(a)) ) : 1.0;
                //            c.a = .2;
                
                if ( true ){
                    burstMesh.addVertex(v);
                    texIndices.push_back(floor(ofRandom(0,numSprites)));
                    texScales.push_back(ofRandom(.5,maxPts));
                    circleScales.push_back(ofRandom(0.0,1.0));
                    burstMesh.addColor(c);
                    if ( i > 0 && v.distance(source) > 150 && ofRandom(100) > density ){
                        burstMesh.addIndex(index-1);
                        burstMesh.addIndex(index);
                    }
                    /*burstMesh.addTexCoord(ofVec2f(ofMap(v.x, 0, charles.width * 2.0, 0, charles.width),
                     ofMap(v.y, 0, charles.height * 1.5, 0, charles.height)));*/
                    modes.push_back(ofRandom(0,4));
                    index++;
                }
            }
        }
        
    } else {
        
        float maxScale = ofRandom(5,140.);
        
        for ( float a = start; a<end; a += inc){
            float liveRad = 0;
            for ( int i=0; i<maxPts; i++){
                ofFloatColor c(color);
                liveRad = ofMap(i, 0, maxPts, 1.0, rad);
                float mult = 1.0;
                if ( a > start + (end-start)/2.0){
                    mult = ofMap(a, start + (end-start)/2.0, end, 1.3, 1.0);
                } else {
                    mult = ofMap(a, start, start + (end-start)/2.0, 1.0, 1.3);
                }
                
                //        burstMesh.addVertex(source);
                ofVec3f v(source);
                v.x += cos( ofDegToRad(a)) * liveRad * mult;
                v.y += -sin( ofDegToRad(a)) * liveRad * mult;
//                switch (type) {
//                    case 0:
//                        v.x += ofSignedNoise(v.x/(intensity)) * mag;
//                        break;
//                        
//                    case 1:
//                        v.x += ofSignedNoise(v.y/(intensity)) * mag;
//                        v.y += ofSignedNoise(v.y/(intensity)) * mag;
//                        break;
//                    
//                    case 2:
//                        v.y += ofRandom(v.x/(intensity)) * mag;
//                        break;
//                }
                
                c.lerp(center, ofMap(abs(v.distance(source)), 0, ofGetWidth()/2.0, 0.0, 1.0));
                c.setHue(c.getHue() + ofSignedNoise(v.x + i ) * .75);
                c.a = bUseShader ? ofNoise( -sin( ofDegToRad(a)) ) : 1.0;
                //            c.a = .2;
                
                if ( ofRandom(100) > density ){
                    burstMesh.addVertex(v);
                    texIndices.push_back(floor(ofRandom(0,numSprites)));
                    texScales.push_back(ofRandom(.5,maxScale));
                    circleScales.push_back(ofRandom(0.0,1.0));
                    burstMesh.addColor(c);
                    if ( i > 0 && ofRandom(100) > density && fabs( v.distance(source) ) > 300 ){
                        burstMesh.addIndex(index-1);
                        burstMesh.addIndex(index);
                    }
                    /*burstMesh.addTexCoord(ofVec2f(ofMap(v.x, 0, charles.width * 2.0, 0, charles.width),
                     ofMap(v.y, 0, charles.height * 1.5, 0, charles.height)));*/
                    modes.push_back(ofRandom(0,4));
                    index++;
                }
            }
        }

    }
    
    burstMesh.setMode(OF_PRIMITIVE_LINES);
//    burstMesh.setMode(OF_PRIMITIVE_POINTS);
    burstSource = ofMesh(burstMesh);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'r' ) bNeedToRandomize = true;
    if ( key == 's' ) bUseShader = !bUseShader;
    if ( key =='t'){
        bReloadShader = true;
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

}
