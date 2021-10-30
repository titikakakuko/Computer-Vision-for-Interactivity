#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	gui.setup();
	gui.add(lowThresholdEdge.setup("low threshold", 50, 0, 100));
	gui.add(lineVoteThreshold.setup("line vote threshold", 150, 0, 200));
	vid.load("solidWhiteRight.mp4");
	vid.play();

}

//--------------------------------------------------------------
void ofApp::update() {

	vid.update();
	vid.getCurrentFrame();
	if (vid.isFrameNew()) {
		ofPixels pixs = vid.getPixels();
		vidFrame.setFromPixels(pixs);
		vidRealMat = toCv(vidFrame);
		vidFrameMat = toCv(vidFrame);
		cvtColor(vidFrameMat, vidFrameMatGray, CV_BGR2GRAY);
		GaussianBlur(vidFrameMatGray, vidFrameMatGray, 3);
		Canny(vidFrameMatGray, vidFrameMatEdge, lowThresholdEdge, lowThresholdEdge * 3);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255, 255, 255);
	drawMat(vidRealMat, 0, 0);

	mask = Mat::zeros(cv::Size(vid.getWidth(), vid.getHeight()), CV_8U);

	for (int i = 0; i < keyPoints.size(); i++) {
		ofSetColor(255, 0, 0);
		ofDrawCircle(keyPoints[i].x, keyPoints[i].y, 5);
	}

	if (keyPoints.size() >= 3) {
		fillConvexPoly(mask, keyPoints.data(), keyPoints.size(), Scalar(255, 255, 255));
		bitwise_and(vidFrameMatEdge, mask, vidFrameMatEdge);
	}

	vector<Vec4i> lines;
	HoughLinesP(vidFrameMatEdge, lines, 2, CV_PI / 180, lineVoteThreshold, 15, 20);
	ofSetColor(255, 0, 0);
	for (int i = 0; i < lines.size(); i++) {
		float x1 = lines[i][0];
		float y1 = lines[i][1];
		float x2 = lines[i][2];
		float y2 = lines[i][3];
		ofPolyline l;
		l.addVertex(x1, y1);
		l.addVertex(x2, y2);

		l.draw();
	}

	gui.draw();

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (x >= 0 && x < vid.getWidth() && y >= 0 && y < vid.getHeight()) {
		cv::Point p(x, y);
		keyPoints.push_back(p);
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}
