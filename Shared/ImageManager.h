//
//  ImageManager.h
//  Bubbles
//
//  Created by Brett Renfer on 5/28/14.
//
//

#pragma once

#include "ofMain.h"
#include "ImageFilter.h"

class ImageManager {
public:
    
    void setup(){
        toLoad.resize(0);
        images.resize(0);
        imagesFiltered.resize(0);
        ofAddListener(ofEvents().update, this, &ImageManager::update );
        ofAddListener(ofEvents().fileDragEvent, this, &ImageManager::onDrag );
    }
    
    void update( ofEventArgs & e ){
        if ( toLoad.size() > 0 ){
            for ( auto & s : toLoad ){
                if ( s != "" ){
                    images.clear();
                    images.insert(images.begin(), ofImage());
                    images.front().loadImage(s);
                    ofNotifyEvent(onLoaded, images.front(), this);
                    ofFile f(s);
                    string ret = f.getFileName();
                    ofNotifyEvent(onLoadedFile, ret, this);
                }
            }
            runFilters();
            toLoad.clear();
        }
    }
    
    void runFilters(){
        imagesFiltered.clear();
        for ( auto & img : images ){
            imagesFiltered.push_back( ofImage(img) );
            for ( auto * filter : filters ){
                filter->process( imagesFiltered.back() );
            }
        }
    }
    
    void onDrag( ofDragInfo & e ){
        
        for ( auto & s : e.files ){
//            cout <<" FILE NAME " << s << endl;
        }
        toLoad.swap(e.files);
    }
    
    // filters
    void addFilter( ImageFilter * filter ){
        filters.push_back(filter);
    }
    
    // getters
    int size(){
        return images.size();
    }
    
    void clear(){
        images.clear();
    }
    
    void clearFilters(){
        filters.clear();
    }
    
    ofImage & getRawImage( int which ){
        which = which % images.size();
        return images[which];
    }
    
    ofImage & getImage( int which ){
        which = which % imagesFiltered.size();
        return imagesFiltered[which];
    }
    
    void setImage( int which, ofImage & img ){
        which = which % imagesFiltered.size();
        imagesFiltered[which] = img;
    }
    
    ofEvent<ofImage>    onLoaded;
    ofEvent<string>     onLoadedFile;
    
protected:
    vector<string>  toLoad;
    vector<ofImage> images;
    vector<ofImage> imagesFiltered;
    vector<ImageFilter *> filters;
};
