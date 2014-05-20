//
//  RandomTriangulator.h
//  Streaming_Triangulate
//
//  Created by Brett Renfer on 5/20/14.
//
//

#pragma once

#include "ImageTriangulator.h"

class RandomTriangulator : public ImageTriangulator {
public:
    
    float density;
    
    virtual void threadedFunction(){
        while(isThreadRunning()){
            if ( bNeedToProcess ){
                triangulate(toProcess);
                
                for ( auto v : mesh.getVertices() ){
                    mesh.addTexCoord(v);
                }
                
                // randomize mesh
                for ( int i=0, len = mesh.getNumIndices(); i<len; i+=3){
                    ofIndexType i1 = mesh.getIndex( (ofIndexType) i);
                    ofIndexType i2 = mesh.getIndex( (ofIndexType) i+1);
                    ofIndexType i3 = mesh.getIndex( (ofIndexType) i+2);
                    
                    ofFloatColor toSet( mesh.getColor(i1) );
                    //toSet.a = ofNoise( mesh.getVertex(i1).x + ofGetElapsedTimeMillis() * .001);
                    toSet.a = (ofRandom(100) > density ? 1.0 : 0.0);
                    mesh.setColor(i1, toSet);
                    mesh.setColor(i2, toSet);
                    mesh.setColor(i3, toSet);
                    
                    ofVec2f v1 = mesh.getVertex(i1);
                    ofVec2f v2 = mesh.getVertex(i2);
                    ofVec2f v3 = mesh.getVertex(i3);
                    
                    ofVec2f shift;
                    
                    shift.x = ofSignedNoise( v1.x + ofGetElapsedTimeMillis() * .001) * toProcess.width/4.0;
                    shift.y = ofSignedNoise( v1.y + ofGetElapsedTimeMillis() * .001) * toProcess.height/4.0;
                    
                    v1 += shift;
                    v2 += shift;
                    v3 += shift;
                    
                    v1.x = ofClamp(v1.x,0,toProcess.width);
                    v1.y = ofClamp(v1.y,0,toProcess.height);
                    
                    v2.x = ofClamp(v2.x,0,toProcess.width);
                    v2.y = ofClamp(v2.y,0,toProcess.height);
                    
                    v3.x = ofClamp(v3.x,0,toProcess.width);
                    v3.y = ofClamp(v3.y,0,toProcess.height);
                    
                    mesh.setTexCoord(i1,v1);
                    mesh.setTexCoord(i2,v2);
                    mesh.setTexCoord(i3,v3);
                }
            }
            
            bNeedToProcess = false;
            sleep(100);
        }
    }
};