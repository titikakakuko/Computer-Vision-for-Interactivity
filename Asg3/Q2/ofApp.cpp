#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	gui.add(minH.setup("min H", 0, 0, 180));
	gui.add(maxH.setup("max H", 180, 0, 180));
	gui.add(minS.setup("min S", 0, 0, 255));
	gui.add(maxS.setup("max S", 255, 0, 255));
	gui.add(minV.setup("min V", 0, 0, 255));
	gui.add(maxV.setup("max V", 255, 0, 255));

	vidGrabber.initGrabber(960, 720);
}

//--------------------------------------------------------------
void ofApp::update() {
	vidGrabber.update();
	//do we have a new frame?
	if (vidGrabber.isFrameNew()) {
		im.setFromPixels(vidGrabber.getPixels());
		mat = toCv(im);
		cvtColor(mat, mat_HSV, CV_BGR2HSV);
		inRange(mat_HSV, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), mat_HSV_Threshold);
		erode(mat_HSV_Threshold, mat_HSV_Threshold, Mat());
		dilate(mat_HSV_Threshold, mat_HSV_Threshold, Mat());

		ofImage im_temp;
		ofxCvGrayscaleImage im_temp_gray;

		toOf(mat_HSV_Threshold, im_temp);

		im_temp_gray.setFromPixels(im_temp.getPixels());

		contourFinder.findContours(im_temp_gray, 5, (960 * 720) / 4, 4, false, true);
		/*detector->detect(mat_HSV_Threshold, keypoints);

		drawKeypoints(mat, keypoints, result, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);*/
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	drawMat(mat, 0, 0);
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		ofRectangle r = contourFinder.blobs.at(i).boundingRect;

		centerX = r.x + (r.width / 2);
		centerY = r.y + (r.height / 2);
		
		if (drawMode) {
			ofDrawBitmapString("Turned on the Drawing mode.", 200, 200);
			ofSetColor(0, 255, 0);
			drawLine[lineNo].addVertex(centerX, centerY);
			ofDrawCircle(centerX, centerY, 2);
			drawLine[lineNo].draw();
		}

		ofSetColor(255, 0, 0);
		ofNoFill();
		ofDrawRectangle(r);
	}
	ofSetColor(0, 255, 0);
	for (int i = 0; i <= lineNo; i++)
	{
		drawLine[i].draw();
	}
	ofSetColor(255, 255, 255);
	drawMat(mat_HSV_Threshold, mat.cols, 0);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 's') {
		if (!drawMode) {
			drawMode = true;
		}
		else {
			drawMode = false;
			lineNo++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}
