//
//  CvFilters.h
//  Knockout_openCV
//
//  Created by Brett Renfer on 5/21/14.
//
//

#pragma once

#include "ofxCv.h"

class CvFilter {
public:
    
    CvFilter(){
        bActive = false;
        amount = 3.0;
    }
    
    bool bActive;
    
    virtual void process( cv::Mat & mat ) = 0;
    
    float amount;
    
};

class FilterContrast : public CvFilter {
public:
    
    void process( cv::Mat & mat ){
        float amt = amount;
        cv::Mat p;
        mat.convertTo(p, -1,amt,0);
        mat = p;
    }
};

class FilterGaussian : public CvFilter {
public:
    
    void process( cv::Mat & mat ){
        int amt = amount;
        if ( amt % 2 == 0 ) amt += 1;
        if ( !bActive ) return;
        cv::GaussianBlur( mat, mat, cv::Size(amt,amt), 0, 0, cv::BORDER_DEFAULT );
    }
};

class FilterLaplace : public CvFilter {
public:
    
    void process( cv::Mat & mat ){
        if ( !bActive ) return;
        int kernel_size = amount;
        int scale = 1;
        int delta = 0;
        int ddepth = mat.depth();
        cv::Laplacian( mat, mat, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT );
    }
};

class FilterDilate : public CvFilter {
public:
    void process( cv::Mat & mat ){
        if ( !bActive ) return;
        int dilation_type;
        int dilation_elem = 0;
        int dilation_size = amount;
        
        if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
        else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
        else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
        
        cv::Mat element = cv::getStructuringElement( dilation_type,
                                            cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                            cv::Point( dilation_size, dilation_size ) );
        cv::dilate(mat, mat, element);
    }
};