//
//  SongToMesh.h
//  SongToWaves
//
//  Created by Brett Renfer on 5/26/14.
//
//

#pragma once

#include "FMODProcessor.h"

class SongMesh {
public:
    
    void load( string filename, int numpoints = 10000 ){
        clear();
        
        ofFile file = ofFile( ofToDataPath(filename, true) );
        
        string dir      = file.getEnclosingDirectory();
        string meshname = file.getBaseName();
        string meshFile = dir + "/" + meshname + "_" + ofToString(numpoints);
        
        bool   bMeshExists = false;
        
        cout << meshFile << endl;
        
        if ( ofFile(meshFile).exists() ){
            bMeshExists = true;
            mesh.load(meshFile);
        } else {
            sound.loadSoundAsSample(ofToDataPath(filename, true));
        }
        
        if ( !bMeshExists ){
            mesh.setMode(OF_PRIMITIVE_LINES);
            
            int index = 0;
            float * data = new float[sound.getLengthInBytes()];
            int len = sound.getSpectrumAtPercentage(data, 0.0, 1.0);
            
            // just left, is that weird?
            int res = 2;
            
            for (int j = 0; j < len; j += res){
                float x = ofMap(j, 0, len / 2.0, 0, 1.0);
                float y = data[j];
                if ( y > 0 ){
                    top.addVertex(x, y);
                } else {
                    bottom.addVertex(x, y);
                }
            }
            
            delete data;
            
            // save out high res
            ofPolyline smoothedTop = top.getResampledByCount(numpoints).getSmoothed(0.0);
            ofPolyline smoothedBottom = bottom.getResampledByCount(numpoints).getSmoothed(0.0);
            
            mesh.clear();
            vector<ofPolyline> smoothed;
            smoothed.push_back(smoothedTop);
            smoothed.push_back(smoothedBottom);
            tess.tessellateToMesh(smoothed, OF_POLY_WINDING_ODD, mesh);
            
            mesh.save(meshFile);
        } else {
            for (auto & v : mesh.getVertices() ){
                if ( v.y > 0 ){
                    top.addVertex(v);
                } else {
                    bottom.addVertex(v);
                }
            }
        }
    }
    
    void smooth( float smooth, float points = 10000){
        ofPolyline smoothedTop = top.getResampledByCount(points).getSmoothed(smooth);
        ofPolyline smoothedBottom = bottom.getResampledByCount(points).getSmoothed(smooth);
        
        mesh.clear();
        vector<ofPolyline> smoothed;
        smoothed.push_back(smoothedTop);
        smoothed.push_back(smoothedBottom);
        tess.tessellateToMesh(smoothed, OF_POLY_WINDING_ODD, mesh);
    }
    
    void draw(float x = 0, float y = 0, float width = -1, float height = -1 ){
        ofPushMatrix();
        ofTranslate(x, y);
        ofScale(width == -1 ? ofGetWidth() : width, height == -1 ? ofGetHeight()/2.0 : height);
        mesh.draw();
        ofPopMatrix();
    }
    
    void clear(){
        top.clear();
        bottom.clear();
        mesh.clear();
        sound.unloadSound();
    }
    
protected:
    
    
    ofPolyline top, bottom;
    ofTessellator tess;
    ofMesh mesh;
    FMODProcessor sound;
};
