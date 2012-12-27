#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxSimpleGuiToo.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();
	
	void drawBoxes();
	void drawScene();
	
	ofShader dofPass;
	ofShader depthPass;

	ofFbo depthFbo;
	ofFbo colorFbo;

	ofLight light;
	ofxKinect kinect;

	ofEasyCam cam;

	float nearTh;
	float farTh;
	float leftTh;
	float rightTh;
	float topTh;
	float bottomTh;
	int simplifaction;
	
	float dof24_focalDepth;
	float dof24_focalLength;
	float dof24_fstop;
	bool dof24_showFocus;
	float dof24_maxblur;
	
	float depth_Far;
	float depth_Near;

	
	float directionLightX;
	float directionLightY;
	float directionLightZ;
	
	float cubeSizeMin;
	float cubeSizeMax;
	
	
		
};
