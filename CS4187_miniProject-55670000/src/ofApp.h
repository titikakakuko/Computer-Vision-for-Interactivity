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

	void keyPressed(int key);
	void initPointsToTrack(cv::Size subPixWinSize, TermCriteria termcrit);
	void trackKeyPoints(cv::Size winSize);
	void getDirections();
	Point2f getAverageDirection();

	ofVideoGrabber myVideoGrabber;
	ofImage imgCam;
	Mat matCam;

	Mat matCamGrey;
	Mat matCamPreGrey;

	Mat mask;

	ofImage imgFront;
	ofImage imgLeft;
	ofImage imgRight;
	ofImage imgUp;
	ofImage imgDown;

	Mat matFront;
	Mat matLeft;
	Mat matRight;
	Mat matUp;
	Mat matDown;

	vector<Point2f> prePoints;
	vector<Point2f> curPoints;

	vector<ofPolyline> tracks;

	vector<Point2f> directions;

	Point2f point;
	Point2f prevAverageDir;
	bool addRemovePt = false;

	int MAX_COUNT = 500;
	bool needToInit = false;

	int directions_thresholdVal;
	int threshold_counting;

	CascadeClassifier face_cascade; //the instance of face detector
	CascadeClassifier eye_cascade; //the instance of eye detector
	std::vector<cv::Rect> faces; // a vector to store the detected faces
	std::vector<cv::Rect> eyes; // a vector to store the detected eyes


	Mat mat_HSV;

	Mat mat_HSV_Threshold;

	ofxPanel gui;
	ofxIntSlider minH;
	ofxIntSlider maxH;
	ofxIntSlider minS;
	ofxIntSlider maxS;
	ofxIntSlider minV;
	ofxIntSlider maxV;

	ofxIntSlider R;
	ofxIntSlider G;
	ofxIntSlider B;
	ofxIntSlider width;

	String status = "Init";
	Mat result;

	ofxCvContourFinder contourFinder;

	ofPolyline drawLine[10];
	ofColor curColor[10];
	float curWidth[10];
	int lineNo = 0;
	bool drawMode = false;
	bool usingObjColor = false;

	int centerX;
	int centerY;

	ofSoundPlayer mySound;

	ofImage outputDraw;
	String filename = "mySketch_";
	int countSketch = 1;
};