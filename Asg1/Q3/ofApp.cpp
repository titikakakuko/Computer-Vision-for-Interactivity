#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	myVideoGrabber.initGrabber(320, 240);
}

//--------------------------------------------------------------
void ofApp::update() {

	//1)	Capture the real-time grayscale image stream in grayscale from a webcam
	myVideoGrabber.update();
	if (myVideoGrabber.isFrameNew()) {
		imgCam.setFromPixels(myVideoGrabber.getPixels());
		matCam = toCv(imgCam);
		cvtColor(matCam, matCam, CV_BGR2GRAY);
		if (isClick) {

			for (int y = 0; y < matCam.rows; y++) {
				for (int x = 0; x < matCam.cols; x++) {
					if (int(matCam.at<uchar>(y, x)) == intensity) {
						matCam.at<uchar>(y, x) = uchar(255 - intensity);
					}
				}
			}
		}

		//2)	Plot the histogram of each frame in real time
		int histSize = 256;
		float range[] = { 0, 256 }; 
		const float* histRange = { range };
		calcHist(&matCam, 1, 0, Mat(), hist, 1, &histSize, &histRange);
		normalize(hist, hist, 0, 400, NORM_MINMAX);
		for (int i = 0; i < histSize; i++) {
			histCols[i].clear();
			histCols[i].addVertex(i + 10, 768);
			histCols[i].addVertex(i + 10, 768 - cvRound(hist.at<float>(i)));
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(255, 255, 255);
	ofSetColor(255);
	drawMat(matCam, 0, 0);
	ofSetColor(0, 0, 0);
	for (int i = 0; i < 256; i++) {
		histCols[i].draw();
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//3)	Mouse-click interaction
	if (button == 0) {
		if (x<= imgCam.getWidth() && y<=imgCam.getHeight())
		{
			isClick = true;
			intensity = int(matCam.at<uchar>(y, x));
		}
		else
		{
			isClick = false;
		}
		
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

