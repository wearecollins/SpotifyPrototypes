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
#include "CvFilters.h"

class ThreadedContourFinder : public ofThread {
public:
    
    ThreadedContourFinder() :
    minArea(0), maxArea(1920 * 1080), threshold(150),
    polylineSimplify(100),
    contrast(1.0),
    bInvert(true), bDoWarp(false), bEdge(true), bMapImage(false),
    windingMode(OF_POLY_WINDING_POSITIVE),
    scale(1.0), curScale(1.0),
    bTint(true),
    cvScale(.25)
    {
        filters.push_back( new FilterGaussian() );
        filters.push_back( new FilterLaplace() );
        filters.push_back( new FilterDilate() );
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
        finder.setTargetColor(ofColor(255));
        startThread();
    }
    
    void update(){
        lock();
        if ( !cvmattrack.empty() ){
            ofxCv::toOf(cvmattrack, drawImage);
            drawImage.update();
        } else {
        }
        unlock();
    }
    
    void draw( float x, float y, float wScale = -1 ){
        if ( drawImage.isAllocated() ){
            float w = drawImage.width;
            float h = drawImage.height;
            
            if ( wScale != - 1){
                w = wScale;
                h *= (float) w/drawImage.width;
            }
            
            drawImage.draw(x,y,w,h);
        } else {
        }
    }
    
    template <class T>
    void loadPixels( T & hasPixels ){
        cv::Mat src = ofxCv::toCv(hasPixels);
        ofxCv::resize(src, cvmat, cvScale, cvScale);
        ofxCv::convertColor(cvmat, cvmatbw, cvmat.channels() == 3 ? CV_RGB2GRAY : CV_RGBA2GRAY);
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
            curMaxArea != maxArea ||
            curThreshold != threshold ||
            curContrast != contrast){
            bForceFind = true;
        }
        
        curMinArea      = minArea;
        curMaxArea      = maxArea;
        curThreshold    = threshold;
        curContrast     = contrast;
        
        finder.setFindHoles(true);
        finder.setAutoThreshold(false);
        finder.setMinArea(curMinArea);
        finder.setMaxArea(curMaxArea);
//        finder.setThreshold(curThreshold);
        
        if ( !cvmatbw.empty() && bForceFind)
        {
            cvmatbw.convertTo(cvmattrack, -1,curContrast,0);
            
            for ( auto * filter : filters ){
                if ( filter->bActive ){
                    filter->process(cvmattrack);
                }
            }
            
            ofxCv::threshold(cvmattrack, curThreshold, bInvert);
            
            finder.findContours(cvmattrack);
        }
        
        if ( bForceFind || curSimplify != polylineSimplify || curScale != scale){
            curSimplify     = polylineSimplify;
            curScale = scale;
            vector<ofPolyline> poly = finder.getPolylines();
        
            bufferMesh.clear();
            polyMeshes.clear();
            
            for ( auto & p : poly ){
                ofPolyline smoothed = p.getSmoothed(polylineSimplify);
                
                smoothed.flagHasChanged();
                if ( smoothed.getArea() != 0 ){
                    if ( bEdge ){
                        int i =0;
                        int total = ofRandom(10,1000);
                        for ( auto & p : smoothed.getVertices() ){
                            p.x = 0;
                            i++;
                            if ( i >= total ) break;
                        }
                    }
                    
                    smoothed.flagHasChanged();
                    if ( smoothed.getArea() != 0 ){
                        if ( scale != 1.0 ){
                            smoothed.flagHasChanged();
                            ofPoint center = smoothed.getCentroid2D();
                            for ( auto & p : smoothed.getVertices() ){
                                ofVec2f dir = p - center;
                                p += dir * (scale-1.0);
                            }
                            smoothed.flagHasChanged();
                        }
                    }
                    
                    if ( cvScale != 1.0 ){
                        for ( auto & p : smoothed.getVertices() ){
                            p *= 1.0/cvScale;
                        }
                    }
                    
                    ofMesh m;
                    tess.tessellateToMesh( smoothed, (ofPolyWindingMode) windingMode, m);
                    polyMeshes.push_back(m);
                    bufferMesh.append(m);
                }
            }
            
            if ( bMapImage ){
                for ( int i=0; i<bufferMesh.getNumVertices(); i++){
                    ofVec2f tex(bufferMesh.getVertex(i).x,bufferMesh.getVertex(i).y);
                    if ( bDoWarp ){
                        tex.x += ofSignedNoise((ofGetElapsedTimeMillis() *.001 + tex.x) * .01)*100.0;
                        tex.y += ofSignedNoise((ofGetElapsedTimeMillis() *.002 + tex.y) * .02)*100.0;
                    }
                    bufferMesh.addTexCoord( tex );
                }
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
    
    float cvScale, scale, minArea, maxArea, threshold, polylineSimplify, contrast;
    
    bool bInvert, bDoWarp, bEdge, bMapImage, bTint;
    
    int windingMode;
    vector<CvFilter*> filters;
    
protected:
    float curMinArea, curMaxArea, curThreshold, curSimplify, curContrast, curScale;
    ofxCv::ContourFinder finder;
    cv::Mat cvmat, cvmatbw, cvmattrack;
    
    vector<ofMesh> polyMeshes;
    ofMesh mesh, bufferMesh;
    bool bForceFind;
    ofTessellator tess;
    ofImage drawImage;
};

#endif
