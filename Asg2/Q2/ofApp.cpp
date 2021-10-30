#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	gui.add(lowThreshold.setup("low threshold", 50, 0, 100));
	myVideoGrabber.initGrabber(320, 240);
	
}

//--------------------------------------------------------------
void ofApp::update() {
	myVideoGrabber.update();

	if (myVideoGrabber.isFrameNew()) {
		imgCam.setFromPixels(myVideoGrabber.getPixels());
		matCam = toCv(imgCam);
		matRealCam = toCv(imgCam);
		cvtColor(matCam, matCam, CV_BGR2GRAY);
		GaussianBlur(matCam, matCam, 3);
		Canny(matCam, matCam, lowThreshold, lowThreshold * 2);
		HoughCircles(matCam, circles, CV_HOUGH_GRADIENT, 2, 50, lowThreshold * 2, 100, 30, 50);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(255, 255, 255);
	ofSetColor(255);
	drawMat(matRealCam, 0, 0);
	drawMat(matCam, imgCam.getWidth(), 0);
	for (int i = 0; i < circles.size(); i++) {
		ofSetColor(255, 255, 255); 
		ofDrawCircle(circles[i][0], circles[i][1], circles[i][2]);

		ofSetColor(0, 0, 0);
		ofDrawCircle(circles[i][0], circles[i][1], circles[i][2] / 2);
	}
	gui.draw();

}
