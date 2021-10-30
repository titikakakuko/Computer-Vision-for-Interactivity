#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"

using namespace ofxCv;
using namespace cv;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	ofVideoGrabber myVideoGrabber;
	ofImage imgCam;
	Mat matCam;
	Mat matRealCam;

	vector<Vec3f> circles;

	ofxPanel gui;
	ofxIntSlider lowThreshold;


};