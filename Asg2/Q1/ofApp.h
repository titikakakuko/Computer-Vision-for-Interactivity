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

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);


	ofVideoPlayer vid;
	ofImage vidFrame;
	Mat vidFrameMat;
	Mat vidRealMat;
	Mat vidFrameMatGray;
	Mat vidFrameMatEdge;

	ofxPanel gui;
	ofxIntSlider lowThresholdEdge;
	ofxIntSlider lineVoteThreshold;

	Mat mask;
	vector<cv::Point> keyPoints;

	vector<Vec4i> lines;


};