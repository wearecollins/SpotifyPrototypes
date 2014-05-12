#pragma once

#include "ofMain.h"

struct Sortable {
    float max;
    int index;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        bool bRegen;
        void generateMeshes( int num );
        ofMesh generate( float x, float y );
    
        vector<ofMesh> sources, draws;
};
