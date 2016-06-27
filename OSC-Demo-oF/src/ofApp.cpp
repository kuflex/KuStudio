#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Visuals for KuStudio demo-project");
	ofSetWindowShape(500, 500);
	ofSetFrameRate(30);

	osc_in_port = 12345;		//OSC in port
	osc_receiver.setup(osc_in_port);	//Start listening port
	
	radius = 200;
	count = 3;
	rotate = 45;

	angle = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	//Read values from OSC
	while (osc_receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		osc_receiver.getNextMessage(m);
		string name = m.getAddress().substr(1);	//get address without first symbol ("/volume1" -> "volume1" )
		float value = (m.getNumArgs() >= 1 && m.getArgType(0) == OFXOSC_TYPE_FLOAT) ? m.getArgAsFloat(0) : 0;
		if (name == "radius") radius = value;
		if (name == "count") count = value;
		if (name == "rotate") rotate = value;
	}

	//Physics
	float dt = 1.0 / 60.0;
	angle += dt * rotate; //Rotation
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(0, 80);

	//Draw figure
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotate(angle);
	ofSetColor(200);
	ofNoFill();
	int k = int(count);
	for (int i = 0; i < k; i++) {
		float a = ofMap(i, 0, k, 0, M_TWO_PI);
		float b = ofMap(i + 1, 0, k, 0, M_TWO_PI);
		ofTriangle(0, 0, cos(a)*radius, sin(a)*radius, cos(b)*radius, sin(b)*radius);
	}
	ofPopMatrix();

	//Print out values
	string s = string("Demo visualisation for KuStudio made with openFrameworks\n")
		+ "Run KuStudio, open demo-project/DemoProject.kus, play it.\n"
		+ "OSC receiver port: 12345" + "\n"
		+ "/radius " + ofToString(radius, 3) + "\n"
		+ "/count " + ofToString(count, 3) + "\n"
		+ "/rotate " + ofToString(rotate, 3);
	ofSetColor(255);
	ofDrawBitmapString(s, 20, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

