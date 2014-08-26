//
//  Gui.cpp
//  Colorizer
//
//  Created by Brett Renfer on 8/26/14.
//
//

#include "Gui.h"

//  CollinsGUI.h

namespace collins {
    
    //--------------------------------------------------------------
    Gui::Gui():
    bMinimized(false),
    bMouseOver(false),
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
    
    void Gui::setup( string fontLoc, float _scale, int fontSize ){
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
        font.setLetterSpacing(.95);
        font.setSpaceSize(.6);
        colorManagerW.setup("assets/colors.xml", scale);
        colorManagerB.setup("assets/colors.xml", scale);
        colorManagerW.x = 0;
        colorManagerB.x = rectWidth/2.0;
        colorManagerW.y = rectHeight * 1.75;
        colorManagerB.y = rectHeight * 1.75;
        height = 600;
        
        minRect.set(rectWidth-padding-(rectHeight-padding*2.0),padding, (rectHeight-padding*2.0), (rectHeight-padding*2.0));
        
        rectColor.set(180, 0);
        randomizeButton.backgroundFill.set(rectColor);
        
        // setup randomize button
        randomizeButton.setup( fontLoc, "Randomize", _scale );
        randomizeButton.x = x;
        randomizeButton.y = y + height;
        
        ofAddListener(randomizeButton.onPressed, this, &Gui::notifyRandomize );
    }
    
    //--------------------------------------------------------------
    void Gui::update( ofEventArgs & e ){
        if ( bMouseOver ){
            rectColor.a = rectColor.a * .6 + 200 * .4;
        } else {
            rectColor.a = rectColor.a * .6;// + 0 * .1;
        }
        randomizeButton.backgroundFill.set(rectColor);
    }
    
    //--------------------------------------------------------------
    void Gui::draw( ofEventArgs & e ){
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(x,y);
        ofSetColor(rectColor);
        ofRect(0,0, rectWidth, height);
        ofNoFill();
        ofSetColor(borderColor);
        ofSetLineWidth(2);
        
        // randomize button
        randomizeButton.x = x + rectWidth/2.0;
        randomizeButton.y = y + height;
        
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
            font.drawString( "[b] "+color1, x + padding, y + font.getSize() + padding * 1.25);
            font.drawString( "[w] "+color2, x + rectWidth/2.0 + padding, y + font.getSize() + padding * 1.25);
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
            
            float xMark = ofMap(*floatValues["contrast"], 1.0, CONTRAST_MAX, x + padding, rectWidth - padding * 2);
            
            ofLine(xMark, y + padding - 8, xMark, y + padding + 8);
            y += rectHeight;
        } else {
            font.drawString( "[b] "+color1, x + padding, y + font.getSize() + padding * 1.25);
            font.drawString( "[w] "+color2, x + rectWidth/2.0 + padding, y + font.getSize() + padding * 1.25);
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
    void Gui::attachColor( string name, ofColor * color ){
        colors[name] = color;
    }
    
    //--------------------------------------------------------------
    void Gui::attachFloat( string name, float * val ){
        floatValues[name] = val;
    }
    
    //--------------------------------------------------------------
    void Gui::min(){
        bMinimized = true;
    }
    
    //--------------------------------------------------------------
    void Gui::max(){
        bMinimized = false;
    }
    
    //--------------------------------------------------------------
    int Gui::getWidth(){
        return rectWidth;
    }
    
    //--------------------------------------------------------------
    int Gui::getHeight(){
        return height;
    }
    
    //--------------------------------------------------------------
    void Gui::randomize(){
        colorManagerW.randomize();
        colorManagerB.randomize();
    }
    
    //--------------------------------------------------------------
    vector<ofColor> Gui::getActive(){
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
    void Gui::mouseMoved(ofMouseEventArgs & m ){
        colorManagerW.checkHit(m.x - x, m.y - y, false, false);
        colorManagerB.checkHit(m.x - x, m.y - y, false, false);
        
        ofRectangle hitRect(x,y,rectWidth,height);
        if ( hitRect.inside(m.x, m.y)){
            bMouseOver = true;
        } else {
            bMouseOver = false;
        }
    }
    
    //--------------------------------------------------------------
    void Gui::mouseDragged(ofMouseEventArgs & m){
        if ( contrastRect.inside(m.x-x, m.y-y)&& !bMinimized){
            float fx = (m.x-x) - contrastRect.x;
            *floatValues["contrast"] = ofMap(fx, 0, contrastRect.width, 1.0, CONTRAST_MAX, true);
        }
    }
    
    //--------------------------------------------------------------
    void Gui::mousePressed(ofMouseEventArgs & m){
        ofColor a,b;
        if (!bMinimized)
            a = colorManagerW.checkHit(m.x - x, m.y - y, true, !ofGetKeyPressed(OF_KEY_SHIFT));
        if (!bMinimized)
            b = colorManagerB.checkHit(m.x - x, m.y - y, true, !ofGetKeyPressed(OF_KEY_SHIFT));
        
        bool isContrastNew = false;
        if ( contrastRect.inside(m.x-x, m.y-y) && !bMinimized ){
            float fx = (m.x-x) - contrastRect.x;
            float old = *floatValues["contrast"];
            *floatValues["contrast"] = ofMap(fx, 0, contrastRect.width, 1.0, CONTRAST_MAX, true);
            isContrastNew = old != *floatValues["contrast"];
        } else if ( minRect.inside(m.x-x, m.y-y)){
            bMinimized = !bMinimized;
        }
        
        if ( a != ofColor(0,0) || b != ofColor(0,0) || isContrastNew) bFrameNew = true;
    }
    
    //--------------------------------------------------------------
    void Gui::notifyRandomize(bool & b){
        ofNotifyEvent(doRandomize);
        randomize();
    }
    
    //--------------------------------------------------------------
    void Gui::setTitle( string t ){
        title = t;
        while ( font.stringWidth(title) + padding * 2.0 + 50 > rectWidth ){
            title = title.substr(0,title.size()-1);
        }
    }
    
    //--------------------------------------------------------------
    ColorManager & Gui::getColorBlack(){
        return colorManagerB;
    }
    
    //--------------------------------------------------------------
    ColorManager & Gui::getColorWhite(){
        return colorManagerW;
    }
    
    //--------------------------------------------------------------
    bool Gui::isFrameNew(){
        bool ret = bFrameNew;
        bFrameNew = false;
        return ret;
    }
    
#pragma mark Button
    
    Button::Button() : ofRectangle(),
    backgroundFill(255,0){
        enabled = true;
    }
    
    //--------------------------------------------------------------
    void Button::setup( string fontLoc, string t, float scale, int fontSize ){
        text = t;
        bInvert = false;
        width = (GUI_WIDTH * scale) / 2.0;
        height = GUI_HEIGHT * scale;
        padding = GUI_PADDING * scale;
        font.loadFont( fontLoc, fontSize * (scale == 1.0 ? 1.0 : scale * 1.1) );
        font.setLetterSpacing(.95);
        font.setSpaceSize(.6);
        ofAddListener(ofEvents().draw, this, &Button::draw);
        ofAddListener(ofEvents().mouseMoved, this, &Button::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &Button::mouseReleased);
    }
    
    //--------------------------------------------------------------
    void Button::draw(ofEventArgs & e){
        ofPushStyle();
        ofFill();
        ofSetColor(backgroundFill);
        ofRect(*this);
        
        if ( enabled ) ofSetColor(255);
        else ofSetColor(150,100);
        
        if ( !bInvert ) ofNoFill();
        else ofFill();
        ofSetLineWidth(2.0);
        ofRect(*this);
        
        if ( bInvert ) ofSetColor(150.0);
        font.drawString(text, x+ padding, y + font.getSize() + padding );
        ofPopStyle();
    }
    
    //--------------------------------------------------------------
    void Button::mouseMoved(ofMouseEventArgs & m ){
        if ( !enabled ) return;
        if ( inside(m.x, m.y)){
            bInvert = true;
        } else {
            bInvert = false;
        }
    }
    
    //--------------------------------------------------------------
    void Button::mousePressed(ofMouseEventArgs & m){
        if ( !enabled ) return;
        if ( inside(m.x, m.y)){
            bInvert = true;
        }
    }
    
    //--------------------------------------------------------------
    void Button::mouseReleased(ofMouseEventArgs & m){
        if ( !enabled ) return;
        bInvert = false;
        if ( inside(m.x, m.y)){
            static bool b = true;
            ofNotifyEvent(onPressed, b, this);
        }
    }
}

