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

	bool isPointOnLine(Mat mat, int y, int x);

	ofImage im;
	Mat mat;
	Mat edgeResult;

	ofxPanel gui;

	ofxIntSlider lowThreshold;


	int startX = 0;
	int endX = 0;
	int curX = 0;
	int curThr = 0;

	bool foundStart = false;
	bool orientation = false;
};