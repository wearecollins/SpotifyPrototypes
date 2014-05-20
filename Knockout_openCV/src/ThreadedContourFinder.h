//
//  ThreadedContourFinder.h
//  Knockout_openCV
//
//  Created by Brett Renfer on 5/19/14.
//
//

#ifndef Knockout_openCV_ThreadedContourFinder_h
#define Knockout_openCV_ThreadedContourFinder_h

#include "ofMain.h"
#include "ofxCv.h"

class ThreadedContourFinder : public ofThread {
public:
    
    ThreadedContourFinder() :
    minArea(0),
    maxArea(1920 * 1080),
    threshold(150),
    polylineSimplify(100),
    contrast(1.0),
    bInvert(true)
    {
    }
    
    ~ThreadedContourFinder(){
        waitForThread();
        stopThread();
    }
    
    void setup(){
        bForceFind = false;
        threshold = ofRandom(50,200);
        polylineSimplify = ofRandom(5,200);
        contrast = ofRandom(.9,3.0);
        startThread();
    }
    
    template <class T>
    void loadPixels( T & hasPixels ){
        cvmat = ofxCv::toCv(hasPixels);
        bForceFind = true;
    }
    
    void threadedFunction(){
        while (isThreadRunning()){
            findContours();
            yield();
        }
    }
    
    void findContours(){
        if ( curMinArea != minArea ||
            curMaxArea != maxArea || curThreshold != threshold || curContrast != contrast){
            bForceFind = true;
        }
        
        curMinArea      = minArea;
        curMaxArea      = maxArea;
        curThreshold    = threshold;
        curContrast     = contrast;
        
        finder.setFindHoles(true);
        finder.setAutoThreshold(true);
        finder.setMinArea(curMinArea);
        finder.setMaxArea(curMaxArea);
        finder.setThreshold(curThreshold);
        
        if ( !cvmat.empty() && bForceFind)
        {
            
            cvmat.convertTo(cvmattrack, -1,curContrast,0);
            finder.setInvert(bInvert);
            finder.findContours(cvmattrack);
        }
        
        if ( true || bForceFind || curSimplify != polylineSimplify ){
            curSimplify     = polylineSimplify;
            vector<ofPolyline> poly = finder.getPolylines();
        
            bufferMesh.clear();
            polyMeshes.clear();
            
            for ( auto & p : poly ){
                ofPolyline smoothed = p.getSmoothed(polylineSimplify);
                ofMesh m;
                tess.tessellateToMesh( smoothed, OF_POLY_WINDING_ODD, m);
                polyMeshes.push_back(m);
                bufferMesh.append(m);
            }
            
            for ( int i=0; i<bufferMesh.getNumVertices(); i++){
                ofVec2f tex(bufferMesh.getVertex(i).x,bufferMesh.getVertex(i).y);
                tex.x += ofSignedNoise((ofGetElapsedTimeMillis() *.001 + tex.x) * .01)*100.0;
                tex.y += ofSignedNoise((ofGetElapsedTimeMillis() *.002 + tex.y) * .02)*100.0;
                bufferMesh.addTexCoord( tex );
            }
            
            while(! lock() ){
            }
            swap(bufferMesh, mesh);
            unlock();
        }
            
        bForceFind = false;
    }
    
    ofMesh & getMesh(){
        return mesh;
    }
    
    float minArea, maxArea, threshold, polylineSimplify, contrast;
    
    bool bInvert;
    
protected:
    float curMinArea, curMaxArea, curThreshold, curSimplify, curContrast;
    ofxCv::ContourFinder finder;
    cv::Mat cvmat, cvmattrack;
    
    vector<ofMesh> polyMeshes;
    ofMesh mesh, bufferMesh;
    bool bForceFind;
    ofTessellator tess;
};

#endif
