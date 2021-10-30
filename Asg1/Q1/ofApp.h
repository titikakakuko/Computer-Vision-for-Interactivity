#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

using namespace cv;
using namespace ofxCv;


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	ofImage imgMan;
	ofImage imgBg;

	Mat matMan;
	Mat matBg;

	Mat RGB[3];

	Mat matNewImage;

	bool isGreen(Mat mat, int x, int y);

};