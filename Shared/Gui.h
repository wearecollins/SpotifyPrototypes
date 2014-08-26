//  CollinsGUI.h

#pragma once

#include "ofMain.h"
#include "ColorManager.h"

namespace collins {
    
#define GUI_WIDTH 400
#define GUI_HEIGHT 35
#define GUI_FONT_SIZE 10
#define GUI_PADDING 10
#define CONTRAST_MAX 4.0
    
    class Button : public ofRectangle {
    public:
        
        Button();
        void setup( string fontLoc, string t = "Save", float scale = 1.0, int fontSize = GUI_FONT_SIZE );
        void draw(ofEventArgs & e);
        void mouseMoved(ofMouseEventArgs & m );
        void mousePressed(ofMouseEventArgs & m);
        void mouseReleased(ofMouseEventArgs & m);
        
        ofEvent<bool> onPressed;
        
        bool enabled;
        
        ofColor backgroundFill;
        
    protected:
        float padding;
        string text;
        bool bInvert;
        ofTrueTypeFont font;
    };
    
    class Gui : public ofVec2f {
    public:
        
        Gui();
        void setup( string fontLoc, float _scale = 1.0, int fontSize = GUI_FONT_SIZE );
        void update( ofEventArgs & e );
        void draw( ofEventArgs & e );
        
        void attachColor( string name, ofColor * color );
        void attachFloat( string name, float * val );
        
        void min();
        void max();
        int getWidth();
        int getHeight();
        void randomize();
        
        vector<ofColor> getActive();
        
        void mouseMoved(ofMouseEventArgs & m );
        void mouseDragged(ofMouseEventArgs & m);
        void mousePressed(ofMouseEventArgs & m);
        
        void setTitle( string t );
        
        ColorManager & getColorBlack();
        ColorManager & getColorWhite();
        
        bool isFrameNew();
        
        ofEvent<void> doRandomize;
        
    protected:
        bool                    bMinimized, bMouseOver, bFrameNew;
        ColorManager            colorManagerB, colorManagerW;
        
        // randomize
        Button                  randomizeButton;
        void notifyRandomize( bool & b );
        
        // contrast stuff
        ofRectangle             contrastRect, minRect;
        
        ofTrueTypeFont font;
        std::map<string, ofColor *>  colors;
        std::map<string, float *>    floatValues;
        
        ofColor rectColor;
        
        // retina
        float scale;
        
        // names
        string title, color1, color2;
        float height;
        
        // layout vars
        ofColor                 borderColor;
        float                   rectHeight, rectWidth, padding;
    };
}

