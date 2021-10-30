#include "ofApp.h"
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	gui.add(minH.setup("min H", 0, 0, 180));
	gui.add(maxH.setup("max H", 180, 0, 180));
	gui.add(minS.setup("min S", 0, 0, 255));
	gui.add(maxS.setup("max S", 255, 0, 255));
	gui.add(minV.setup("min V", 0, 0, 255));
	gui.add(maxV.setup("max V", 255, 0, 255));

	gui.add(R.setup("R", 0, 0, 255));
	gui.add(G.setup("G", 0, 0, 255));
	gui.add(B.setup("B", 0, 0, 255));
	gui.add(width.setup("width", 1, 0, 50));

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

	// 5 different angle image of bingbing
	imgFront.load("frontFace.jpg");
	matFront = toCv(imgFront);
	imgLeft.load("leftFace.jpg");
	matLeft = toCv(imgLeft);
	imgRight.load("rightFace.jpg");
	matRight = toCv(imgRight);
	imgUp.load("upFace.jpg");
	matUp = toCv(imgUp);
	imgDown.load("downFace.jpg");
	matDown = toCv(imgDown);

	// music
	mySound.load("China-X.mp3");
	mySound.play();
}

void ofApp::initPointsToTrack(cv::Size subPixWinSize, TermCriteria termcrit) {

	// Because of my OpenCv version, I use CASCADE_SCALE_IMAGE instead of CV_HAAR_SCALE_IMAGE
	// You can adjust for your own version of OpenCv. 
	// Here are the reference links: https://docs.opencv.org/3.1.0/d9/d31/group__objdetect__c.html#ga812f46d031349fa2ee78a5e7240f5016
	//                               https://answers.opencv.org/question/20066/simple-face-recognition/
	face_cascade.detectMultiScale(matCam, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	for (int i = 0; i < faces.size(); i++) {

		eye_cascade.detectMultiScale(matCam, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));
		//if no eye has been detected, we should skip 
		if (eyes.size() < 2)
		{
			continue;
		}
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
			if ((abs(averageDir.x) - abs(prevAverageDir.x)) / abs(prevAverageDir.x) > 0.3) {
				status = "left";
			}
			else if ((abs(averageDir.x) - abs(prevAverageDir.x)) / abs(prevAverageDir.x) < -0.3)
			{
				status = "right";
			}
			else if ((abs(averageDir.y) - abs(prevAverageDir.y)) / abs(prevAverageDir.y) > 0.1) {
				status = "down";
			}
			else if ((abs(averageDir.y) - abs(prevAverageDir.y)) / abs(prevAverageDir.y) < -0.1)
			{
				status = "up";
			}
			else {
				status = "Still";
			}
			prevAverageDir = averageDir;

		}

		std::swap(curPoints, prePoints);
		cv::swap(matCamPreGrey, matCamGrey);

		cvtColor(matCam, mat_HSV, CV_BGR2HSV);
		inRange(mat_HSV, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), mat_HSV_Threshold);
		erode(mat_HSV_Threshold, mat_HSV_Threshold, Mat());
		dilate(mat_HSV_Threshold, mat_HSV_Threshold, Mat());

		ofImage im_temp;
		ofxCvGrayscaleImage im_temp_gray;

		toOf(mat_HSV_Threshold, im_temp);

		im_temp_gray.setFromPixels(im_temp.getPixels());

		contourFinder.findContours(im_temp_gray, 5, (640 * 480) / 4, 4, false, true);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	drawMat(matCam, 0, 0);
	ofDrawBitmapString("Current status: ", 20, 20);
	ofDrawBitmapString(status, 20, 40);
	/*if (!prePoints.empty()) {
		for (int i = 0; i < curPoints.size(); i++)
		{
			ofSetColor(0, 255, 0);
			ofDrawCircle(curPoints[i].x, curPoints[i].y, 3);
			ofSetColor(255, 0, 0);
			ofDrawLine(prePoints[i].x, prePoints[i].y, curPoints[i].x, curPoints[i].y);
		}
	}*/

	face_cascade.detectMultiScale(matCam, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	for (int i = 0; i < faces.size(); i++) {

		eye_cascade.detectMultiScale(matCam, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, cv::Size(30, 30));
		if (eyes.size() < 2)
		{
			continue;
		}
		for (int j = 0; j < 2; j++) {

			if ((faces.at(i).x < eyes.at(j).x) &&
				((faces.at(i).x + faces.at(i).width) > eyes.at(j).x) &&
				(faces.at(i).y < eyes.at(j).y) &&
				((faces.at(i).y + faces.at(i).height) > eyes.at(j).y)) {
				if (status == "Still")
				{
					imgFront.draw(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
				}
				else if (status == "left")
				{
					imgLeft.draw(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
				}
				else if (status == "right")
				{
					imgRight.draw(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
				}
				else if (status == "up")
				{
					imgUp.draw(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
				}
				else if (status == "down")
				{
					imgDown.draw(faces.at(i).x, faces.at(i).y, faces.at(i).width, faces.at(i).height);
				}
			}
		}
	}

	for (int i = 0; i < contourFinder.nBlobs; i++) {
		ofRectangle r = contourFinder.blobs.at(i).boundingRect;

		centerX = r.x + (r.width / 2);
		centerY = r.y + (r.height / 2);

		if (drawMode) {
			ofDrawBitmapString("Turned on the Drawing mode.", 200, 200);
			if (usingObjColor) {
				curColor[lineNo] = myVideoGrabber.getPixelsRef().getColor(centerX, centerY);
			}
			else
			{
				curColor[lineNo] = ofColor(R, G, B);
			}
			ofSetColor(curColor[lineNo]);
			drawLine[lineNo].addVertex(centerX, centerY+matCam.rows);
			curWidth[lineNo] = width;
			ofDrawCircle(centerX, centerY+matCam.rows, curWidth[lineNo]);
			ofSetLineWidth(curWidth[lineNo]);
			drawLine[lineNo].draw();
		}

		ofSetColor(255, 0, 0);
		ofNoFill();
		ofDrawRectangle(r);
	}


	for (int i = 1; i <= lineNo; i++)
	{
		ofSetColor(curColor[i]);
		ofSetLineWidth(curWidth[i]);
		drawLine[i].draw();
	}
	ofSetColor(255, 255, 255);
	drawMat(mat_HSV_Threshold, matCam.cols, 0);
	gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
	case 'c':
		cout << "clear points" << endl;
		prePoints.clear();
		curPoints.clear();
		status = "Init";
		break;
	case 'r':
		cout << "re-initialize" << endl;
		needToInit = true;
		break;
	case 'd':
		if (!drawMode) {
			drawMode = true;
			lineNo++;
		}
		else {
			drawMode = false;
		}
		break;
	case 'i':
		lineNo = 0;
		break;
	case 'm':
		mySound.play();
		break;
	case 'n':
		mySound.stop();
		break;
	case 'o':
		usingObjColor = !usingObjColor;
		break;
	case 's':
		outputDraw.grabScreen(0, matCam.rows, matCam.cols, matCam.rows);
		outputDraw.save(filename + ofToString(countSketch, 2, '0') + ".png");
		countSketch++;
		break;
	}

}