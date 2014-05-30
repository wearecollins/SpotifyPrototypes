//
//  Bubbles.h
//  Bubbles
//
//  Created by Brett Renfer on 5/30/14.
//
//

#pragma once

#include "ofxLabFlexParticleSystem.h"

using namespace lab;

class SParticle : public Particle {
public:
    SParticle( float x, float y ) :
    Particle(x,y){
        bCreated = false;
        bTransparent = false;
        bLocked = false;
        setDefaults();
        set(x,y);
        pos.set(x,y);
        res = 100;
        noiseFactor = 1.0;
        complexity = 1.0;
    }
    
    void draw(){
        mesh.clear();
        if ( bLocked ) set(pos);
        mesh = rc::ofMeshCircle::circle(radius * 1.3 * (2.0 - 1.0 * complexity) , x, y, res * complexity );
        
        for ( auto & i : mesh.getIndices()){
            ofVec3f v = mesh.getVertex(i);
            v.x += ofSignedNoise( (v.x + v.y) * .01 + ofGetElapsedTimeMillis() * .002) * (radius * noiseFactor);
            v.y += ofSignedNoise( (v.x + v.y) * .02 + ofGetElapsedTimeMillis() * .001) * (radius * noiseFactor);
            mesh.setVertex(i, v);
            mesh.setTexCoord(i,v);
        }
        
        ofPushMatrix();
        ofTranslate(0,0,z);
        if ( !bTransparent ){
            mesh.draw();
        }
        ofPopMatrix();
    }
    bool bTransparent, bLocked;
    
    int res;
    
    float complexity;
    
    float noiseFactor;
    
protected:
    ofVec3f pos;
    ofMesh mesh;
    bool bCreated;
};

/** @class */
class Bubbles {
public:
    
    Bubbles() :
    bAnimate(true),
    zRange(-5),
    lastdensity(1.0), density(1.0),
    noiseFactor(0.0),
    complexity(1.0)
    {
    }
    
    void setup(){
        resize(ofGetWidth(), ofGetHeight());
        particles.setOption(ParticleSystem::HORIZONTAL_WRAP, false);
        particles.setOption(ParticleSystem::VERTICAL_WRAP, false);
        particles.setOption(ParticleSystem::DETECT_COLLISIONS, true);
        
        ofAddListener( ofEvents().update, this, &Bubbles::update );
        burst();
    }
    
    void update( ofEventArgs & e ){
        ParticleSystem::const_Iterator it = particles.getParticles()->begin();
        ParticleSystem::const_Iterator it2 = particles.getParticles()->begin();
        
        ofVec2f pcenter = particles.getWorldBox();
        
        ofVec3f center( pcenter.x/2.0, pcenter.y/2.0, fabs(sin( ofGetElapsedTimeMillis() * .0001 ))* zRange);
        
        if ( lastdensity != density ){
            lastdensity = density;
            int count = particles.getNumParticles() * density;
            int i = 0;
            for( it; it != particles.getParticles()->end(); ++it )
            {
                bool bL = ((SParticle*) it->second)->bLocked;
                if ( i < count && !bL ){
                    ((SParticle*) it->second)->bTransparent = false;
                } else {
                    ((SParticle*) it->second)->bTransparent = true;
                }
                if ( !bL) i++;
            }
        }
        
        for( it; it != particles.getParticles()->end(); ++it )
        {
            ((SParticle*) it->second)->noiseFactor = noiseFactor;
            ((SParticle*) it->second)->complexity = complexity;
            
            it->second->attract(center);
            for( it2 = particles.getParticles()->begin(); it2 != particles.getParticles()->end(); ++it2 )
            {
                it->second->repel(*it2->second);
            }
        }
        if ( bAnimate) particles.update();
    }
    
    void draw(){
        particles.draw();
    }
    
    void burst(){
        clear();
        int num = ofRandom(30,100);
        ofVec2f pcenter = particles.getWorldBox();
        pcenter.x /= 2.0;
        pcenter.y /= 2.0;
        for ( int i=0; i<num; i++){
            addParticle( ofRandom(pcenter.x - 50, pcenter.x + 50),
                         ofRandom(pcenter.y - 50, pcenter.y + 50) );
        }
    }
    
    void clear(){
        particles.clear();
    }
    
    void resize( int width, int height ){
        particles.setupSquare(ofVec2f(width, height));
    }
    
    SParticle * addParticle( float x, float y ){
        SParticle * p = new SParticle(x,y);
        p->z = ofRandom(10,2);
        int ran = ofRandom(100);
        p->radius = ran > 90 ? ofRandom(10,300) : ofRandom(3,50);
        p->mass = ran < 90 ? p->radius : p->radius * .25;
        p->damping = .9;
        particles.addParticle(p);
        if ( ofGetKeyPressed( OF_KEY_SHIFT) ){
            p->bLocked = true;
            p->bTransparent = true;
            p->radius = 100;
        } else {
            p->res = ofRandom(3,50);
        }
        return p;
    }
    
    bool bAnimate;
    float zRange;
    float density, lastdensity;
    float noiseFactor;
    float complexity;
    
protected:
    
    // particles
    ParticleSystem particles;
    
};
