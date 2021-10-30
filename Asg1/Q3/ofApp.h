#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

using namespace ofxCv;
using namespace cv;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	ofVideoGrabber myVideoGrabber;
	ofImage imgCam;
	Mat matCam;

	bool isClick = false;
	int intensity;

	Mat hist;
	Mat histImage;

	ofPolyline histogram;
	ofPolyline histCols[256];

};