#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	gui.add(lowThreshold.setup("low threshold", 50, 0, 100));
	im.load("test1.jpg");
	im.setImageType(OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (curThr != lowThreshold)
	{
		curThr = lowThreshold;
		mat = toCv(im);
		cvtColor(mat, mat, CV_BGR2GRAY);
		GaussianBlur(mat, mat, 3);
		Canny(mat, edgeResult, lowThreshold, lowThreshold * 2);

		for (int x = 0; x < edgeResult.cols; x++) {
			for (int y = 0; y < edgeResult.rows; y++) {

				if (isPointOnLine(edgeResult, y, x))
				{
					if (foundStart == false) {
						foundStart = true;
						startX = x;
					}
					else
					{
						endX = x;
					}
				}
			}
		}

		if (curX == 0)
		{
			curX = startX;
		}	
	}

	if (curX == endX) { orientation = true; }
	if (curX == startX) { orientation = false; }

	if (orientation == false) {
		if (curX < endX) {
			curX++;
		}
	}
	else {
		if (curX > startX) {
			curX--;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	im.draw(0, 0);
	drawMat(edgeResult, im.getWidth(), 0);

	for (int y = 0; y < edgeResult.rows; y++) {
		if (isPointOnLine(edgeResult, y, curX)) {
			ofSetColor(255, 255, 255);
			ofDrawCircle(curX, y, 15);
			break;
		}
	}

	gui.draw();
}

bool ofApp::isPointOnLine(Mat mat, int y, int x) {
	return mat.at<uchar>(y, x) == 255 ? true : false;
}