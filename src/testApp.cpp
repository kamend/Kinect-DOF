#include "testApp.h"
#include "ofxMeshSaver.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetSmoothLighting(true);
	ofEnableAlphaBlending();
	
	
	// - Shaders
	dofPass.load("shaders/dofPass");
	depthPass.load("shaders/depthPass");
	
	// - FBO
	depthFbo.allocate(ofGetWidth(), ofGetHeight());
	colorFbo.allocate(ofGetWidth(), ofGetHeight());
	
	
	depthFbo.begin();
	ofClear(0,0,0,0);
	depthFbo.end();
	
	colorFbo.begin();
	ofClear(0,0,0,0);
	colorFbo.end();
	// - Kinect
	
	kinect.init();
	kinect.open();
	kinect.setRegistration(true);
		
	// lights

	light.setDirectional();
	light.setOrientation(ofVec3f(0,90,0));

	
	// - GUI
	
	gui.addPage("Kinect DOF");
	gui.addSlider("Near Th", nearTh, 0, 10000);
	gui.addSlider("Far Th", farTh, 0, 10000);
	gui.addSlider("Left Th", leftTh, 0, 640);
	gui.addSlider("Right Th", rightTh, 0, 640);
	gui.addSlider("Top Th", topTh, 0, 480);
	gui.addSlider("Bottom Th", bottomTh, 0, 480);
	gui.addSlider("Simplification", simplifaction, 1, 16);
	
	gui.addSlider("dof24 Focal Depth", dof24_focalDepth, 1.0,20.0);
	gui.addSlider("dof24 Focal Length", dof24_focalLength, 0, 10);
	gui.addSlider("dof24 fstop", dof24_fstop, 0, 24);
	gui.addSlider("dof24_maxblur", dof24_maxblur, 0,10.0);
	gui.addToggle("dof24 show", dof24_showFocus);
	
	gui.addSlider("Light X", directionLightX, -1000, 1000);
	gui.addSlider("Light Y", directionLightY, -1000, 1000);
	gui.addSlider("Light Z", directionLightZ, -2000, 2000);
	
	gui.addSlider("cubeSize Min", cubeSizeMin, 0, 100);
	gui.addSlider("cubeSize Max", cubeSizeMax, 0, 100);
	
	gui.addSlider("Depth Far", depth_Far, 0,10000);
	gui.addSlider("Depth Near", depth_Near, 0, 100);
	
	gui.loadFromXML();

}

//--------------------------------------------------------------
void testApp::update(){	
	light.setOrientation(ofVec3f(directionLightX, directionLightY, directionLightZ));


	kinect.update();

	if(gui.isOn()) {
		cam.disableMouseInput();
	} else {
		cam.enableMouseInput();
	}
	
	
}


//--------------------------------------------------------------
void testApp::drawBoxes() {
	for(int x=0;x<640;x+=simplifaction*2) {
		for(int y=0;y<480;y+=simplifaction*2) {
			
			float depth = kinect.getDistanceAt(x, y);
			
			if(depth > nearTh && depth < farTh) {
				ofVec3f wc = kinect.getWorldCoordinateAt(x, y);
				ofColor c = kinect.getColorAt(x, y);
				
				c *= 60.0;
				ofSetColor(c);
				//ofSphere(wc.x, wc.y,wc.z,  ofMap(ofNoise(x,y),0,1.0,cubeSizeMin,cubeSizeMax));
				ofBox(wc.x,wc.y,wc.z, ofMap(ofNoise(x,y),0,1.0,cubeSizeMin,cubeSizeMax));
			}
			
		}
	}
}
//--------------------------------------------------------------
void testApp::drawScene() {
	cam.begin();
	ofPushMatrix();
	ofTranslate(0,0, 1800);
	ofScale(2,2,-2);
	drawBoxes();
	ofPopMatrix();
	cam.end();
}

//--------------------------------------------------------------
void testApp::draw(){
	glEnable(GL_DEPTH_TEST);
	ofEnableAlphaBlending();
	// depth pass
	depthFbo.begin();
	ofClear(0,0,0,0);
	depthPass.begin();
	depthPass.setUniform1f("near", depth_Near);
	depthPass.setUniform1f("far", depth_Far);
	drawScene();
	depthPass.end();
	depthFbo.end();
	
	// color Pass
	ofEnableLighting();
	light.enable();
	colorFbo.begin();
	ofClear(0,0,0,0);
	drawScene();
	colorFbo.end();
	light.disable();
	ofDisableLighting();

	
	ofBackgroundGradient(ofColor(0,0,0), ofColor(80,80,80));

	dofPass.begin();
	dofPass.setUniformTexture("bgl_RenderedTexture", colorFbo.getTextureReference(), 0);
	dofPass.setUniformTexture("bgl_DepthTexture", depthFbo.getTextureReference(), 1);
	dofPass.setUniform1f("bgl_RenderedTextureWidth", ofGetWidth());
	dofPass.setUniform1f("bgl_RenderedTextureHeight", ofGetHeight());
	dofPass.setUniform1f("focalDepth", dof24_focalDepth);
	dofPass.setUniform1f("focalLength", dof24_focalLength);
	dofPass.setUniform1f("fstop", dof24_fstop);
	dofPass.setUniform1f("maxblur", dof24_maxblur);
	dofPass.setUniform1i("showFocus", dof24_showFocus);
	colorFbo.draw(0, 0);
	dofPass.end();

	
	ofDisableAlphaBlending();
	// GUI
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	gui.draw();
	glPopAttrib();
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
}

void testApp::exit() {
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == ' ') {
		gui.toggleDraw();
	}

	
	if(key == 's') {
		dofPass.load("shaders/dof24");
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}