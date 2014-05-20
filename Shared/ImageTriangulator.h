//
//  ImageTriangulator.h
//  Streaming_Triangulate
//
//  Created by Brett Renfer on 5/20/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxDelaunay.h"

class ImageTriangulator : public ofThread {
public:
    
    ImageTriangulator() :
    threshold(.05){
        
    }
    
    void process( ofImage & image ){
        toProcess.setFromPixels(image.getPixelsRef());
        toProcess.setUseTexture(false);
        bNeedToProcess = true;
        if (!isThreadRunning()) startThread();
    }
    
    virtual void reprocess(){
        bNeedToProcess = true;
    }
    
    void draw(){
        lock();
        mesh.draw();
        unlock();
    }
    
    virtual void threadedFunction(){
        while(isThreadRunning()){
            if ( bNeedToProcess ) triangulate(toProcess);
            bNeedToProcess = false;
            sleep(100);
        }
    }
    
    float threshold;
    
protected:
    
    ofImage toProcess;
    ofxDelaunay triangulator;
    ofMesh mesh;
    bool bNeedToProcess = true;
    
    void triangulate( ofImage & image ){
        triangulator.reset();
        vector<ofPoint> points = findPoints( image, threshold / 1000 );
        //triangulator.addPoints( points );
        // is this still an issue?
        triangulator.addPoint(0,0,0);
        triangulator.addPoint(image.width,0,0);
        triangulator.addPoint(image.width,image.height,0);
        triangulator.addPoint(0,image.height,0);
        for ( auto p : points ){
            // why do we have to do this ? :(
            float x = ofRandom(p.x,p.x+.1), y =  ofRandom(p.y,p.y+.1);
            triangulator.addPoint(x,y, 0 );
        }
        triangulator.triangulate();
        
        mesh.clear();
        
        ofColor white;
        for ( auto v : triangulator.triangleMesh.getVertices() ){
            triangulator.triangleMesh.addColor(white);
        }
        
        // set colors
        for ( int i=0, len = triangulator.triangleMesh.getNumIndices(); i<len; i+=3){
            ofIndexType i1 = triangulator.triangleMesh.getIndex( (ofIndexType) i);
            ofIndexType i2 = triangulator.triangleMesh.getIndex( (ofIndexType) i+1);
            ofIndexType i3 = triangulator.triangleMesh.getIndex( (ofIndexType) i+2);
            
            ofPoint p1 = triangulator.triangleMesh.getVertex( i1 );
            ofPoint p2 = triangulator.triangleMesh.getVertex( i2 );
            ofPoint p3 = triangulator.triangleMesh.getVertex( i3 );
            
            ofPoint center;
            center.x = (p1.x + p2.x + p3.x )/3.0;
            center.y = (p1.y + p2.y + p3.y )/3.0;
            
            ofColor c = image.getColor(round(center.x), round(center.y));
            //        c.a = 200;
            ofFloatColor toSet; toSet.set( c.r/255.0f, c.g/255.0f, c.b/255.0f, 1.0f);
            triangulator.triangleMesh.setColor(i1, toSet);
            triangulator.triangleMesh.setColor(i2, toSet);
            triangulator.triangleMesh.setColor(i3, toSet);
            
            mesh.addIndex(mesh.getNumVertices());
            mesh.addVertex(p1);
            mesh.addColor(toSet);
            mesh.addIndex(mesh.getNumVertices());
            mesh.addVertex(p2);
            mesh.addColor(toSet);
            mesh.addIndex(mesh.getNumVertices());
            mesh.addVertex(p3);
            mesh.addColor(toSet);
        }
    }
    
    vector<ofPoint> findPoints( ofImage & image, float threshold ){
        cv::Mat toProcess = ofxCv::toCv(image);
        vector<cv::KeyPoint> objectKeypoints;
        vector<ofPoint> points;
        
        // Extract key points
        //    The detector can be any of (see OpenCV features2d.hpp):
        //    ﻿cv::FeatureDetector * detector = new cv::DenseFeatureDetector();
        //    ﻿cv::FeatureDetector * detector = new cv::FastFeatureDetector();
        //    ﻿cv::FeatureDetector * detector = new cv::GFTTDetector();
        //    cv::FeatureDetector * detector = new cv::MSER();
        //    ﻿cv::FeatureDetector * detector = new cv::ORB();
        //    ﻿cv::FeatureDetector * detector = new cv::StarFeatureDetector();
        //    ﻿cv::FeatureDetector * detector = new cv::SURF(600.0);
        const cv::SIFT::DetectorParams& detectorParams = cv::SIFT::DetectorParams(threshold, 10.0);
        
        cv::FeatureDetector * detector = new cv::SiftFeatureDetector( detectorParams );
        detector->detect(toProcess, objectKeypoints);
        delete detector;
        
        // convert to ofVec2f vector
        for ( int i=0, len = objectKeypoints.size(); i<len; i++){
            points.push_back( ofVec2f( objectKeypoints[i].pt.x, objectKeypoints[i].pt.y ) );
        }
        return points;
    }
    
};