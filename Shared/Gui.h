//  CollinsGUI.h

#pragma once

#include "ofMain.h"
#include "ColorManager.h"

namespace collins {
    
#define GUI_WIDTH 400
#define GUI_HEIGHT 35
#define GUI_FONT_SIZE 10
#define GUI_PADDING 10
    
    class Gui : public ofVec2f {
    public:
        
        //--------------------------------------------------------------
        Gui():
        bMinimized(false),
        rectHeight(GUI_HEIGHT),
        rectWidth(GUI_WIDTH),
        padding(GUI_PADDING),
        title("(No Image)"),
        color1("Black"),
        color2("White"),
        borderColor(255),
        scale(1.0){
        }
        //--------------------------------------------------------------
        
        void setup( string fontLoc, float _scale = 1.0, int fontSize = GUI_FONT_SIZE ){
            ofAddListener(ofEvents().update, this, &Gui::update);
            ofAddListener(ofEvents().draw, this, &Gui::draw);
            ofAddListener(ofEvents().mouseMoved, this, &Gui::mouseMoved);
            ofAddListener(ofEvents().mouseDragged, this, &Gui::mouseDragged);
            ofAddListener(ofEvents().mousePressed, this, &Gui::mousePressed);
            scale = _scale;
            
            rectWidth *= scale;
            rectHeight *= scale;
            padding *= scale;
            
            font.loadFont( fontLoc, fontSize * (scale == 1.0 ? 1.0 : scale * 1.1));
            colorManagerW.setup("assets/colors.xml", scale);
            colorManagerB.setup("assets/colors.xml", scale);
            colorManagerW.x = 0;
            colorManagerB.x = rectWidth/2.0;
            colorManagerW.y = rectHeight * 1.75;
            colorManagerB.y = rectHeight * 1.75;
            height = 600;
            
            minRect.set(rectWidth-padding-(rectHeight-padding*2.0),padding, (rectHeight-padding*2.0), (rectHeight-padding*2.0));
        }
        
        //--------------------------------------------------------------
        void update( ofEventArgs & e ){
            
        }
        
        //--------------------------------------------------------------
        void draw( ofEventArgs & e ){
            ofPushMatrix();
            ofPushStyle();
            
            ofTranslate(*this);
            ofSetColor(150,100);
            ofRect(0,0, rectWidth, height);
            ofNoFill();
            ofSetColor(borderColor);
            ofSetLineWidth(2);
            
            // title
            float x = 0;
            float y = 0;
            ofRect(x, y, rectWidth, rectHeight);
            
            ofLine(minRect.x, minRect.y + minRect.height/2.0, minRect.x + minRect.width, minRect.y + minRect.height/2.0);
            if ( bMinimized ){
                ofLine(minRect.x+minRect.width/2.0, minRect.y, minRect.x+minRect.width/2.0, minRect.y + minRect.height);
            }
            
            font.drawString( title, x + padding, y + font.getSize() + padding * 1.25);
            y += rectHeight;
            
            // colors
            color1 = colorManagerW.getActiveName() == "" ? "Black" : colorManagerW.getActiveName();
            color2 = colorManagerB.getActiveName() == "" ? "White" : colorManagerB.getActiveName();
            
            if ( !bMinimized ){
                font.drawString( "a. "+color1, x + padding, y + font.getSize() + padding * 1.25);
                font.drawString( "b. "+color2, x + rectWidth/2.0 + padding, y + font.getSize() + padding * 1.25);
                ofRect(0, y, rectWidth/2.0, rectHeight * 5.0);
                ofRect(rectWidth/2.0, y, rectWidth/2.0, rectHeight * 5.0);
                y += rectHeight * 5.0;
                colorManagerB.draw();
                colorManagerW.draw();
                contrastRect.set(padding, y + rectHeight, rectWidth - padding * 2, rectHeight);
                
                // contrast
                ofRect(x, y, rectWidth, rectHeight * 2.0);
                font.drawString( "Contrast", x + padding, y + font.getSize() + padding * 1.25);
                font.drawString( ofToString(*floatValues["contrast"], 1), x + rectWidth/2.0 + padding, y + font.getSize() + padding * 1.25);
                y += rectHeight;
                
                // draw line
                ofLine(x + padding, y + padding, rectWidth - padding * 2, y + padding);
                
                float xMark = ofMap(*floatValues["contrast"], 1.0, 5.0, x + padding, rectWidth - padding * 2);
                
                ofLine(xMark, y + padding - 8, xMark, y + padding + 8);
                y += rectHeight;
            } else {
                font.drawString( "a. "+color1, x + padding, y + font.getSize() + padding * 1.25);
                font.drawString( "b. "+color2, x + rectWidth/2.0 + padding, y + font.getSize() + padding * 1.25);
                ofRect(0, y, rectWidth/2.0, rectHeight);
                ofRect(rectWidth/2.0, y, rectWidth/2.0, rectHeight);
                y += rectHeight;
                contrastRect.set(padding, y + rectHeight, rectWidth - padding * 2, rectHeight);
                
                // contrast
                ofRect(x, y, rectWidth, rectHeight);
                font.drawString( "Contrast", x + padding, y + font.getSize() + padding * 1.25);
                font.drawString( ofToString(*floatValues["contrast"], 1), x + rectWidth/2.0 + padding, y + font.getSize() + padding * 1.25);
                y += rectHeight;
            }
            
            height = y;
            
            ofPopMatrix();
            ofPopStyle();
        }
        
        //--------------------------------------------------------------
        void attachColor( string name, ofColor * color ){
            colors[name] = color;
        }
        
        //--------------------------------------------------------------
        void attachFloat( string name, float * val ){
            floatValues[name] = val;
        }
        
        //--------------------------------------------------------------
        void min(){
            bMinimized = true;
        }
        
        //--------------------------------------------------------------
        void max(){
            bMinimized = false;
        }
        
        //--------------------------------------------------------------
        int getWidth(){
            return rectWidth;
        }
        
        //--------------------------------------------------------------
        int getHeight(){
            return height;
        }
        
        //--------------------------------------------------------------
        void randomize(){
            colorManagerW.randomize();
            colorManagerB.randomize();
        }
        
        //--------------------------------------------------------------
        vector<ofColor> getActive(){
            static vector<ofColor> ret; ret.clear();
            if ( colorManagerW.getActive() == ofColor(0,0))
                ret.push_back(ofColor(0));
            else ret.push_back(colorManagerW.getActive());
            if ( colorManagerB.getActive() == ofColor(0,0))
                ret.push_back(ofColor(255));
            else ret.push_back(colorManagerB.getActive());
            return ret;
        }
        
        //--------------------------------------------------------------
        void mouseMoved(ofMouseEventArgs & m ){
            colorManagerW.checkHit(m.x - x, m.y - y, false, false);
            colorManagerB.checkHit(m.x - x, m.y - y, false, false);
        }
        
        //--------------------------------------------------------------
        void mouseDragged(ofMouseEventArgs & m){
            if ( contrastRect.inside(m.x-x, m.y-y)){
                float fx = (m.x-x) - contrastRect.x;
                *floatValues["contrast"] = ofMap(fx, 0, contrastRect.width, 1.0, 5.0, true);
            }
        }
        
        //--------------------------------------------------------------
        void mousePressed(ofMouseEventArgs & m){
            if (!bMinimized) colorManagerW.checkHit(m.x - x, m.y - y, true, !ofGetKeyPressed(OF_KEY_SHIFT));
            if (!bMinimized) colorManagerB.checkHit(m.x - x, m.y - y, true, !ofGetKeyPressed(OF_KEY_SHIFT));
            
            if ( contrastRect.inside(m.x-x, m.y-y)){
                float fx = (m.x-x) - contrastRect.x;
                *floatValues["contrast"] = ofMap(fx, 0, contrastRect.width, 1.0, 5.0, true);
            } else if ( minRect.inside(m.x-x, m.y-y)){
                bMinimized = !bMinimized;
            }
        }
        
        //--------------------------------------------------------------
        void setTitle( string t ){
            title = t;
            while ( font.stringWidth(title) + padding * 2.0 + 50 > rectWidth ){
                title = title.substr(0,title.size()-1);
            }
        }
        
        //--------------------------------------------------------------
        ColorManager & getColorBlack(){
            return colorManagerB;
        }
        
        //--------------------------------------------------------------
        ColorManager & getColorWhite(){
            return colorManagerW;
        }
        
    protected:
        bool                    bMinimized;
        ColorManager            colorManagerB, colorManagerW;
        
        // contrast stuff
        ofRectangle             contrastRect, minRect;
        
        ofTrueTypeFont font;
        std::map<string, ofColor *>  colors;
        std::map<string, float *>    floatValues;
        
        // retina
        float scale;
        
        // names
        string title, color1, color2;
        float height;
        
        // layout vars
        ofColor                 borderColor;
        float                   rectHeight, rectWidth, padding;
    };
    
    class Button : public ofRectangle {
    public:
        
        void setup( string fontLoc, string t = "Save", float scale = 1.0, int fontSize = GUI_FONT_SIZE ){
            text = t;
            bInvert = false;
            width = (GUI_WIDTH * scale) / 2.0;
            height = GUI_HEIGHT * scale;
            padding = GUI_PADDING * scale;
            font.loadFont( fontLoc, fontSize * (scale == 1.0 ? 1.0 : scale * 1.1) );
            ofAddListener(ofEvents().draw, this, &Button::draw);
            ofAddListener(ofEvents().mouseMoved, this, &Button::mouseMoved);
            ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);
            ofAddListener(ofEvents().mouseReleased, this, &Button::mouseReleased);
        }
        
        //--------------------------------------------------------------
        void draw(ofEventArgs & e){
            ofPushStyle();
            ofFill();
            ofSetColor(150,100);
            ofRect(*this);
            
            ofSetColor(255);
            
            if ( !bInvert ) ofNoFill();
            else ofFill();
            ofSetLineWidth(2.0);
            ofRect(*this);
            
            if ( bInvert ) ofSetColor(150.0);
            font.drawString(text, x+ padding, y + font.getSize() + padding );
            ofPopStyle();
        }
        
        //--------------------------------------------------------------
        void mouseMoved(ofMouseEventArgs & m ){
            if ( inside(m.x, m.y)){
                bInvert = true;
            } else {
                bInvert = false;
            }
        }

        //--------------------------------------------------------------
        void mousePressed(ofMouseEventArgs & m){
            if ( inside(m.x, m.y)){
                bInvert = true;
            }
        }
        
        //--------------------------------------------------------------
        void mouseReleased(ofMouseEventArgs & m){
            bInvert = false;
            if ( inside(m.x, m.y)){
                static bool b = true;
                ofNotifyEvent(onPressed, b, this);
            }
        }
        
        ofEvent<bool> onPressed;
        
    protected:
        float padding;
        string text;
        bool bInvert;
        ofTrueTypeFont font;
    };
}

