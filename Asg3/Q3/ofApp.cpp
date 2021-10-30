#include "ofApp.h"
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup() {
	myVideoGrabber.initGrabber(640, 480);
	prevAverageDir = Point2f(0, 0);
	directions_thresholdVal = 20;
	threshold_counting = 0;
	//need to use absolute path here, for the latest version of OpenFrameworks and its OpenCV addon
	if (!face_cascade.load("C:/Users/w/Desktop/cs/2020-2021/cs4187cv/faceDetection_Image/bin/data/haarcascade_frontalface_default.xml")) {
		cout << "Error loading haarcascade_frontalface_default.xml" << endl;
	}

	if (!eye_cascade.load("C:/Users/w/Desktop/cs/2020-2021/cs4187cv/faceDetection_Image/bin/data/haarcascade_eye.xml")) {
		cout << "Error loading haarcascade_eye.xml" << endl;
	}
}

void ofApp::initPointsToTrack(cv::Size subPixWinSize, TermCriteria termcrit) {

	face_cascade.detectMultiScale(matCam, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	for (int i = 0; i < faces.size(); i++) {

		eye_cascade.detectMultiScale(matCam, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));
		for (int j = 0; j < 2; j++) {
			
			if ((faces.at(i).x < eyes.at(j).x) &&
				((faces.at(i).x + faces.at(i).width) > eyes.at(j).x) &&
				(faces.at(i).y < eyes.at(j).y) &&
				((faces.at(i).y + faces.at(i).height) > eyes.at(j).y)) {

				Rect roi(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
				mask = Mat::zeros(matCamGrey.size(), CV_8UC1);
				mask(roi).setTo(255);
			}
		}
	}

	goodFeaturesToTrack(matCamGrey, curPoints, MAX_COUNT, 0.01, 10, mask, 3, false, 0.04);
	mask.setTo(0);
	cornerSubPix(matCamGrey, curPoints, subPixWinSize, cv::Size(-1, -1), termcrit);

}

void ofApp::trackKeyPoints(cv::Size winSize) {

	vector<uchar> status;
	vector<float> err;
	if (matCamPreGrey.empty()) {
		matCamGrey.copyTo(matCamPreGrey);
	}
	calcOpticalFlowPyrLK(matCamPreGrey, matCamGrey, prePoints, curPoints, status, err, winSize);

}

void ofApp::getDirections() {
	for (int i = 0; i < curPoints.size(); i++) {
		Point2f dir = Point2f(curPoints.at(i).x - prePoints.at(i).x, curPoints.at(i).y - prePoints.at(i).y);
		directions.push_back(dir);
	}
}

Point2f ofApp::getAverageDirection() {
	Point2f averageDir(0, 0);
	for (int i = 0; i < directions.size(); i++) {
		averageDir.x += directions.at(i).x;
		averageDir.y += directions.at(i).y;
	}
	if (directions.size() != 0) {
		averageDir.x /= directions.size();
		averageDir.y /= directions.size();
	}
	return averageDir;
}


//--------------------------------------------------------------
void ofApp::update() {
	myVideoGrabber.update();

	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	cv::Size subPixWinSize(10, 10), winSize(31, 31);

	// If the grabber indeed has fresh data,
	if (myVideoGrabber.isFrameNew()) {
		// Obtain a pointer to the grabber's image data.
		imgCam.setFromPixels(myVideoGrabber.getPixels());
		matCam = toCv(imgCam);
		cvtColor(matCam, matCamGrey, CV_BGR2GRAY);
		if (needToInit) {
			initPointsToTrack(subPixWinSize, termcrit);
			needToInit = false;
		}
		else if (!prePoints.empty()) {
			trackKeyPoints(winSize);
			getDirections();
			Point2f averageDir = getAverageDirection();
			
			//the threshold needs to be adjusted
			if (abs(abs(averageDir.x) - abs(prevAverageDir.x)) / abs(prevAverageDir.x) > 0.5) {
				status = "Shaking";
			}

			else if (abs(abs(averageDir.y) - abs(prevAverageDir.y)) / abs(prevAverageDir.y) > 0.5) {
				status = "Nodding";
			}
			else {
				status = "Still";
			}
			prevAverageDir = averageDir;

		}

		std::swap(curPoints, prePoints);
		cv::swap(matCamPreGrey, matCamGrey);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	drawMat(matCam, 0, 0);
	ofDrawBitmapString("Current status: ", 20, 20);
	ofDrawBitmapString(status, 20, 40);
	if (!prePoints.empty()) {
		for (int i = 0; i < curPoints.size(); i++)
		{
			ofSetColor(0, 255, 0);
			ofDrawCircle(curPoints[i].x, curPoints[i].y, 3);
			ofSetColor(255, 0, 0);
			ofDrawLine(prePoints[i].x, prePoints[i].y, curPoints[i].x, curPoints[i].y);
		}
	}



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case 'c':
		cout << "clear points" << endl;
		prePoints.clear();
		curPoints.clear();
		break;
	case 'r':
		cout << "re-initialize" << endl;
		needToInit = true;
		break;
	}

}