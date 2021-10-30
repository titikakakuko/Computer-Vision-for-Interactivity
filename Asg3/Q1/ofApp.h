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

	ofVideoPlayer vid;
	ofImage vidImg;
	Mat vidMat;

	CascadeClassifier face_cascade; //the instance of face detector
	CascadeClassifier eye_cascade; //the instance of eye detector
	std::vector<cv::Rect> faces; // a vector to store the detected faces
	std::vector<cv::Rect> eyes; // a vector to store the detected eyes


};